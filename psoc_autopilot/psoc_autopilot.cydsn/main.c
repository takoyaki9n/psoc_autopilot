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

int main()
{
	float acc[3], gyr[3], mag[3];	
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	CyGlobalIntEnable;
	initSensors();
    for(;;)
    {
		GetAccData(acc);
		GetGyrData(gyr);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
