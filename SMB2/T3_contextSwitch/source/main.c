#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

#define RTOS_STACK_SIZE 500

#define RTOS_MAX_NUMBER_OF_TASKS 3

#define STACK_FRAME_SIZE 8

#define STACK_PC_OFFSET 2

#define STACK_PSR_OFFSET 1

#define STACK_PSR_DEFAULT 0x01000000

//Enum to select on task priority
typedef enum
{
	Ready,
	Running,
	Waiting
}Task_state_t;

//enum to verify were the change of context comes from
typedef enum
{
	execution,
	isr
}Context_switch_source_t;

typedef struct
{
  uint32_t *sp;
  void (*task_body)();
  uint32_t stack[RTOS_STACK_SIZE];
  uint64_t local_tick;
  Task_state_t state;
  uint8_t priority;
  uint64_t ticks;
} task_t;

struct
{
  int8_t current_task;
  int8_t next_task;
  task_t tasks[RTOS_MAX_NUMBER_OF_TASKS + 1];
  uint64_t global_tick;
} task_list = { 0 };

void task0(void); // LED Red
void task1(void); // LED Green
void task2(void); // LED Blue
void idletask(void);

void rtosStart(void);
void rtosReloadSysTick(void);

void PendSV_Handler(void);
void SysTick_Handler(void); // 1KHz

void delay_set(uint64_t ticks);
void scheduler(Context_switch_source_t source);
void delay_check(void);

int main(void)
{
  gpio_pin_config_t led_config =
  {
    kGPIO_DigitalOutput,
	1
  };

  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
  /* Init FSL debug console. */
  BOARD_InitDebugConsole();
#endif
  CLOCK_EnableClock(kCLOCK_PortC);
  PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, &led_config);

  CLOCK_EnableClock(kCLOCK_PortE);
  PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &led_config);

  CLOCK_EnableClock(kCLOCK_PortA);
  PORT_SetPinMux(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, &led_config);

  task_list.tasks[0].task_body = task0;
  task_list.tasks[0].sp = &(task_list.tasks[0].stack[RTOS_STACK_SIZE - 1]) - STACK_FRAME_SIZE;
  task_list.tasks[0].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] = (uint32_t) task0;
  task_list.tasks[0].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] = STACK_PSR_DEFAULT;
  task_list.tasks[0].state = Ready;
  task_list.tasks[0].priority = 0;
  task_list.tasks[0].ticks = 10;

  task_list.tasks[1].task_body = task1;
  task_list.tasks[1].sp = &(task_list.tasks[1].stack[RTOS_STACK_SIZE - 1]) - STACK_FRAME_SIZE;
  task_list.tasks[1].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] = (uint32_t) task1;
  task_list.tasks[1].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] = STACK_PSR_DEFAULT;
  task_list.tasks[1].state = Ready;
  task_list.tasks[1].priority = 2;
  task_list.tasks[1].ticks = 30;

  task_list.tasks[2].task_body = task2;
  task_list.tasks[2].sp = &(task_list.tasks[2].stack[RTOS_STACK_SIZE - 1]) - STACK_FRAME_SIZE;
  task_list.tasks[2].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] = (uint32_t) task2;
  task_list.tasks[2].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] = STACK_PSR_DEFAULT;
  task_list.tasks[2].state = Ready;
  task_list.tasks[2].priority = 1;
  task_list.tasks[2].ticks = 20;

  task_list.tasks[3].task_body = idletask;
  task_list.tasks[3].sp = &(task_list.tasks[3].stack[RTOS_STACK_SIZE - 1]) - STACK_FRAME_SIZE;
  task_list.tasks[3].stack[RTOS_STACK_SIZE - STACK_PC_OFFSET] = (uint32_t) idletask;
  task_list.tasks[3].stack[RTOS_STACK_SIZE - STACK_PSR_OFFSET] = STACK_PSR_DEFAULT;
  task_list.tasks[3].state = Ready;
  task_list.tasks[3].priority = 4;

  NVIC_SetPriority(PendSV_IRQn, 0xFF);

  PRINTF("RTOS Init\n");

  while(1)
  {
	rtosStart();

    __asm volatile ("nop");
  }

  return 0 ;
}


void rtosReloadSysTick(void)
{
	SysTick->LOAD = (SystemCoreClock / 1e3);
	SysTick->VAL = 0;
}

void PendSV_Handler(void) // Context switching code
{
	register int32_t r0 asm("r0");
	(void) r0;
	SCB->ICSR |= SCB_ICSR_PENDSVCLR_Msk;
	r0 = (int32_t) task_list.tasks[task_list.current_task].sp;
	asm("mov r7,r0");
}

void SysTick_Handler(void) // 1KHz
{

  // Increment systick counter for LED blinking
  task_list.global_tick++;

  delay_check();

  rtosReloadSysTick();

  scheduler(isr); //we go check what is next
}

//Function to check how many systicks are left for task and change it to ready once its done
void delay_check(void)
{
	uint8_t task_index;

	for (task_index = 0; task_index <RTOS_MAX_NUMBER_OF_TASKS;task_index++)
	{
		if(Waiting == task_list.tasks[task_index].state)
		{
			task_list.tasks[task_index].local_tick--;
			if(0 == task_list.tasks[task_index].local_tick)
			{
				task_list.tasks[task_index].state = Ready;
			}
		}
	}
}

void task0(void) // LED Red
{
  while (1)
  {
    GPIO_PortClear(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);

    delay_set(10); //we restart delay
  }
}

void task1(void) // LED Green
{
  while (1)
  {
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);

    delay_set(30);
  }
}

void task2(void) // LED Blue
{
  while (1)
  {
    GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

    delay_set(20);
  }
}

void idletask(void) //infinite loop
{
	while(1);
}

void rtosStart(void)
{
  task_list.global_tick = 0;
  task_list.current_task = -1;

  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  rtosReloadSysTick();
  for (;;);
}

//function to change to waiting state and restart counter of systicks
void delay_set(uint64_t ticks)
{
	task_list.tasks[task_list.current_task].state = Waiting;
	task_list.tasks[task_list.current_task].local_tick = ticks;
	scheduler(execution);
}

//Function were magic happens
void scheduler(Context_switch_source_t source)
{
  static uint8_t first = 1;
  //uint8_t next_task = 0;
  uint8_t next_task_found = 0;
  register uint32_t r0 asm("r0");
  (void) r0;

//we go through priority first to see which one goes first
  for(uint8_t priority = 0;priority < 5 && !next_task_found;priority++)
  {
	  for(uint8_t task_index = 0;task_index< 4 && !next_task_found;task_index++)
	  {
		  if(task_list.tasks[task_index].priority == priority && task_list.tasks[task_index].state != Waiting) //oncer we find the next one we change it
		  {
			  //next_task = task_index;
			  task_list.next_task = task_index;
			  next_task_found = 1;
		  }
	  }
  }

  if (task_list.current_task!=task_list.next_task)
  { // Context switching needed
	if (!first)
	{
	  asm("mov r0, r7");
	  task_list.tasks[task_list.current_task].sp = (uint32_t*) r0;


	  if(source)
	  {
		  task_list.tasks[task_list.current_task].sp -= (-7); //values observed by looking into task stack and see offset
	  }
	  else
	  {
		  task_list.tasks[task_list.current_task].sp -= (9); //different offset when it just execute
	  }


	}
	else
	{
	  first = 0;
	}

	task_list.current_task = task_list.next_task;
	task_list.tasks[task_list.current_task].state = Running;
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Set PendSV to pending
  }
}
