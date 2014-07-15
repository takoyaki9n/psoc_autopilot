/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <stdio.h>
#include <math.h>
#include <project.h>
#include "main.h"
#include "common.h"
#include "sensor.h"
#include "counter.h"
#include "MadgwickAHRS.h"

float acc_value[3], gyr_value[3], mag_value[3];
float psy, phi, theta;
int16 counter_value[COUNTERS];
extern uint32 dt;

CY_ISR(ISR_SENSOR){
	updateSensors(acc_value, gyr_value, mag_value);
	
//	MadgwickAHRSupdate(gyr_value[0], gyr_value[1], gyr_value[2], acc_value[0], acc_value[1], acc_value[2], mag_value[0], mag_value[1], mag_value[2]);	
	MadgwickAHRSupdateIMU(gyr_value[0], gyr_value[1], gyr_value[2], acc_value[0], acc_value[1], acc_value[2]);
	
	psy = atan2(2 * (q1 * q2 - q0 * q3), 2 * (q0 * q0 + q1 * q1) - 1) / 3.14 * 180;
	theta = - asin(2 * (q1 * q3 + q0 * q3))                           / 3.14 * 180;
	phi = atan2(2 * (q2 * q3 - q0 * q1), 2 * (q0 * q0 + q3 * q3) - 1) / 3.14 * 180;
}

CY_ISR(ISR_MAIN){
	int16 pwm_e[PWMS];
	int8 i;
	
	pwm_e[PWM_THR] = counter_value[COUNTER_THR];
	pwm_e[PWM_ELV] = counter_value[COUNTER_ELV];
	pwm_e[PWM_RUD] = counter_value[COUNTER_RUD];
	for(i = 0; i < PWMS; i++){
		if(pwm_e[i] < PWM_E_MIN){
			pwm_e[i] = PWM_E_MIN;
		}else if(pwm_e[i] > PWM_E_MAX){
			pwm_e[i] = PWM_E_MAX;
		}
	}
	PWM_Thr_WriteCompare(pwm_e[PWM_THR]);
	PWM_Elv_WriteCompare(pwm_e[PWM_ELV]);
	PWM_Rud_WriteCompare(pwm_e[PWM_RUD]);
}

void initPWMs(){
	PWM_Thr_Start();
	PWM_Elv_Start();	
	PWM_Rud_Start();	
}

void init(){
	int i;
	//電源投入直後は待つ
	for(i = 0; i < 2; i++){
		Init_LED_Out_Write(1);
		CyDelay(500);
		Init_LED_Out_Write(0);
		CyDelay(500);
	}
	
	Init_LED_Out_Write(1);
	CyGlobalIntEnable;
#ifdef USB_EN	
	USBUART_1_Start(0, USBUART_1_DWR_VDDD_OPERATION);
	while(!USBUART_1_GetConfiguration());
	USBUART_1_CDC_Init();
#endif
	Timer_Global_Start();
	initSensors();
	initCounters();
	initPWMs();
	ISR_SENSOR_StartEx(ISR_SENSOR);
	ISR_MAIN_StartEx(ISR_MAIN);
	Counters_StartEx();
	Init_LED_Out_Write(0);
}

int main(){
	char str[64];
	
	init();
	
	for(;;){
#ifdef USB_EN
		sprintf(str, "acc: %f, %f, %f\r\n", acc_value[0], acc_value[1], acc_value[2]);
		while(USBUART_1_CDCIsReady() == 0u);
		USBUART_1_PutString(str);
		
		sprintf(str, "gyr: %f, %f, %f\r\n", gyr_value[0], gyr_value[1], gyr_value[2]);
		while(USBUART_1_CDCIsReady() == 0u);
		USBUART_1_PutString(str);
		
		sprintf(str, "mag: %f, %f, %f\r\n", mag_value[0], mag_value[1], mag_value[2]);
		while(USBUART_1_CDCIsReady() == 0u);
		USBUART_1_PutString(str);
		
		sprintf(str, "ang: %f, %f, %f\r\n\r\n", psy, phi, theta);
		while(USBUART_1_CDCIsReady() == 0u);
		USBUART_1_PutString(str);
#endif
		CyDelay(1000);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
