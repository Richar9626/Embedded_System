/**
	\file
	\brief
		This is a software module that can generate delays using ASM instructions.
		The generated delay is blocking.
	\date	26/11/2019
	\todo

 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

/** It tells the function to generate a delay in mili-seconds*/
#define DELAY_mS (0U)
/** It tells the function to generate a delay in micro-seconds*/
#define DELAY_uS (1U)
/** Constant use to calculate delay in mili-seconds*/
#define m_SECONDS_CONST (1000.0F)
/** Constant use to calculate delay in micro-seconds*/
#define u_SECONDS_CONST (1000000.0F)


/*!
 	 \brief	 This function generates a delay based in the received arguments.
 	 Internally calculate the number of clock cycle needed to spend in order to generate the
 	 delay specified by the argument time and ms_or_us

 	 \param[in] time: timer to delayed the CPU, It is an integer number.
 	 \param[in] system_clock: Clock frequency at what the CPU is running.
 	 \param[in] ms_or_us: If the argument is 1, the delay will be in microseconds, if the argument is 0 the delay will be in miliseconds
 	 \return void
 	 \todo Nothing.
 */
void Delay_ms_or_us(uint16_t time, uint32_t system_clock, uint8_t ms_or_us);


/*!
 	 \brief	 This function receives the numbers of clock cycles the the CPU will be delayed.

 	 \param[in]  Number of clock cycles.
 	 \return void
 	 \todo Nothing.
 */
void Delay_cycles(uint32_t cycles);


#endif /* DELAY_H_ */
