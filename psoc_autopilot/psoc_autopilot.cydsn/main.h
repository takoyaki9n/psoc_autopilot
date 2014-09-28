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

#define USB_EN
#define UART_TIMEOUT 100e+3
#define PWM_E_MAX 2000
#define PWM_E_MIN 1000
#define Q_COUNT 20

enum { //操縦モード
	MODE_MANUAL,
	MODE_STRAIGHT,
};
enum { //pwm_e インデックス
	PWM_ELV,
	PWM_RUD,
	PWM_THR,
	PWM_BAG,
	
	PWMS
};
enum { //K インデックス
/*	GAIN_YAW_P,
	GAIN_YAW_I,
	GAIN_YAW_D,*/
	GAIN_PITCH_P,
	GAIN_PITCH_I,
	GAIN_PITCH_D,
	GAIN_ROLL_P,
	GAIN_ROLL_I,
	GAIN_ROLL_D,
	
	GAINS
};

CY_ISR_PROTO(ISR_MAIN);
CY_ISR_PROTO(ISR_SENSOR);

int UARTWait(uint32 timeout);

/* [] END OF FILE */
