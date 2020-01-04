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

//MY INCLUDES
#include "my_pwm.h"

/*********************FUNCTION START******************************/
/***************************************************
 * Name: Duty95Check
 * Desc: The Tiva doesn't allow 100% duty cycle;
 *       it'll result in the pwm dropping to low
 *       this function caps it at 95%
 **************************************************/
static float PWM_Duty95Check(float duty){

    if(duty >= 0.95){
        return 0.95;
    }else{return duty;}

}

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
void PWM_Init(void){
    //PWM clock enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    //Pin muxing
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinConfigure(GPIO_PC4_M0PWM6);
    GPIOPinConfigure(GPIO_PC5_M0PWM7);

    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    //PWM MOD 0 CONFIG
    PWMGenConfigure(PWM0_BASE,
                    PWM_GEN_3,
                    PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, PWM_PERIOD());

    //PWM MOD 1 CONFIG
    PWMGenConfigure(PWM1_BASE,
                    PWM_GEN_3,
                    PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, PWM_PERIOD());

    //generator enable
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    //output enable
    PWMOutputState(PWM0_BASE, 0xFF,true);
    PWMOutputState(PWM1_BASE, 0xFF,true);

    PWM_SetDuty_PF2(0);
    PWM_SetDuty_PF3(0);
    PWM_SetDuty_PC4(0);
    PWM_SetDuty_PC5(0);
}

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
void PWM_SetDuty_PF2(float duty){

    duty = PWM_Duty95Check(duty);
    PWMPulseWidthSet(PWM1_BASE,
                     PWM_OUT_6,
                     duty * PWMGenPeriodGet(PWM1_BASE,PWM_GEN_3));
}
void PWM_SetDuty_PF3(float duty){
    duty = PWM_Duty95Check(duty);
    PWMPulseWidthSet(PWM1_BASE,
                     PWM_OUT_7,
                     duty * PWMGenPeriodGet(PWM1_BASE,PWM_GEN_3));

}
void PWM_SetDuty_PC4(float duty){
    duty = PWM_Duty95Check(duty);
    PWMPulseWidthSet(PWM0_BASE,
                     PWM_OUT_6,
                     duty * PWMGenPeriodGet(PWM0_BASE,PWM_GEN_3));

}
void PWM_SetDuty_PC5(float duty){
    duty = PWM_Duty95Check(duty);
    PWMPulseWidthSet(PWM0_BASE,
                     PWM_OUT_7,
                     duty * PWMGenPeriodGet(PWM0_BASE,PWM_GEN_3));

}

/*********************FUNCTION END********************************/



