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
#include <project.h>
#include "main.h"
#include "common.h"
#include "sensor.h"
#include "counter.h"

extern uint32 dt;
float acc_value[3], gyr_value[3], mag_value[3];
int16 counter_value[COUNTERS];

CY_ISR(ISR_SENSOR){
	updateSensors(acc_value, gyr_value, mag_value);
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
	Init_LED_Out_Write(1);
	//電源投入直後は待つ
	CyDelay(1000);
	Init_LED_Out_Write(0);
	CyDelay(100);
	Init_LED_Out_Write(1);
	CyGlobalIntEnable;
#ifdef USB_EN	
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
	while(!USBUART_1_GetConfiguration());
	USBUART_1_CDC_Init();
#endif
	Timer_Global_Start();
	initSensors();
	initCounters();
	initPWMs();
	ISR_SENSOR_StartEx(ISR_SENSOR);
	ISR_MAIN_StartEx(ISR_MAIN);
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
#endif
		CyDelay(1000);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
