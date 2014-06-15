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
#include <project.h>
#include "common.h"
#include "sensor.h"

CY_ISR_PROTO(ISR_MAIN);
CY_ISR_PROTO(ISR_SENSOR);

float acc[3], gyr[3], mag[3];

CY_ISR(ISR_MAIN){
	
}

CY_ISR(ISR_SENSOR){
	updateSensors(acc, gyr, mag);
}

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	CyGlobalIntEnable;
	ISR_SENSOR_StartEx(ISR_SENSOR);
	ISR_MAIN_StartEx(ISR_MAIN);
	initSensors();
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
