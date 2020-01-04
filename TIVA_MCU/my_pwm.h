/*************************************************
 * Name: pwm
 * Author: Marquez Jones
 * Desc: pwm use and init functions
 *
 * PWM:
 * Left Motor:
 * PF2 - M1PWM6 - POS
 * PF3 - M1PWM7 - NEG
 *
 * Right Motor:
 * PC4 - M0PWM6 - POS
 * PC5 - M0PWM7 - NEG
 *
 *************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/pwm.h"

#ifndef MY_PWM_H_
#define MY_PWM_H_

/*********************DEFINES START******************************/

#define PWM_PERIOD_US 2000 //2000 microsecond period
#define PWM_PERIOD_SEC 0.02 //2 ms
#define PWM_PERIOD() PWM_PERIOD_SEC * (SysCtlClockGet())

/*********************DEFINES END********************************/

/*********************FUNCTION START******************************/

/*******************************************************
 * Name: PWM_Init
 * Desc: intialize pwm channels for project
 *
 * Left Motor:
 * PF2 - M1PWM6 - POS
 * PF3 - M1PWM7 - NEG
 *
 * Right Motor:
 * PC4 - M0PWM6 - POS
 * PC5 - M0PWM7 - NEG
 *******************************************************/
void PWM_Init(void);

/*******************************************************
 * Name: PWM_SetDuty
 * Desc: sets duty for specified channel
 *
 * Left Motor:
 * PF2 - M1PWM6 - POS
 * PF3 - M1PWM7 - NEG
 *
 * Right Motor:
 * PC4 - M0PWM6 - POS
 * PC5 - M0PWM7 - NEG
 *******************************************************/
void PWM_SetDuty_PF2(float duty);
void PWM_SetDuty_PF3(float duty);
void PWM_SetDuty_PC4(float duty);
void PWM_SetDuty_PC5(float duty);

/*********************FUNCTION END********************************/

#endif /* PWM_H_ */
