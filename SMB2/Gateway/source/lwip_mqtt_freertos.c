/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_RAW && LWIP_NETCONN && LWIP_DHCP && LWIP_DNS

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "lwip/api.h"
#include "lwip/apps/mqtt.h"
#include "lwip/dhcp.h"
#include "lwip/netdb.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "lwip_mqtt_id.h"

#include "ctype.h"
#include "stdio.h"

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_device_registers.h"
#include "fsl_flexcan.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

/* PHY operations. */
#define EXAMPLE_PHY_OPS phyksz8081_ops

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/* GPIO pin configuration. */
#define BOARD_LED_GPIO       BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN   BOARD_LED_RED_GPIO_PIN
#define BOARD_SW_GPIO        BOARD_SW3_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_SW3_GPIO_PIN
#define BOARD_SW_PORT        BOARD_SW3_PORT
#define BOARD_SW_IRQ         BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER


#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

/*! @brief MQTT server host name or IP address. */
#define EXAMPLE_MQTT_SERVER_HOST "io.adafruit.com" //edit with broker

/*! @brief MQTT server port number. */
#define EXAMPLE_MQTT_SERVER_PORT 1883

/*! @brief Stack size of the temporary lwIP initialization thread. */
#define INIT_THREAD_STACKSIZE 1024

/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

/*! @brief Stack size of the temporary initialization thread. */
#define APP_THREAD_STACKSIZE 1024

/*! @brief Priority of the temporary initialization thread. */
#define APP_THREAD_PRIO DEFAULT_THREAD_PRIO

#define EXAMPLE_CAN            CAN0
#define EXAMPLE_CAN_CLK_SOURCE (kFLEXCAN_ClkSrc1)
#define EXAMPLE_CAN_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (0U)
#define RX_MESSAGE_BUFFER_NUM      (9)
#define TX_MESSAGE_BUFFER_NUM      (8)
#define DLC                        (8)

/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void connect_to_mqtt(void *ctx);
void CAN_Tx_Task(void * args);
void CAN_Rx_Task(void * args);
void CAN_Rx_KA(void * args);
void Delay_Task(void * args);
/*******************************************************************************
 * Variables
 ******************************************************************************/

static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};

/*! @brief MQTT client data. */
static mqtt_client_t *mqtt_client;

/*! @brief MQTT client ID string. */
static char client_id[40];

/*! @brief MQTT client information. */
static const struct mqtt_connect_client_info_t mqtt_client_info = {
    .client_id   = (const char *)&client_id[0],
    .client_user = "jimenez1", //changed from adafruit
    .client_pass = "", //change from adafruit --> key
    .keep_alive  = 100,
    .will_topic  = NULL,
    .will_msg    = NULL,
    .will_qos    = 0,
    .will_retain = 0,
#if LWIP_ALTCP && LWIP_ALTCP_TLS
    .tls_config = NULL,
#endif
};

/*! @brief MQTT broker IP address. */
static ip_addr_t mqtt_addr;

/*! @brief Indicates connection to MQTT broker. */
static volatile bool connected = false;
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_mb_transfer_t txXfer, rxXfer, rxKAfer;
flexcan_frame_t txFrame, rxFrame,rxKAFrame ;
flexcan_rx_mb_config_t mbConfig;

char battery_level[4] = {'0','0','0','\0'};
uint8_t g_KA_State = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Called when subscription request finishes.
 */
static void mqtt_topic_subscribed_cb(void *arg, err_t err)
{
    const char *topic = (const char *)arg;

    if (err == ERR_OK)
    {
        PRINTF("Subscribed to the topic \"%s\".\r\n", topic);
    }
    else
    {
        PRINTF("Failed to subscribe to the topic \"%s\": %d.\r\n", topic, err);
    }
}

/*!
 * @brief Called when there is a message on a subscribed topic.
 */
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    LWIP_UNUSED_ARG(arg);

    PRINTF("Received %u bytes from the topic \"%s\": \"", tot_len, topic);
}

/*!
 * @brief Called when recieved incoming published message fragment.
 */
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    int i;

    LWIP_UNUSED_ARG(arg);

    PRINTF("data asterisco %d\n", *data);

    if(*data == 79)
    {
    	LED_RED_ON();
    	/* Setup Tx Message Buffer. */
    	FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    	/* Prepare Tx Frame for sending. */
    	txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    	txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    	txFrame.id     = FLEXCAN_ID_STD(0x124);
    	txFrame.length = (uint8_t)DLC;

    	/* Send data through Tx Message Buffer. */
    	txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    	txXfer.frame = &txFrame;

    	txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x1) | CAN_WORD0_DATA_BYTE_1(0x0) | CAN_WORD0_DATA_BYTE_2(0x0) |
    						CAN_WORD0_DATA_BYTE_3(0x0);
    	txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x0) | CAN_WORD1_DATA_BYTE_5(0x0) | CAN_WORD1_DATA_BYTE_6(0x0) |
    						CAN_WORD1_DATA_BYTE_7(0x0);

		(void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
		LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
		LOG_INFO("tx word0 = 0x%x\r\n", txFrame.dataWord0);
		LOG_INFO("tx word1 = 0x%x\r\n", txFrame.dataWord1);
		txComplete = false;
    }
    else if(*data == 49)
    {
    	PRINTF("Led off \n");
        LED_RED_OFF();
    	/* Setup Tx Message Buffer. */
    	FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    	/* Prepare Tx Frame for sending. */
    	txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    	txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    	txFrame.id     = FLEXCAN_ID_STD(0x124);
    	txFrame.length = (uint8_t)DLC;

    	/* Send data through Tx Message Buffer. */
    	txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    	txXfer.frame = &txFrame;

    	txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x0) | CAN_WORD0_DATA_BYTE_1(0x0) | CAN_WORD0_DATA_BYTE_2(0x0) |
    						CAN_WORD0_DATA_BYTE_3(0x0);
    	txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x0) | CAN_WORD1_DATA_BYTE_5(0x0) | CAN_WORD1_DATA_BYTE_6(0x0) |
    						CAN_WORD1_DATA_BYTE_7(0x0);

		(void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
		LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
		LOG_INFO("tx word0 = 0x%x\r\n", txFrame.dataWord0);
		LOG_INFO("tx word1 = 0x%x\r\n", txFrame.dataWord1);
		txComplete = false;
    }
    else if(*data == 53)
    {
    	/* Setup Tx Message Buffer. */
    	FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    	/* Prepare Tx Frame for sending. */
    	txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    	txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    	txFrame.id     = FLEXCAN_ID_STD(0x124);
    	txFrame.length = (uint8_t)DLC;

    	/* Send data through Tx Message Buffer. */
    	txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    	txXfer.frame = &txFrame;

    	txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x0) | CAN_WORD0_DATA_BYTE_1(0x0) | CAN_WORD0_DATA_BYTE_2(0x0) |
    						CAN_WORD0_DATA_BYTE_3(0x0);
    	txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x05) | CAN_WORD1_DATA_BYTE_5(0x0) | CAN_WORD1_DATA_BYTE_6(0x0) |
    						CAN_WORD1_DATA_BYTE_7(0x0);

		(void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
		LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
		LOG_INFO("tx word0 = 0x%x\r\n", txFrame.dataWord0);
		LOG_INFO("tx word1 = 0x%x\r\n", txFrame.dataWord1);
		txComplete = false;
    }

    else if(*data == 55)
    {
    	/* Setup Tx Message Buffer. */
    	FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    	/* Prepare Tx Frame for sending. */
    	txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    	txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    	txFrame.id     = FLEXCAN_ID_STD(0x124);
    	txFrame.length = (uint8_t)DLC;

    	/* Send data through Tx Message Buffer. */
    	txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    	txXfer.frame = &txFrame;

    	txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x0) | CAN_WORD0_DATA_BYTE_1(0x0) | CAN_WORD0_DATA_BYTE_2(0x0) |
    						CAN_WORD0_DATA_BYTE_3(0x0);
    	txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x10) | CAN_WORD1_DATA_BYTE_5(0x0) | CAN_WORD1_DATA_BYTE_6(0x0) |
    						CAN_WORD1_DATA_BYTE_7(0x0);

		(void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
		LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
		LOG_INFO("tx word0 = 0x%x\r\n", txFrame.dataWord0);
		LOG_INFO("tx word1 = 0x%x\r\n", txFrame.dataWord1);
		txComplete = false;
    }

    else if(*data == 57)
    {
    	/* Setup Tx Message Buffer. */
    	FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    	/* Prepare Tx Frame for sending. */
    	txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    	txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    	txFrame.id     = FLEXCAN_ID_STD(0x124);
    	txFrame.length = (uint8_t)DLC;

    	/* Send data through Tx Message Buffer. */
    	txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    	txXfer.frame = &txFrame;

    	txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x0) | CAN_WORD0_DATA_BYTE_1(0x0) | CAN_WORD0_DATA_BYTE_2(0x0) |
    						CAN_WORD0_DATA_BYTE_3(0x0);
    	txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x15) | CAN_WORD1_DATA_BYTE_5(0x0) | CAN_WORD1_DATA_BYTE_6(0x0) |
    						CAN_WORD1_DATA_BYTE_7(0x0);

		(void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
		LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
		LOG_INFO("tx word0 = 0x%x\r\n", txFrame.dataWord0);
		LOG_INFO("tx word1 = 0x%x\r\n", txFrame.dataWord1);
		txComplete = false;
    }
    else
    {
    	LED_RED_OFF();
    }

}

/*!
 * @brief Subscribe to MQTT topics.
 */
static void mqtt_subscribe_topics(mqtt_client_t *client)
{
    static const char *topics[] = {"jimenez1/feeds/pr2"}; //Changed added key from adafruit-->feed-->feed info
    int qos[]                   = {0}; //Changed to be one
    err_t err;
    int i;

    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb,
                            LWIP_CONST_CAST(void *, &mqtt_client_info));

    for (i = 0; i < ARRAY_SIZE(topics); i++)
    {
        err = mqtt_subscribe(client, topics[i], qos[i], mqtt_topic_subscribed_cb, LWIP_CONST_CAST(void *, topics[i]));

        if (err == ERR_OK)
        {
            PRINTF("Subscribing to the topic \"%s\" with QoS %d...\r\n", topics[i], qos[i]);
        }
        else
        {
            PRINTF("Failed to subscribe to the topic \"%s\" with QoS %d: %d.\r\n", topics[i], qos[i], err);
        }
    }
}

/*!
 * @brief Called when connection state changes.
 */
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    const struct mqtt_connect_client_info_t *client_info = (const struct mqtt_connect_client_info_t *)arg;

    connected = (status == MQTT_CONNECT_ACCEPTED);

    switch (status)
    {
        case MQTT_CONNECT_ACCEPTED:
            PRINTF("MQTT client \"%s\" connected.\r\n", client_info->client_id);
            mqtt_subscribe_topics(client);
            break;

        case MQTT_CONNECT_DISCONNECTED:
            PRINTF("MQTT client \"%s\" not connected.\r\n", client_info->client_id);
            /* Try to reconnect 1 second later */
            sys_timeout(1000, connect_to_mqtt, NULL);
            break;

        case MQTT_CONNECT_TIMEOUT:
            PRINTF("MQTT client \"%s\" connection timeout.\r\n", client_info->client_id);
            /* Try again 1 second later */
            sys_timeout(1000, connect_to_mqtt, NULL);
            break;

        case MQTT_CONNECT_REFUSED_PROTOCOL_VERSION:
        case MQTT_CONNECT_REFUSED_IDENTIFIER:
        case MQTT_CONNECT_REFUSED_SERVER:
        case MQTT_CONNECT_REFUSED_USERNAME_PASS:
        case MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_:
            PRINTF("MQTT client \"%s\" connection refused: %d.\r\n", client_info->client_id, (int)status);
            /* Try again 10 seconds later */
            sys_timeout(10000, connect_to_mqtt, NULL);
            break;

        default:
            PRINTF("MQTT client \"%s\" connection status: %d.\r\n", client_info->client_id, (int)status);
            /* Try again 10 seconds later */
            sys_timeout(10000, connect_to_mqtt, NULL);
            break;
    }
}

/*!
 * @brief Starts connecting to MQTT broker. To be called on tcpip_thread.
 */
static void connect_to_mqtt(void *ctx)
{
    LWIP_UNUSED_ARG(ctx);

    PRINTF("Connecting to MQTT broker at %s...\r\n", ipaddr_ntoa(&mqtt_addr));

    mqtt_client_connect(mqtt_client, &mqtt_addr, EXAMPLE_MQTT_SERVER_PORT, mqtt_connection_cb,
                        LWIP_CONST_CAST(void *, &mqtt_client_info), &mqtt_client_info);
}

/*!
 * @brief Called when publish request finishes.
 */
static void mqtt_message_published_cb(void *arg, err_t err)
{
    const char *topic = (const char *)arg;

    if (err == ERR_OK)
    {
        PRINTF("Published to the topic \"%s\".\r\n", topic);
    }
    else
    {
        PRINTF("Failed to publish to the topic \"%s\": %d.\r\n", topic, err);
    }
}

/*!
 * @brief Publishes a message. To be called on tcpip_thread.
 */
static void publish_message(void *ctx)
{
    static const char *topic   = "jimenez1/feeds/pr-display";//cambio a key de adifruit-->feeds-->feed info
    char *message;

	message = battery_level;
//	LOG_INFO("message = %s\r\n", message);

    LWIP_UNUSED_ARG(ctx);

//    PRINTF("Going to publish to the topic \"%s\"...\r\n", topic);

    mqtt_publish(mqtt_client, topic, message, strlen(message), 1, 0, mqtt_message_published_cb, (void *)topic);
}

/*!
 * @brief Publishes a message. To be called on tcpip_thread.
 */
static void publish_message_KA(void *ctx)
{
    static const char *topic   = "jimenez1/feeds/pr2-ka";//cambio a key de adifruit-->feeds-->feed info
    char *message = "1";

    LWIP_UNUSED_ARG(ctx);

    PRINTF("Going to publish to the topic \"%s\"...\r\n", topic);

    mqtt_publish(mqtt_client, topic, message, strlen(message), 1, 0, mqtt_message_published_cb, (void *)topic);
}
/*!
 * @brief Publishes a message. To be called on tcpip_thread.
 */
static void publish_message_KA_OFF(void *ctx)
{
    static const char *topic   = "jimenez1/feeds/pr2-ka";//cambio a key de adifruit-->feeds-->feed info
    char *message = "0";

    LWIP_UNUSED_ARG(ctx);

    PRINTF("Going to publish to the topic \"%s\"...\r\n", topic);

    mqtt_publish(mqtt_client, topic, message, strlen(message), 1, 0, mqtt_message_published_cb, (void *)topic);
}

/*!
 * @brief Application thread.
 */
static void app_thread(void *arg)
{
    struct netif *netif = (struct netif *)arg;
    struct dhcp *dhcp;
    err_t err;
    int i;

    /* Wait for address from DHCP */

    PRINTF("Getting IP address from DHCP...\r\n");

    do
    {
        if (netif_is_up(netif))
        {
            dhcp = netif_dhcp_data(netif);
        }
        else
        {
            dhcp = NULL;
        }

        sys_msleep(20U);

    } while ((dhcp == NULL) || (dhcp->state != DHCP_STATE_BOUND));

    PRINTF("\r\nIPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
    PRINTF("IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
    PRINTF("IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));

    /*
     * Check if we have an IP address or host name string configured.
     * Could just call netconn_gethostbyname() on both IP address or host name,
     * but we want to print some info if goint to resolve it.
     */
    if (ipaddr_aton(EXAMPLE_MQTT_SERVER_HOST, &mqtt_addr) && IP_IS_V4(&mqtt_addr))
    {
        /* Already an IP address */
        err = ERR_OK;
    }
    else
    {
        /* Resolve MQTT broker's host name to an IP address */
        PRINTF("Resolving \"%s\"...\r\n", EXAMPLE_MQTT_SERVER_HOST);
        err = netconn_gethostbyname(EXAMPLE_MQTT_SERVER_HOST, &mqtt_addr);
    }

    if (err == ERR_OK)
    {
        /* Start connecting to MQTT broker from tcpip_thread */
        err = tcpip_callback(connect_to_mqtt, NULL);
        if (err != ERR_OK)
        {
            PRINTF("Failed to invoke broker connection on the tcpip_thread: %d.\r\n", err);
        }
    }
    else
    {
        PRINTF("Failed to obtain IP address: %d.\r\n", err);
    }

    /* Publish some messages */
    for (i = 0; i < 1;)
    {
        if (connected)
        {
            err = tcpip_callback(publish_message, NULL);
            if (err != ERR_OK)
            {
                PRINTF("Failed to invoke publishing of a message on the tcpip_thread: %d.\r\n", err);
            }
            i++;
        }

        sys_msleep(1000U);
    }

    vTaskDelete(NULL);
}

static void generate_client_id(void)
{
    uint32_t mqtt_id[MQTT_ID_SIZE];
    int res;

    get_mqtt_id(&mqtt_id[0]);

    res = snprintf(client_id, sizeof(client_id), "nxp_%08lx%08lx%08lx%08lx", mqtt_id[3], mqtt_id[2], mqtt_id[1],
                   mqtt_id[0]);
    if ((res < 0) || (res >= sizeof(client_id)))
    {
        PRINTF("snprintf failed: %d\r\n", res);
        while (1)
        {
        }
    }
}

/*!
 * @brief Initializes lwIP stack.
 *
 * @param arg unused
 */
static void stack_init(void *arg)
{
    static struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
    };

    LWIP_UNUSED_ARG(arg);
    generate_client_id();

    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    IP4_ADDR(&netif_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_gw, 0U, 0U, 0U, 0U);

    tcpip_init(NULL, NULL);

    LOCK_TCPIP_CORE();
    mqtt_client = mqtt_client_new();
    UNLOCK_TCPIP_CORE();
    if (mqtt_client == NULL)
    {
        PRINTF("mqtt_client_new() failed.\r\n");
        while (1)
        {
        }
    }

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    netifapi_dhcp_start(&netif);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" MQTT client example\r\n");
    PRINTF("************************************************\r\n");

    if (sys_thread_new("app_task", app_thread, &netif, APP_THREAD_STACKSIZE, APP_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("stack_init(): Task creation failed.", 0);
    }

    vTaskDelete(NULL);
}
/*!
 * @brief FlexCAN Call Back function
 */
static FLEXCAN_CALLBACK(flexcan_callback)
{
    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
	flexcan_config_t flexcanConfig;

    SYSMPU_Type *base = SYSMPU;
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Disable SYSMPU. */
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;

    LED_RED_INIT(1);

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.baudRate               = 1000000U; se tiene que cambiar
     * flexcanConfig.baudRateFD             = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig); /*Se tiene que cambiar el baudrate*/

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

//  flexcanConfig.enableLoopBack = true; /*Quitamos el loopback para hacerlo externo*/
    flexcanConfig.baudRate = 125000U; /*Modificamos el baudrate*/

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);


    xTaskCreate(CAN_Rx_KA, "CAN_Rx_KA", configMINIMAL_STACK_SIZE, NULL, (configMAX_PRIORITIES - 2), NULL);
    xTaskCreate(Delay_Task, "Delay_Task", configMINIMAL_STACK_SIZE, NULL, (configMAX_PRIORITIES - 2), NULL);


    /* Initialize lwIP from thread */
    if (sys_thread_new("main", stack_init, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("main(): Task creation failed.", 0);
    }

    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    return 0;
}
void CAN_Rx_KA(void * args)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(20);
	xLastWakeTime = xTaskGetTickCount();
    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard; /* 11 bits*/
    mbConfig.type   = kFLEXCAN_FrameTypeData; /* Datos*/
    mbConfig.id     = FLEXCAN_ID_STD(0x100); /*Cambiamos el ID*/

    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true); /* Mailbox 9*/

    /* Start receive data through Rx Message Buffer. */
	rxKAfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM; /*Pasamos ese mailbox a un frame*/
	rxKAfer.frame = &rxKAFrame;

	for(;;) /*Monitorear que tengamos datos disponibles en el buffer de recepción*/
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		(void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxKAfer); /*Monitorea cunado existan datos*/
		if(rxComplete)
		{
			LOG_INFO("\r\nReceived message from MB%d\r\n", RX_MESSAGE_BUFFER_NUM);
			LOG_INFO("rx word0 = 0x%x\r\n", rxKAFrame.dataWord0);
			LOG_INFO("rx word1 = 0x%x\r\n", rxKAFrame.dataWord1);

			if(1 == rxKAFrame.dataByte0)
			{
				g_KA_State = 1;
				tcpip_callback(publish_message_KA, NULL);
			}
			else
			{
				battery_level[0] = rxKAFrame.dataByte6;
				battery_level[1] = rxKAFrame.dataByte5;
				battery_level[2] = rxKAFrame.dataByte4;
				battery_level[3] = '\0';
				tcpip_callback(publish_message, NULL);
			}


			rxComplete = false;
		}
	}
}

void Delay_Task(void * args)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(6000);
	xLastWakeTime = xTaskGetTickCount();


	for(;;) /*Monitorear que tengamos datos disponibles en el buffer de recepción*/
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		if(1 == g_KA_State)
		{
			LOG_INFO("rx word1 = 0x%x\r\n", rxKAFrame.dataWord1);
			g_KA_State = 0;
		}
		else
		{
			tcpip_callback(publish_message_KA_OFF, NULL);
		}
	}
}

#endif
