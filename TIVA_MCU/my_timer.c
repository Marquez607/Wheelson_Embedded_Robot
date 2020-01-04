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

#include <stdbool.h>
#include <stdint.h>
#include "my_timer.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"

/************************
 * Name: Init
 * Desc: initializes delay
 * timer
 ************************/
void TIM_Init(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, TIM_PRESCALAR);
    TimerLoadSet(TIMER0_BASE, TIMER_A,0xFFFFFFFF);

}

/************************
 * Name: Start
 * Desc:
 ************************/
void TIM_Start(void){

    TimerEnable(TIMER0_BASE, TIMER_A);

}

/************************
 * Name: Stop
 * Desc:
 ************************/
void TIM_Stop(void){

    TimerDisable(TIMER0_BASE, TIMER_A);

}

/************************
 * Name: Read
 * Desc:
 ************************/
uint32_t TIM_Read(void){

    return TimerValueGet(TIMER0_BASE,TIMER_A);

}

/************************
 * Name: Reset
 * Desc:
 ************************/
void TIM_Reset(void){

   HWREG(TIMER0_BASE + TIMER_O_TAV) = 0;
   HWREG(TIMER0_BASE + TIMER_O_TBV) = 0;
}



