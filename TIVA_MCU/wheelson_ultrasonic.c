/***********************************************
 * Name: wheelson_ultrasonic.h
 * Author: Maruez Jones
 * Desc: portable abstraction layer for using 
 * 		 HC-SR04 on any microcontroller 
 * 
 * ACRONYMS:
 * WUS - wheelson ultra sonic 
 ***********************************************/
 
 #include <stdint.h>
 #include "wheelson_ultrasonic.h"
 
/******************************FUNCTIONS START***************************************/

/*************************
 * Name: delay10us
 * Desc: used for HC-SR04 
 *       specs for trigger 
 * 		 input 
 *************************/
static void delay10us(wheelson_sonic_t sensor){
	
	//software delay 
	volatile uint32_t delay = (uint32_t)((float)(WUS_10US) * (float)(sensor.mcu_clk));
	for(uint32_t i = 0; i < delay; i++){
		asm("");
	}

}

/***************************
 * Name: SonicUpdate
 * Desc: updates output variables 
 * in sonic struct 
 ***************************/
sensor_stat_t  WUS_SonicUpdate(wheelson_sonic_t *sensor){

	//echo start sequence for sensor
	sensor->gpio_write_trig(HIGH);
	sensor->delay10us();
	sensor->gpio_write_trig(LOW);
	while(sensor->gpio_read_echo() == LOW); //wait to go high
	
	//start timer 
	//uint32_t result = sensor->timer_read();
	volatile uint32_t result = 0;
	sensor->timer_start();
	
	//if timeout enable, use wait + timeout 
	// else just have a wait loop 
	if(sensor->timeout_enable){
		uint32_t count = 0;
		while(sensor->gpio_read_echo() == HIGH){
			if(count >= sensor->timeout_max){
				return SEN_NOK;
				break;
			}
			count++;
		}	
	}
	else{
		while(sensor->gpio_read_echo() == HIGH);
	}
	
	//stop timer 
	sensor->timer_stop();
	result = sensor->timer_read();
	volatile float seconds = (float)(result)/sensor->timer_freq;
	sensor->timer_rst();

	//convert to microseconds
	sensor->output_us = seconds * 1E-6; 
	
	//convert result to meters
	// the 0.5 is due to the time being the total time 
	// of sending and receiving the pulse, we only wnat the time
	// it took the pulse to come back jk
	sensor->output_meters = seconds * WUS_SOUND_VELO * 0.5; 
	
	return SEN_OK;
}

/***************************
 * Name: Get Functions 
 * Desc: returns output 
 * variables in the struct 
 * kind of arbitary but could 
 * make code look cleaner 
 ***************************/
float WUS_GetMeters(wheelson_sonic_t sensor){
	
	return sensor.output_meters;
	
}
uint32_t WUS_GetMicroSec(wheelson_sonic_t sensor){
	
	return sensor.output_us;
	
}

/******************************FUNCTIONS END*****************************************/
