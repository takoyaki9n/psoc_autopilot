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

float acc_value[3], gyr_value[3], mag_value[3];
int16 counter_value[COUNTERS];

CY_ISR(ISR_SENSOR){
	updateSensors(acc_value, gyr_value, mag_value);
}

CY_ISR(ISR_MAIN){
	int16 pwm_e[SERVOS];
	int8 i;
	
	pwm_e[INDEX_SERVO_1] = counter_value[INDEX_COUNT_1];
	pwm_e[INDEX_SERVO_2] = counter_value[INDEX_COUNT_2];
	for(i = 0; i < SERVOS; i++){
		if(pwm_e[i] < PWM_E_MIN){
			pwm_e[i] = PWM_E_MIN;
		}else if(pwm_e[i] > PWM_E_MAX){
			pwm_e[i] = PWM_E_MAX;
		}
	}
	PWM_1_WriteCompare(pwm_e[INDEX_SERVO_1]);
	PWM_2_WriteCompare(pwm_e[INDEX_SERVO_2]);
}

void initPWMs(){
	PWM_1_Start();
	PWM_2_Start();	
}

int main(){
	char str[64];
	
	CyGlobalIntEnable;
#ifdef USB_EN	
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
	while(!USBUART_1_GetConfiguration());
	USBUART_1_CDC_Init();
#endif	
	ISR_SENSOR_StartEx(ISR_SENSOR);
	ISR_MAIN_StartEx(ISR_MAIN);
	initSensors();
	initCounters();
	initPWMs();
    for(;;)
    {
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
