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

//#define USB_EN
#define PWM_E_MAX 2000
#define PWM_E_MIN 1000

enum { //pwm_e インデックス
	PWM_THR,
	PWM_ELV,
	PWM_RUD,
	
	PWMS
};

CY_ISR_PROTO(ISR_MAIN);
CY_ISR_PROTO(ISR_SENSOR);

/* [] END OF FILE */