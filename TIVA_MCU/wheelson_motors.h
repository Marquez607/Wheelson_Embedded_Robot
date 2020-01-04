/***********************************************
 * Name: wheelson_motor.h
 * Author: Maruez Jones
 * Desc: abstraction for using wheeled robot with 
 *		 pwm motors interfaced through H-Bridge
 *      
 * 	     code was designed to portable to any 
 *		 embedded microcontroller assuming the 
 *       host software provides pwm functions in
 *       form specified by structs 
 *
 *       in this, host software is expected to 
 *       also initialize pwm systems used in design 
 * 
 * ACRONYMS:
 * WM - wheelson motor 
 ***********************************************/

/***********************************
	PWM POS | PWM NEG | MOTOR 
 		L	| 	L 	  | STOP
		L	| 	H	  | REVERSE
		H	|	L	  | FORWARD
		H	|	H	  | STOP 
************************************/

#ifndef WHEELSON_MOTORS_H_
#define WHEELSON_MOTORS_H_

/*****************************************DEFINES START******************************************/
#define WM_NO_WEIGHT 1 
#define WM_NO_BIAS 0

/*****************************************DEFINES END********************************************/

/*****************************************STRUCTS START******************************************/

/****************************************
 * Name: wheelson_motor2
 * Desc: Abstraction data for robot 
 *	     with 2 motors using H-BRIDGE
 * NOTE: User must provide pwm functions 
 ****************************************/
typedef struct wheelson_motor2{
	
	/****************************************
	 * Name: weights
	 * Desc: used in the case of motors
	 * having unequel max speeds 
	 * basically caps max speed by multiplying
	 *  
	 * NOTE: IF UNUSED, SET TO WM_NO_WEIGHT(1)
	 ****************************************/
	float left_weight;  
	float right_weight; 
	
	/****************************************
	 * Name: bias
	 * Desc: similar to the weights
	 * in that it attempts to balance speed
	 * between two unequal motors except 
	 * it adds/subtracts from input duty 
	 * cycle
	 *
	 * NOTE: IF UNUSED, SET TO WM_NO_BIAS(0)
	 ***************************************/
	float left_bias; 
	float right_bias;

	/****************************************
	 * Name: Speed functions
	 * Desc:
	 * User provided drivers for setting
	 * PWM speed
	 * 
	 * these functions set motor speed for
	 * each channel
	 *  
	 * Inputs:
	 * duty - duty cycle of motor
	 * should be between 0 and 1 
	 *
	 * NOTE: THIS CODE ASSUMES H-BRIDGE WITH
	 * TWO PWM PER MOTOR FOR FORWARD AND 
	 * REVERSE OPERATION 
	 *
	 ****************************************/
	void (*speed_right_pos)(float duty); //forward terminal of right motor
	void (*speed_right_neg)(float duty);  //reverse terminal of right motor 
	void (*speed_left_pos)(float duty);  //forward terminal of left motor
	void (*speed_left_neg)(float duty);   //reverse terminal of left motor 

}wheelson_motor2_t;

/*****************************************STRUCTS END********************************************/

/*****************************************FUNCTIONS START****************************************/

/********************************************
 * Name: Stop 
 * Desc: Sets all pwm channel to low
*******************************************/
void WM_Stop(wheelson_motor2_t motor_data);

/*******************************************************
 * Name: Forward/Reverse (2 motors)
 * Desc: sets both motors forward/reverse
 * 	     in other words the motors spin same direction
 * Inputs:
 * motor_data - struct containing motor data/functions
 * duty_left/duty_right - duty cycle for both motors 
 *******************************************************/
void WM_Forward2(float duty_left, float duty_right, wheelson_motor2_t motor_data);
void WM_Reverse2(float duty_left, float duty_right, wheelson_motor2_t motor_data);

/*********************************************************
 * Name: Turn Left/Right (2 motors)
 * Desc: set motors opposite directions of each other 
 * TurnLeft:
 * left motor set to reverse, right motor set to forward 
 * 
 * TurnRight:
 * left motor set to forward, right motor set to reverse 
 *
 *********************************************************/
void WM_TurnLeft2(float duty_left, float duty_right, wheelson_motor2_t motor_data);
void WM_TurnRight2(float duty_left, float duty_right, wheelson_motor2_t motor_data);

/*****************************************FUNCTIONS END****************************************/

#endif /* WHEELSON_MOTORS_H_ */

