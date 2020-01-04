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

#include "wheelson_motors.h"

 /***********************************
	PWM POS | PWM NEG | MOTOR 
 		L	| 	L 	  | STOP
		L	| 	H	  | REVERSE
		H	|	L	  | FORWARD
		H	|	H	  | STOP 
************************************/

/*****************************************FUNCTIONS START****************************************/

/*******************************************************
 * Name: Forward/Reverse (2 motors)
 * Desc: sets both motors forward/reverse
 * 	     in other words the motors spin same direction
 * Inputs:
 * motor_data - struct containing motor data/functions
 * duty_left/duty_right - duty cycle for both motors 
 *******************************************************/
void WM_Forward2(float duty_left, float duty_right, wheelson_motor2_t motor_data){
	
	/***********************************
	PWM POS | PWM NEG | MOTOR 
 		L	| 	L 	  | STOP
		L	| 	H	  | REVERSE
		H	|	L	  | FORWARD
		H	|	H	  | STOP 
	************************************/

	//biased and weighed duty
	duty_left = motor_data.left_weight * duty_left + motor_data.left_bias;
	duty_right = motor_data.right_weight * duty_right + motor_data.right_bias;
	
	//RIGHT MOTOR 
	motor_data.speed_right_pos(duty_right);
	motor_data.speed_right_neg(0);
	
	//LEFT MOTOR 
	motor_data.speed_left_pos(duty_left);
	motor_data.speed_left_neg(0);
	
}
void WM_Reverse2(float duty_left, float duty_right, wheelson_motor2_t motor_data){
	
	/***********************************
	PWM POS | PWM NEG | MOTOR 
 		L	| 	L 	  | STOP
		L	| 	H	  | REVERSE
		H	|	L	  | FORWARD
		H	|	H	  | STOP 
	************************************/

	//biased and weighed duty
	duty_left = motor_data.left_weight * duty_left + motor_data.left_bias;
	duty_right = motor_data.right_weight * duty_right + motor_data.right_bias;
	
	//RIGHT MOTOR 
	motor_data.speed_right_pos(0);
	motor_data.speed_right_neg(duty_right);
	
	//LEFT MOTOR 
	motor_data.speed_left_pos(0);
	motor_data.speed_left_neg(duty_left);
	
}

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
void WM_TurnLeft2(float duty_left, float duty_right, wheelson_motor2_t motor_data){
	
	/***********************************
	PWM POS | PWM NEG | MOTOR 
 		L	| 	L 	  | STOP
		L	| 	H	  | REVERSE
		H	|	L	  | FORWARD
		H	|	H	  | STOP 
	************************************/

	//biased and weighed duty
	duty_left = motor_data.left_weight * duty_left + motor_data.left_bias;
	duty_right = motor_data.right_weight * duty_right + motor_data.right_bias;
	
	//RIGHT MOTOR 
	motor_data.speed_right_pos(duty_right);
	motor_data.speed_right_neg(0);
	
	//LEFT MOTOR 
	motor_data.speed_left_pos(0);
	motor_data.speed_left_neg(duty_left);
	
}
void WM_TurnRight2(float duty_left, float duty_right, wheelson_motor2_t motor_data){
	
	/***********************************
	PWM POS | PWM NEG | MOTOR 
 		L	| 	L 	  | STOP
		L	| 	H	  | REVERSE
		H	|	L	  | FORWARD
		H	|	H	  | STOP 
	************************************/

	//biased and weighed duty
	duty_left = motor_data.left_weight * duty_left + motor_data.left_bias;
	duty_right = motor_data.right_weight * duty_right + motor_data.right_bias;
	
	//RIGHT MOTOR 
	motor_data.speed_right_pos(0);
	motor_data.speed_right_neg(duty_right);
	
	//LEFT MOTOR 
	motor_data.speed_left_pos(duty_left);
	motor_data.speed_left_neg(0);
	
}

/********************************************
 * Name: Stop 
 * Desc: Sets all pwm channel to low
*******************************************/
void WM_Stop(wheelson_motor2_t motor_data){
		
	//RIGHT MOTOR 
	motor_data.speed_right_pos(0);
	motor_data.speed_right_neg(0);
	
	//LEFT MOTOR 
	motor_data.speed_left_pos(0);
	motor_data.speed_left_neg(0);
	
}
/*****************************************FUNCTIONS END****************************************/
