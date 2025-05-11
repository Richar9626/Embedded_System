/**
 * @file rtos.c
 * @author ITESO
 * @date Feb 2018
 * @brief Implementation of rtos API
 *
 * This is the implementation of the rtos module for the
 * embedded systems II course at ITESO
 */

#include "rtos.h"
#include "rtos_config.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#ifdef RTOS_ENABLE_IS_ALIVE
#include "fsl_gpio.h"
#include "fsl_port.h"
#endif
/**********************************************************************************/
// Module defines
/**********************************************************************************/

#define FORCE_INLINE 	__attribute__((always_inline)) inline

#define STACK_FRAME_SIZE			8
#define STACK_LR_OFFSET				2
#define STACK_PSR_OFFSET			1
#define STACK_PSR_DEFAULT			0x01000000
#define MIN_PRIORITY		     	4 /* We set 4 as lowest priority*/

/**********************************************************************************/
// IS ALIVE definitions
/**********************************************************************************/

#ifdef RTOS_ENABLE_IS_ALIVE
#define CAT_STRING(x,y)  		x##y
#define alive_GPIO(x)			CAT_STRING(GPIO,x)
#define alive_PORT(x)			CAT_STRING(PORT,x)
#define alive_CLOCK(x)			CAT_STRING(kCLOCK_Port,x)
static void init_is_alive(void);
static void refresh_is_alive(void);
#endif

/**********************************************************************************/
// Type definitions
/**********************************************************************************/

typedef enum
{
	S_READY = 0, S_RUNNING, S_WAITING, S_SUSPENDED
} task_state_e;
typedef enum
{
	kFromISR = 0, kFromNormalExec
} task_switch_type_e;

typedef struct
{
	uint8_t priority;
	task_state_e state;
	uint32_t *sp;
	void (*task_body)();
	rtos_tick_t local_tick;
	uint32_t reserved[10];
	uint32_t stack[RTOS_STACK_SIZE];
} rtos_tcb_t;

/**********************************************************************************/
// Global (static) task list
/**********************************************************************************/

struct
{
	uint8_t nTasks;
	rtos_task_handle_t current_task;
	rtos_task_handle_t next_task;
	rtos_tcb_t tasks[RTOS_MAX_NUMBER_OF_TASKS + 1];
	rtos_tick_t global_tick;
} task_list =
{ 0 };

/**********************************************************************************/
// Local methods prototypes
/**********************************************************************************/

static void reload_systick(void);
static void dispatcher(task_switch_type_e type);
static void activate_waiting_tasks();
FORCE_INLINE static void context_switch(task_switch_type_e type);
static void idle_task(void);
static rtos_task_handle_t idle;

/**********************************************************************************/
// API implementation
/**********************************************************************************/

/* We start systick and create idle task just as regular FreeRTOS */
void rtos_start_scheduler(void)
{
#ifdef RTOS_ENABLE_IS_ALIVE
	init_is_alive();
#endif
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk| SysTick_CTRL_ENABLE_Msk; /* Sets systick control*/
	reload_systick();
	task_list.global_tick = 0;
	task_list.current_task = -1; /* current task starts on -1 so priority sets it*/

	idle = rtos_create_task(idle_task, MIN_PRIORITY, kAutoStart); /* we create idle task*/

	for (;;);
}

/* For task creation we use nTasks variant and define sp and stack as before*/
rtos_task_handle_t rtos_create_task(void (*task_body)(), uint8_t priority,
		rtos_autostart_e autostart)
{
	/* Condition to have a max of 10 tasks including the idle task
	 * we can have 9 regular tasks top and the || is for the idle
	 * this following the correct order that is creating tasks and then rtos_start*/
	if(task_list.nTasks < RTOS_MAX_NUMBER_OF_TASKS-1 || task_body == idle_task)
	{
		if(kAutoStart == autostart) /*we determine if it will start running state or suspend*/
		{
			task_list.tasks[task_list.current_task].state = S_READY;
		}
		else
		{
			task_list.tasks[task_list.current_task].state = S_SUSPENDED;
		}
		task_list.tasks[task_list.nTasks].task_body = task_body;
		task_list.tasks[task_list.nTasks].sp = &(task_list.tasks[task_list.nTasks].stack[RTOS_STACK_SIZE - 1]) - STACK_FRAME_SIZE;
		task_list.tasks[task_list.nTasks].stack[RTOS_STACK_SIZE - STACK_LR_OFFSET] = (uint32_t) task_body;
		task_list.tasks[task_list.nTasks].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] = STACK_PSR_DEFAULT;
		task_list.tasks[task_list.nTasks].priority = priority;
		task_list.tasks[task_list.current_task].local_tick = 0;

		task_list.nTasks ++; /* Task index incremented for next task*/

		return(task_list.nTasks - 1); /* we substract what we added before*/
	}
	else
	{
		PRINTF("Task limit\n");
		return -1; /* This means an error occured, no other action was specified*/
	}

}

rtos_tick_t rtos_get_clock(void)
{
	return task_list.global_tick;
}

/* Function to reset tasks delay*/
void rtos_delay(rtos_tick_t ticks)
{
	task_list.tasks[task_list.current_task].state = S_WAITING;
	task_list.tasks[task_list.current_task].local_tick = ticks;
	dispatcher(kFromNormalExec);
}

void rtos_suspend_task(void)
{
	task_list.tasks[task_list.current_task].state = S_SUSPENDED;
	dispatcher(kFromNormalExec);
}

void rtos_activate_task(rtos_task_handle_t task)
{
	task_list.tasks[task_list.current_task].state = S_READY;
	dispatcher(kFromNormalExec);
}

/**********************************************************************************/
// Local methods implementation
/**********************************************************************************/

static void reload_systick(void)
{
	SysTick->LOAD = USEC_TO_COUNT(RTOS_TIC_PERIOD_IN_US, CLOCK_GetCoreSysClkFreq());
	SysTick->VAL = 0;
}

/* Function acting as scheduler, determines priority and whos next task*/
static void dispatcher(task_switch_type_e type)
{
	 uint8_t next_task_found = 0;

	 for(uint8_t priority = 0;priority <= MIN_PRIORITY && !next_task_found;priority++)
	 {
		 for(uint8_t task_index = 0;task_index< task_list.nTasks  && !next_task_found;task_index++)
		 {
			  if(task_list.tasks[task_index].priority == priority && task_list.tasks[task_index].state != S_WAITING)
			  {
				  task_list.next_task = task_index;
				  next_task_found = 1; /* enable flag to exit for*/
			  }
		  }
	   }

	 if (task_list.current_task!=task_list.next_task)
	 {
			 context_switch(type); /* enter here if we need to change context*/
	 }
}

/*function in charge of loading registers into sp and adjust offset*/
FORCE_INLINE static void context_switch(task_switch_type_e type)
{
	static uint8_t first = 1;
	register uint32_t r0 asm("r0");
	(void) r0;

	if(!first)
	{
		asm("mov r0, r7");
		task_list.tasks[task_list.current_task].sp = (uint32_t*) r0;

		/* There is a different offset between context changes
		 * determined by checking on task stack before loading it*/
		if(kFromISR == type)
		{
			task_list.tasks[task_list.current_task].sp -= (-9);
		}
		else
		{
			task_list.tasks[task_list.current_task].sp -= (9);
		}
	}
	else
	{
		first = 0;
	}
	 task_list.current_task = task_list.next_task;
	 task_list.tasks[task_list.current_task].state = S_RUNNING;
	 SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Set PendSV to pending
}

/*Function were ticks are decreased and is set to ready*/
static void activate_waiting_tasks()
{
	uint8_t task_index;

	for (task_index = 0; task_index <task_list.nTasks; task_index++)
	{
		if(S_WAITING == task_list.tasks[task_index].state)
		{
			task_list.tasks[task_index].local_tick--;
			if(0 == task_list.tasks[task_index].local_tick)
			{
				task_list.tasks[task_index].state = S_READY;
			}
		}
	}
}

/**********************************************************************************/
// IDLE TASK
/**********************************************************************************/

static void idle_task(void)
{
	for (;;)
	{

	}
}

/****************************************************/
// ISR implementation
/****************************************************/

void SysTick_Handler(void)
{
#ifdef RTOS_ENABLE_IS_ALIVE
	refresh_is_alive();
#endif

	task_list.global_tick++;
	activate_waiting_tasks();
	reload_systick();
	dispatcher(kFromISR);
}

void PendSV_Handler(void)
{
	register int32_t r0 asm("r0");
	(void) r0;
	SCB->ICSR |= SCB_ICSR_PENDSVCLR_Msk;
	r0 = (int32_t) task_list.tasks[task_list.current_task].sp;
	asm("mov r7,r0");
}

/**********************************************************************************/
// IS ALIVE SIGNAL IMPLEMENTATION
/**********************************************************************************/

#ifdef RTOS_ENABLE_IS_ALIVE
static void init_is_alive(void)
{
	gpio_pin_config_t gpio_config =
	{ kGPIO_DigitalOutput, 1, };

	port_pin_config_t port_config =
	{ kPORT_PullDisable, kPORT_FastSlewRate, kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
	        kPORT_UnlockRegister, };
	CLOCK_EnableClock(alive_CLOCK(RTOS_IS_ALIVE_PORT));
	PORT_SetPinConfig(alive_PORT(RTOS_IS_ALIVE_PORT), RTOS_IS_ALIVE_PIN,
	        &port_config);
	GPIO_PinInit(alive_GPIO(RTOS_IS_ALIVE_PORT), RTOS_IS_ALIVE_PIN,
	        &gpio_config);
}

static void refresh_is_alive(void)
{
	static uint8_t state = 0;
	static uint32_t count = 0;
	SysTick->LOAD = USEC_TO_COUNT(RTOS_TIC_PERIOD_IN_US,
	        CLOCK_GetCoreSysClkFreq());
	SysTick->VAL = 0;
	if (RTOS_IS_ALIVE_PERIOD_IN_US / RTOS_TIC_PERIOD_IN_US - 1 == count)
	{
		GPIO_PinWrite(alive_GPIO(RTOS_IS_ALIVE_PORT), RTOS_IS_ALIVE_PIN,
		        state);
		state = state == 0 ? 1 : 0;
		count = 0;
	} else //
	{
		count++;
	}
}
#endif
///
