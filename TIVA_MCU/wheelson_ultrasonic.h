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
 #include <stdbool.h>

#ifndef WHEELSON_ULTRASONIC_H_
#define WHEELSON_ULTRASONIC_H_

/******************************DEFINE START***************************************/

#define WUS_SOUND_VELO 340 //m/s speed of sound 
#define WUS_10US 10/1000000 // 10 microseconds

/******************************DEFINE END*****************************************/

/******************************ENUM START***************************************/

/***********************************
 * Name: gpio_state_t 
 * Desc: for sake of standardizing
 * gpio functions across mcus
 ***********************************/
typedef enum{
	HIGH = 1,
	LOW  = 0	
}gpio_state_t;

/***********************************
 * Name: sensor_stat_t
 * Desc: used to indicate function 
 * failure 
 * OK = good 
 * NOK = not ok (function timed out)
 ***********************************/
typedef enum{
	SEN_OK,
	SEN_NOK
}sensor_stat_t;

/******************************ENUM END*****************************************/

/******************************STRUCTS START************************************/

/****************************************
 * Name: wheelson_sonic
 * Desc: Struct for interfacing with ultra sonic
	     sensor
 * NOTE: User must provide designated functions
 ****************************************/
typedef struct wheelson_sonic{
	
	/************************USER CONFIG START******************/
	
	/******************************
	 * Application provided data: 
	 * Note: assumes hardware timer 
	 * software implementation could
	 * also be made 
	 ******************************/
	float timer_freq;			  //mcu_clk/timer_div 
	float mcu_clk;				  //processor clock speed 
	
	void (*timer_start)(void);    //function to start timer count 
	void (*timer_stop)(void);     //function to stop 
	void (*timer_rst)(void);      //function to reset count to 0
	uint32_t (*timer_read)(void); //read timer count register 
	void (*delay10us)(void);      //provide delay 10 microsecond function
	
	//function for setting/clearing trigger pin
	//NOTE: function should use gpio_state_t variable
	void (*gpio_write_trig)(gpio_state_t state);
	
	//function for reading echo pin 
	//NOTE: function should use gpio_state_t variable
	gpio_state_t (*gpio_read_echo)(void);      
	
	/*****************************************
	 * timeout is to force stop 
	 * polling if echo is not 
	 * received after specified
	 * iterations
	 * 
	 * timeout could indicate hardware 
	 * problems or that the nearest 
	 * obstacle is too far away for
	 * to return within a reasonable
	 * amount of time 
	 *
	 *******************************************/
	bool timeout_enable;  //enable timeout functionality
	uint32_t timeout_max; //if echo takes more iteration than timeout
	
	/************************USER CONFIG END******************/
	
	/************************OUTPUTS START******************/
	
	/***********************
	 * system outputs:
	 * Note: user has option 
	 * or reading distance in
	 * meters or time for return
	 * in microseconds
	 ***********************/
	float output_us;
	float output_meters;
	
	/************************OUTPUTS END********************/
	
}wheelson_sonic_t;

/******************************STRUCTS END**************************************/

/******************************FUNCTIONS START***************************************/

/***************************
 * Name: SonicUpdate
 * Desc: updates output variables 
 * in sonic struct 
 ***************************/
sensor_stat_t WUS_SonicUpdate(wheelson_sonic_t *ptr_sensor);

/***************************
 * Name: Get Functions 
 * Desc: returns output 
 * variables in the struct 
 * kind of arbitary but could 
 * make code look cleaner 
 ***************************/
float WUS_GetMeters(wheelson_sonic_t sensor);
uint32_t WUS_GetMicroSec(wheelson_sonic_t sensor);

/******************************FUNCTIONS END*****************************************/


#endif /* WHEELSON_ULTRASONIC_H_ */
