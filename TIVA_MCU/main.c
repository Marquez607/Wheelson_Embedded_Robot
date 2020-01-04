/*************************************************
 * Name: main
 * Author: Marquez Jones
 * Desc: main program for wheelson robots
 *
 *
 * Importan Hardware Connections
 *
 * UART2 (used for bluetooth)
 * PD6 - RX
 * PD7 - TX
 *mn,
 * PWM:
 * Left Motor:
 * PF2 - M1PWM6
 * PF3 - M1PWM7
 *
 * Right Motor:
 * PC4 - M0PWM6
 * PC5 - M0PWM7
 *
 * ULTRSONIC 1:
 * PD0- TRIG
 * PD1- ECHO
 *
 * MARK 1 UART COMMANDS
 * 'W' go forward
 * 'S' go reverse
 * 'A' rotate left
 * 'D' rotate right
 * 'E' stop
 *
 ************************************************/

//includes
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

//MIL includes
#include "MIL_CLK.h"
#include "MIL_UART.h"

//my includes
#include "my_pwm.h"
#include "my_timer.h"

//wheelson includes
#include "wheelson_motors.h"
#include "wheelson_ultrasonic.h"

/*********************************************DEFINES START*******************************/

#define MCU_CLK_FREQ 16000000

//ultrasonic IO defines
#define USON_TRIG_PIN GPIO_PIN_1
#define USON_ECHO_PIN GPIO_PIN_0
#define USON_IO_BASE GPIO_PORTD_BASE

//This board uses literally every port on the MCU
#define INIT_ALL_PORT_CLKS() do{ \
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); \
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); \
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); \
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); \
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); \
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); \
        }while(0)

/*********************************************DEFINES END*********************************/

/*********************************************DEFINES START*******************************/

#define STOP_CMD 'E'
#define AUTO_CMD 'Q'
#define FWD_CMD 'W'
#define REV_CMD 'S'
#define LEFT_CMD 'A'
#define RIGHT_CMD 'D'


/*********************************************DEFINES END*********************************/

/*********************************************GLOBAL VARIABLES START*******************************/

//Will be used to echo back received data
uint8_t rx_flag = 0;
uint8_t rx_data = 0;

wheelson_motor2_t motors;
wheelson_sonic_t ultrasonic;

/*********************************************GLOBAL VARIABLES END*********************************/

/*********************************************FUNCTION PROTOTYPES START**********************************/

//Desc: arbitrary software delay
void ArbSoftDelay(void);

/******************************************
 * Name: Init Trig/Echo
 * Desc: intitailizes trigger and echo
 *       IO for ultrosonic sensor
 * ULTRSONIC 1:
 * PD0- TRIG (output)
 * PD1- ECHO (input)
 *
 ******************************************/
void Init_TrigEcho(void);

/******************************************
 * Name: Read/Write functions
 * Desc: used to comply with wheelson
 *       struct needs
 *
 *       write trigger pin
 *       read echo pin
 *****************************************/
void Trig_Write(gpio_state_t state);
gpio_state_t Echo_Read(void);


/*******************************
 * Name: Delay functions
 * Desc: software delays
 *******************************/
void Delay10us(void);
//delay specified amount of seconds
void DelaySeconds(float seconds);

/*********************************************FUNCTION PROTOTYPES END************************************/


/*************************************ISR PROTOTYPES START***********************************************/

//interrupt configured for RX interrupt
void UART_RX_Handler(void);

/*************************************ISR PROTOTYPES END*************************************************/

/*********************************************MAIN START*************************************************/

int main(void){

    //clock init
    MIL_ClkSetInt_16MHz();

    INIT_ALL_PORT_CLKS();

    //uart2 init
    MIL_InitUART(UART2_BASE,MIL_BAUD_9600);
    MIL_UART_InitISR(UART2_BASE,MIL_RX_INT_EN,&UART_RX_Handler);

    //pwm init
    PWM_Init();

    //io init for ultra sonic
    Init_TrigEcho();

    //timer init for ultra sonic
    TIM_Init();

    /**************MOTOR INIT*******************/
    motors.left_weight  = WM_NO_WEIGHT;
    motors.right_weight = WM_NO_WEIGHT;
    motors.left_bias    = WM_NO_BIAS;
    motors.right_bias   = WM_NO_BIAS;
    motors.speed_right_pos = &PWM_SetDuty_PC4;
    motors.speed_right_neg = &PWM_SetDuty_PC5;
    motors.speed_left_pos  = &PWM_SetDuty_PF2;
    motors.speed_left_neg  = &PWM_SetDuty_PF3;

    /**************ULTRA SONIC INIT*******************/
    //variables
    ultrasonic.timer_freq = SysCtlClockGet();
    ultrasonic.mcu_clk = SysCtlClockGet();
    ultrasonic.timeout_enable = true; // 3 ms timeout = 16Mhz * 1s/340m * 1m
    ultrasonic.timeout_max =  SysCtlClockGet() * 0.003; // 3 ms timeout = 16Mhz * 1s/340m * 1m
    ultrasonic.delay10us = &Delay10us;

    //gpio functions
    ultrasonic.gpio_write_trig = &Trig_Write;
    ultrasonic.gpio_read_echo = &Echo_Read;

    //timer functions
    ultrasonic.timer_start = &TIM_Start;
    ultrasonic.timer_stop = &TIM_Stop;
    ultrasonic.timer_rst = &TIM_Reset;
    ultrasonic.timer_read = &TIM_Read;


    const float speed = 0.95; //motor speed
    volatile float dist = 0; //distance in front(m) of robot
    volatile sensor_stat_t sen_stat; //status of sonic
    volatile uint32_t uart_count = 0; //I didn't feel like using a timer
    volatile uint8_t auto_flag = 1;
    char uart_buffer[80];
    while(1){

        //get front distance
        sen_stat = WUS_SonicUpdate(&ultrasonic);
        dist = ultrasonic.output_meters;
        Delay10us();

        /**********************************
        typedef enum{

            FORWARD = 'W',
            REVERSE ='S',
            ROT_RIGHT = 'D',
            ROT_LEFT = 'A',
            STOP = 'E'

        }cmd_t;
        ***********************************/

        //autonomouse feature mode
        if(auto_flag){
            if(dist < 0.1){
                WM_Stop(motors);
            }
            else{
                switch(rx_data){
                    //forword
                    case 'W':
                        WM_Forward2(speed,speed,motors);
                        break;
                    case 'S':
                        WM_Reverse2(speed,speed,motors);
                        break;
                    case 'D':
                        WM_TurnRight2(speed,speed,motors);
                        break;
                    case 'A':
                        WM_TurnLeft2(speed,speed,motors);
                        break;
                    case 'E':
                        WM_Stop(motors);
                        break;
                    case 'Q':
                        auto_flag ^= 1;
                        break;
                    default:
                        WM_Stop(motors);
                        break;
                }
            }
        }
        //without autonmous features
        else{
            switch(rx_data){
                //forword
                case 'W':
                    WM_Forward2(speed,speed,motors);
                    break;
                case 'S':
                    WM_Reverse2(speed,speed,motors);
                    break;
                case 'D':
                    WM_TurnRight2(speed,speed,motors);
                    break;
                case 'A':
                    WM_TurnLeft2(speed,speed,motors);
                    break;
                case 'E':
                    WM_Stop(motors);
                    break;
                case 'Q':
                    auto_flag ^= 1;
                    break;
                default:
                    WM_Stop(motors);
                    break;
            }

        }

    }

}

/*********************************************MAIN END***************************************************/

/*********************************************FUNCTION DEFINITIONS START**********************************/

//Desc: arbitrary software delay
void ArbSoftDelay(void){

    for(uint32_t idx = 0; idx < 60000;idx++);
}

/******************************************
 * Name: Init Trig/Echo
 * Desc: intitailizes trigger and echo
 *       IO for ultrosonic sensor
 * ULTRSONIC 1:
 * PD0- TRIG (output)
 * PD1- ECHO (input)
 *
 ******************************************/
void Init_TrigEcho(void){
    GPIOPinTypeGPIOInput(USON_IO_BASE,USON_ECHO_PIN);
    GPIOPinTypeGPIOOutput(USON_IO_BASE,USON_TRIG_PIN);
}

/******************************************
 * Name: Read/Write functions
 * Desc: used to comply with wheelson
 *       struct needs
 *
 *       write trigger pin
 *       read echo pin
 *****************************************/
void Trig_Write(gpio_state_t state){

    uint8_t bit_mask = USON_TRIG_PIN;
    if(state == LOW){
        bit_mask = 0x00;
    }
    GPIOPinWrite(USON_IO_BASE,USON_TRIG_PIN,bit_mask);

}
gpio_state_t Echo_Read(void){

    if(GPIOPinRead(USON_IO_BASE,USON_ECHO_PIN) & USON_ECHO_PIN){
        return HIGH;
    }
    else{
        return LOW;
    }

}


/*******************************
 * Name: Delay functions
 * Desc: software delays
 *******************************/
void Delay10us(void){

    //don't question this, it's a 10 us delay
    SysCtlDelay(SysCtlClockGet() * WUS_10US/7);

}
void DelaySeconds(float seconds){

    SysCtlDelay(SysCtlClockGet() * seconds);

}



/*********************************************FUNCTION DEFINITIONS END************************************/

/****************************************************ISR START********************************************/
void UART_RX_Handler(void){

    rx_data = (UARTCharGet(UART2_BASE));

    rx_flag = 1;  // now echo

}

/****************************************************ISR END**********************************************/


