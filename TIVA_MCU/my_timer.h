/******************************
 * Name: my_timer
 * Date: 12/23/2019
 * Desc: timer functions for wheelson
 *       project
 ******************************/

/********************************************************************
 * TO COMPLY WITH
 *
 *  void (*timer_start)(void);    //function to start timer count
 *  void (*timer_stop)(void);     //function to stop
 *  void (*timer_rst)(void);      //function to reset count to 0
 *  uint32_t (*timer_read)(void); //read timer count register
 *
 ********************************************************************/

#include <stdint.h>

#ifndef MY_TIMER_H_
#define MY_TIMER_H_

#define TIM_PRESCALAR 0xFF

/**********************WHEELSON TIMER START*****************************/

/************************
 * Name: Init
 * Desc: initializes delay
 * timer
 ************************/
void TIM_Init(void);

/************************
 * Name: Start
 * Desc:
 ************************/
void TIM_Start(void);

/************************
 * Name: Stop
 * Desc:
 ************************/
void TIM_Stop(void);

/************************
 * Name: Read
 * Desc:
 ************************/
uint32_t TIM_Read(void);

/************************
 * Name: Reset
 * Desc:
 ************************/
void TIM_Reset(void);

/**********************WHEELSON TIMER START*****************************/


#endif /* MY_TIMER_H_ */
