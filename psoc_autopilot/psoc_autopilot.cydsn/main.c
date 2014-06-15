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
#include "common.h"
#include "sensor.h"

CY_ISR_PROTO(ISR_MAIN);
CY_ISR_PROTO(ISR_SENSOR);

float acc[3], gyr[3], mag[3];
char str[64];

CY_ISR(ISR_MAIN){
}

CY_ISR(ISR_SENSOR){
	updateSensors(acc, gyr, mag);
}

int main()
{
	CyGlobalIntEnable;
	
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
	while(!USBUART_1_GetConfiguration());
	USBUART_1_CDC_Init();
	
	ISR_SENSOR_StartEx(ISR_SENSOR);
	ISR_MAIN_StartEx(ISR_MAIN);
	initSensors();
    for(;;)
    {
		sprintf(str, "acc: %f, %f, %f\r\n", acc[0], acc[1], acc[2]);
		while(USBUART_1_CDCIsReady() == 0u);
		USBUART_1_PutString(str);
		
		sprintf(str, "gyr: %f, %f, %f\r\n", gyr[0], gyr[1], gyr[2]);
		while(USBUART_1_CDCIsReady() == 0u);
		USBUART_1_PutString(str);
		
		sprintf(str, "mag: %f, %f, %f\r\n", mag[0], mag[1], mag[2]);
		while(USBUART_1_CDCIsReady() == 0u);
		USBUART_1_PutString(str);
		
		CyDelay(1000);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
