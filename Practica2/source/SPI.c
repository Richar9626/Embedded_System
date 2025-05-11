/*
 * SPI.c
 *
 *  Created on: Mar 25, 2021
 *      Author: rjimenez
 *
 *
 *      This device driver is intended to cover SPI usage
 *      From initialization to transference of data
 */

#include <MK64F12.h>
#include "stdint.h"
#include "GPIO.h"
#include "SPI.h"
#include "Bits.h"



/*it enable the clock module of the SPI by modifying the MDIS bits
 * MDIS bit is bit 14 under MCR register, enabled with 0
 * SPI0,1 and 2 struct is defined on MK64F12.h*/
 void SPI_enable(spi_channel_t channel)
 {
	switch(channel)/** Selecting the SPI for enabling*/
	{
		case SPI_0: /** SPI_0 is selected*/
			SPI0->MCR &= ~(SPI_MCR_MDIS_MASK);/*SPI_MCR_MDIS_MASK macro corresponds to bit 14*/
			break;
		case SPI_1: /** SPI_1  is selected*/
			SPI1->MCR &= ~(SPI_MCR_MDIS_MASK);
			break;
		default: /** SPI_2  is selected*/
			SPI2->MCR &= ~(SPI_MCR_MDIS_MASK);
			break;
	}
	return;
 }


 /*It activates the clock gating
  * SCGC3 bit 12 is for SPI2
  * SCGC6 bit 13 is for SPI0
  * SCGC6 bit 12 is for SPI1*/
 void SPI_clk(spi_channel_t channel)
 {
	switch(channel)/** Selecting the SPI for clock enabling*/
	{
		case SPI_0: /** SPI_0 is selected*/
			SIM->SCGC6 |= SPI0_CLOCK_GATING;
			break;
		case SPI_1: /** SPI_1  is selected*/
			SIM->SCGC6 |= SPI1_CLOCK_GATING;
			break;
		default: /** SPI_2  is selected*/
			SIM->SCGC3 |= SPI2_CLOCK_GATING;
			break;
	}
	return;
 }


/*It configure the SPI as a master or slave depending on the value of masterOrslave
 * MCR register bit 31 MSTR is in charge of this mode select
 * 0 -> slave
 * 1 -> master */
 void SPI_set_master(spi_channel_t channel, spi_master_t master_or_slave)
 {
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			if (SPI_SLAVE == master_or_slave) /*Use of 2 if just to take advantage of both enum options declared by prof*/
			{
				SPI0->MCR &= ~(SPI_MCR_MSTR_MASK); /*SPI_MCR_MSTR_MASK macro equivalent to 31 bit*/
			}
			if (SPI_MASTER == master_or_slave)
			{
				SPI0->MCR |= SPI_MCR_MSTR_MASK;
			}
			break;
		case SPI_1: /** SPI_1  is selected*/
			if (SPI_SLAVE == master_or_slave) /*Use of 2 if just to take advantage of both enum options declared by prof*/
			{
				SPI1->MCR &= ~(SPI_MCR_MSTR_MASK);
			}
			if (SPI_MASTER == master_or_slave)
			{
				SPI1->MCR |= SPI_MCR_MSTR_MASK;
			}
			break;
		default: /** SPI_2  is selected*/
			if (SPI_SLAVE == master_or_slave) /*Use of 2 ifs just to take advantage of both enum options declared by prof*/
			{
				SPI2->MCR &= ~(SPI_MCR_MSTR_MASK);
			}
			if (SPI_MASTER == master_or_slave)
			{
				SPI2->MCR |= SPI_MCR_MSTR_MASK;
			}
			break;
	}
	return;
 }


/*It activates the TX and RX FIFOs of the SPI depending on the value of enableOrdisable
 *both TxF and RxF are on MCR register bit 13 and 12
 *both enabled with 0*/
 void SPI_fifo(spi_channel_t channel, spi_enable_fifo_t enable_or_disable)
 {
	 switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			if(SPI_ENABLE_FIFO == enable_or_disable)
			{
				SPI0->MCR &= ~(SPI_MCR_DIS_TXF_MASK);
				SPI0->MCR &= ~(SPI_MCR_DIS_RXF_MASK);
			}
			if(SPI_DISABLE_FIFO == enable_or_disable)
			{
				SPI0->MCR |= SPI_MCR_DIS_TXF_MASK;
				SPI0->MCR |= SPI_MCR_DIS_RXF_MASK;
			}
			break;
		case SPI_1: /** SPI_1  is selected*/
			if(SPI_ENABLE_FIFO == enable_or_disable)
			{
				SPI1->MCR &= ~(SPI_MCR_DIS_TXF_MASK);
				SPI1->MCR &= ~(SPI_MCR_DIS_RXF_MASK);
			}
			if(SPI_DISABLE_FIFO == enable_or_disable)
			{
				SPI1->MCR |= SPI_MCR_DIS_TXF_MASK;
				SPI1->MCR |= SPI_MCR_DIS_RXF_MASK;
			}
			break;
		default: /** SPI_2  is selected*/
			if(SPI_ENABLE_FIFO == enable_or_disable)
			{
				SPI2->MCR &= ~(SPI_MCR_DIS_TXF_MASK);
				SPI2->MCR &= ~(SPI_MCR_DIS_RXF_MASK);
			}
			if(SPI_DISABLE_FIFO == enable_or_disable)
			{
				SPI2->MCR |= SPI_MCR_DIS_TXF_MASK;
				SPI2->MCR |= SPI_MCR_DIS_RXF_MASK;
			}
			break;
	}
	 return;
 }


/*It selects the clock polarity depending on the value of cpol
 * Clock polarity on CTAR register bit 26, macro: SPI_CTAR_CPOL_MASK
 * 0 -> inactive state value of SCK is low
 * 1 -> inactive state value of SCK is high*/
 void SPI_clock_polarity(spi_channel_t channel, spi_polarity_t cpol)
 {
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			if (SPI_HIGH_POLARITY == cpol)
			{
				SPI0->CTAR[SPI_CTAR_0] |= SPI_CTAR_CPOL_MASK;
			}
			if (SPI_LOW_POLARITY == cpol)
			{
				SPI0->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_CPOL_MASK);
			}
			break;
		case SPI_1: /** SPI_1  is selected*/
			if (SPI_HIGH_POLARITY == cpol)
			{
				SPI1->CTAR[SPI_CTAR_0] |= SPI_CTAR_CPOL_MASK;
			}
			if (SPI_LOW_POLARITY == cpol)
			{
				SPI1->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_CPOL_MASK);
			}
			break;
		default: /** SPI_2  is selected*/
			if (SPI_HIGH_POLARITY == cpol)
			{
				SPI2->CTAR[SPI_CTAR_0] |= SPI_CTAR_CPOL_MASK;
			}
			if (SPI_LOW_POLARITY == cpol)
			{
				SPI2->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_CPOL_MASK);
			}
			break;
	}
	return;
 }


/*It selects the frame size depending on the value of frameSize and the macros defined in SPI.h
 *FMSZ on register CTAR bits 27 - 30
 *FMSZ 4 is minimum value*/
 void SPI_frame_size(spi_channel_t channel, uint32_t frame_size)
 {
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			SPI0->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_FMSZ_MASK); /*We clear the initial value*/
			SPI0->CTAR[SPI_CTAR_0] |= frame_size;
			break;
		case SPI_1: /** SPI_1  is selected*/
			SPI0->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_FMSZ_MASK);
			SPI1->CTAR[SPI_CTAR_0] |= frame_size;
			break;
		default: /** SPI_2  is selected*/
			SPI0->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_FMSZ_MASK);
			SPI2->CTAR[SPI_CTAR_0] |= frame_size;
			break;
	}
	return;
 }


/*It selects the clock phase depending on the value of cpha
 * CPHA is found in CTAR register bit 25, macro: SPI_CTAR_CPHA_MASK
 * 0 -> Data is captured on the leading edge of SCK and changed on the following edge.
 * 1 -> Data is changed on the leading edge of SCK and captured on the following edge.*/
 void SPI_clock_phase(spi_channel_t channel, spi_phase_t cpha)
 {
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			if (SPI_LOW_PHASE == cpha)
			{
				SPI0->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_CPHA_MASK);
			}
			if (SPI_HIGH_PHASE == cpha)
			{
				SPI0->CTAR[SPI_CTAR_0] |= SPI_CTAR_CPHA_MASK;
			}
			break;
		case SPI_1: /** SPI_1  is selected*/
			if (SPI_LOW_PHASE == cpha)
			{
				SPI1->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_CPHA_MASK);
			}
			if (SPI_HIGH_PHASE == cpha)
			{
				SPI1->CTAR[SPI_CTAR_0] |= SPI_CTAR_CPHA_MASK;
			}
			break;
		default: /** SPI_2  is selected*/
			if (SPI_LOW_PHASE == cpha)
			{
				SPI2->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_CPHA_MASK);
			}
			if (SPI_HIGH_PHASE == cpha)
			{
				SPI2->CTAR[SPI_CTAR_0] |= SPI_CTAR_CPHA_MASK;
			}
			break;
	}
	return;
 }


 /*It selects the baud rate depending on the value of baudRate and the macros that are defined spi.h
  * Bits 0 - 3 in CTAR register, no need for shift*/
 void SPI_baud_rate(spi_channel_t channel, uint32_t baud_rate)
 {
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			SPI0->CTAR[SPI_CTAR_0] |= baud_rate;
			break;
		case SPI_1: /** SPI_1  is selected*/
			SPI1->CTAR[SPI_CTAR_0] |= baud_rate;
			break;
		default: /** SPI_2  is selected*/
			SPI2->CTAR[SPI_CTAR_0] |= baud_rate;
			break;
	}
	return;
 }


/*It selects if MSB or LSM bits is first transmitted
 * LSBFE bit 24 in register CTAR, macro: SPI_CTAR_LSBFE_MASK
 * 0 -> Data is transferred MSB first.
 * 1 -> Data is transferred LSB first.*/
 void SPI_msb_first(spi_channel_t channel, spi_lsb_or_msb_t msb)
 {
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			if (SPI_MSB == msb)
			{
				SPI0->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_LSBFE_MASK);
			}
			if (SPI_LSM == msb)
			{
				SPI0->CTAR[SPI_CTAR_0] |= SPI_CTAR_LSBFE_MASK;
			}
			break;
		case SPI_1: /** SPI_1  is selected*/
			if (SPI_MSB == msb)
			{
				SPI1->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_LSBFE_MASK);
			}
			if (SPI_LSM == msb)
			{
				SPI1->CTAR[SPI_CTAR_0] |= SPI_CTAR_LSBFE_MASK;
			}
			break;
		default: /** SPI_2  is selected*/
			if (SPI_MSB == msb)
			{
				SPI2->CTAR[SPI_CTAR_0] &= ~(SPI_CTAR_LSBFE_MASK);
			}
			if (SPI_LSM == msb)
			{
				SPI2->CTAR[SPI_CTAR_0] |= SPI_CTAR_LSBFE_MASK;
			}
			break;
	}
	return;

 }


/*It stars the SPI transmission by modifying the value of HALT bit
 * HALT is bit 0 of MCR register, macro: SPI_MCR_HALT_MASK
 * 0 -> Start transfers.
 * 1 -> Stop transfers.*/
void SPI_start_tranference(spi_channel_t channel)
{
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			SPI0->MCR &= ~(SPI_MCR_HALT_MASK);
			break;
		case SPI_1: /** SPI_1  is selected*/
			SPI1->MCR &= ~(SPI_MCR_HALT_MASK);
			break;
		default: /** SPI_2  is selected*/
			SPI2->MCR &= ~(SPI_MCR_HALT_MASK);
			break;
	}
	return;
}


/*It stops the SPI transmission by modifying the value of HALT bit*/
void SPI_stop_tranference(spi_channel_t channel)
{
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			SPI0->MCR |= SPI_MCR_HALT_MASK;
			break;
		case SPI_1: /** SPI_1  is selected*/
			SPI1->MCR |= SPI_MCR_HALT_MASK;
			break;
		default: /** SPI_2  is selected*/
			SPI2->MCR |= SPI_MCR_HALT_MASK;
			break;
	}
	return;
}


/*It transmits the information contained in data
 * Need to write initial bits on SPIx_PUSHR to initiate transmission and clock  gen
 * need to verify the transmission checking bit TCF of SPIx_SR register
 * clean flag for completed transmission writing 1 on TCF, macro: SPI_SR_TCF_MASK*/
uint8_t SPI_tranference(spi_channel_t channel, uint8_t data)
{
	uint8_t received_data = 0x00;
	switch(channel)/** Selecting the SPI*/
	{
		case SPI_0: /** SPI_0 is selected*/
			SPI0->PUSHR = data;
			while (FALSE == (SPI0->SR & SPI_SR_TCF_MASK))
			{}
			SPI0->SR |= SPI_SR_TCF_MASK;
			received_data = SPI0->POPR;
			break;
		case SPI_1: /** SPI_1  is selected*/
			SPI1->PUSHR = data;
			while (FALSE == (SPI1->SR & SPI_SR_TCF_MASK))
			{}
			SPI1->SR |= SPI_SR_TCF_MASK;
			received_data = SPI1->POPR;
			break;
		default: /** SPI_2  is selected*/
			SPI2->PUSHR = data;
			while (FALSE == (SPI2->SR & SPI_SR_TCF_MASK))
			{}
			SPI2->SR |= SPI_SR_TCF_MASK;
			received_data = SPI2->POPR;
			break;
	}


	return (received_data);
}


/*It configures the SPI for transmission, this function as arguments receives a pointer to a constant structure where are all
 * the configuration parameters*/
void SPI_init(const spi_config_t* config_struct)
{
	SPI_clk(config_struct->spi_channel);
	SPI_set_master(config_struct->spi_channel, config_struct->spi_master);

	GPIO_clock_gating(config_struct->spi_gpio_port.gpio_port_name);

	GPIO_pin_control_register(config_struct->spi_gpio_port.gpio_port_name, config_struct->spi_gpio_port.spi_clk,  &(config_struct->pin_config));
	GPIO_pin_control_register(config_struct->spi_gpio_port.gpio_port_name, config_struct->spi_gpio_port.spi_sout, &(config_struct->pin_config));
	GPIO_pin_control_register(config_struct->spi_gpio_port.gpio_port_name, config_struct->spi_gpio_port.spi_sin,  &(config_struct->pin_config));

	SPI_frame_size(config_struct->spi_channel, config_struct->spi_frame_size);
	SPI_baud_rate(config_struct->spi_channel, config_struct->spi_baudrate);
	SPI_msb_first(config_struct->spi_channel, config_struct->spi_lsb_or_msb);
	SPI_clock_polarity(config_struct->spi_channel, config_struct->spi_polarity);
	SPI_clock_phase(config_struct->spi_channel, config_struct->spi_phase);
	SPI_fifo(config_struct->spi_channel, config_struct->spi_enable_fifo);

	SPI_enable(config_struct->spi_channel);
	return;
}
