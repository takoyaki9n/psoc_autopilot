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
#include "MahonyAHRS.h"

int8 pilot_mode;
float K[GAINS];
float acc_value[3], gyr_value[3], mag_value[3];
float q_buf[Q_COUNT][4], q_ave[4];
int8 q_i;
int16 counter_value[COUNTERS];

CY_ISR(ISR_SENSOR){
    int8 i, j;
    float recipNorm;
    
	updateSensors(acc_value, gyr_value, mag_value); // 2000 usec
    
	MahonyAHRSupdate(gyr_value[0], gyr_value[1], gyr_value[2], acc_value[0], acc_value[1], acc_value[2], mag_value[0], mag_value[1], mag_value[2]); // 5000 usec
    
    q_buf[q_i][0] = q0; q_buf[q_i][1] = q1; q_buf[q_i][2] = q2; q_buf[q_i][3] = q3;
    q_ave[0] = 0; q_ave[1] = 0; q_ave[2] = 0; q_ave[3] = 0;
    for (i = 0; i < Q_COUNT; i++) {
        for (j = 0; j < 4; j++) {
            q_ave[j] += q_buf[i][j];
        }
    }
    recipNorm = invSqrt(q_ave[0]*q_ave[0] + q_ave[1]*q_ave[1] + q_ave[2]*q_ave[2] + q_ave[3]*q_ave[3]);
    for (j = 0; j < 4; j++) {
        q_ave[j] *= recipNorm;
    }

    q_i = (q_i + 1) % Q_COUNT;
}

CY_ISR(ISR_MAIN){
	int16 pwm_e[PWMS];
    float gyr[3];
	float delta[PWMS-2];
    float roll, pitch;//, yaw;
	int8 i;
    
	for(i = 0; i < 3; i++){
        gyr[i] = gyr_value[i];
    }

//    x = atan2(2*q2*q3 - 2*q0*q1, 2*q0*q0 + 2*q3*q3 - 1); //phi
//    y = -asin(2*(q1*q3 + q0*q2)); //theta
//    z = atan2(2*q1*q2 - 2*q0*q3, 2*q0*q0 + 2*q1*q1 - 1); //psy
    //センサーが横向きに取り付けてあるので
    roll  = atan2(2*q_ave[2]*q_ave[3] - 2*q_ave[0]*q_ave[1], 2*q_ave[0]*q_ave[0] + 2*q_ave[3]*q_ave[3] - 1); //phi
    pitch = -asin(2*(q_ave[1]*q_ave[3] + q_ave[0]*q_ave[2])); //theta
	
    if (counter_value[COUNTER_MOD] <= 1200) {
        pilot_mode = MODE_MANUAL;
    } else if (counter_value[COUNTER_MOD] <= 1700) {
        if (pilot_mode != MODE_STRAIGHT) {
        }
        pilot_mode = MODE_STRAIGHT;
    } else {
        pilot_mode = MODE_MANUAL;
    }
    //TODO デバッグ用コード削除
    pilot_mode = MODE_STRAIGHT;
    
    if (pilot_mode == MODE_MANUAL) {
    	pwm_e[PWM_ELV] = counter_value[COUNTER_ELV];
    	pwm_e[PWM_RUD] = counter_value[COUNTER_RUD];
    	pwm_e[PWM_THR] = counter_value[COUNTER_THR];
    } else if (pilot_mode == MODE_STRAIGHT) {
#define PITCHC (0.0)
#define ROLLC  (0.0)
		delta[PWM_ELV] = /*K[GAIN_PITCH_D] * gyr[2] +*/ K[GAIN_PITCH_P] * (pitch - PITCHC);
		delta[PWM_RUD] = /*K[GAIN_ROLL_D ] * gyr[0] +*/ K[GAIN_ROLL_P ] * (roll  - ROLLC );
    	pwm_e[PWM_ELV] = counter_value[COUNTER_ELV] + delta[PWM_ELV];
        pwm_e[PWM_RUD] = counter_value[COUNTER_RUD] + delta[PWM_RUD];
        pwm_e[PWM_THR] = counter_value[COUNTER_THR];
    }
        
    //サチュレーション制御
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

void initK(){
	K[GAIN_PITCH_P] = 100.0f;
	K[GAIN_PITCH_I] = 0.0f;
	K[GAIN_PITCH_D] = -6.0f;
	K[GAIN_ROLL_P ] = 100.0f;
	K[GAIN_ROLL_I ] = 0.0f;
	K[GAIN_ROLL_D ] = -12.0f;
}

void initPWMs(){
	PWM_Thr_Start();
	PWM_Elv_Start();	
	PWM_Rud_Start();	
//	PWM_Bag_Start();	
}

void init(){
	int i, j;
	//電源投入直後は待つ
	for(i = 0; i < 2; i++){
		Init_LED_Out_Write(1);
		CyDelay(500);
		Init_LED_Out_Write(0);
		CyDelay(500);
	}
	
	Init_LED_Out_Write(1);
    
    //モード初期化
    pilot_mode = MODE_MANUAL;
    
    //q初期化
    q_i = 0;
    q_ave[0] = q0; q_ave[1] = q1; q_ave[2] = q2; q_ave[3] = q3;
    for (i = 0; i < Q_COUNT; i++) {
        for (j = 0; j < 4; j++) {
            q_buf[i][j] = q_ave[j];
        }
    }
    
    initK();    
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
	float qtmp[4];
	
	init();
	
	for(;;){
#ifdef USB_EN
		CyDelay(100);
		Init_LED_Out_Write(1);

        if (UARTWait(UART_TIMEOUT)){
//			qtmp[0] = q0; qtmp[1] = q1; qtmp[2] = q2; qtmp[3] = q3;
			qtmp[0] = q_ave[0]; qtmp[1] = q_ave[1]; qtmp[2] = q_ave[2]; qtmp[3] = q_ave[3];
			USBUART_1_PutData(qtmp, 16);
		}
		if (UARTWait(UART_TIMEOUT)){
			USBUART_1_PutCRLF();
		}
/*		
		if (UARTWait(UART_TIMEOUT)){
			sprintf(str, "%d\r\n", (int) dt);
			USBUART_1_PutString(str);
		}
		if (UARTWait(UART_TIMEOUT)){
			sprintf(str, "acc: %f, %f, %f\r\n", acc_value[0], acc_value[1], acc_value[2]);
			USBUART_1_PutString(str);
		}
		if (UARTWait(UART_TIMEOUT)){
			sprintf(str, "gyr: %f, %f, %f\r\n", gyr_value[0], gyr_value[1], gyr_value[2]);
			USBUART_1_PutString(str);
		}
		if (UARTWait(UART_TIMEOUT)){
			sprintf(str, "mag: %f, %f, %f\r\n", mag_value[0], mag_value[1], mag_value[2]);
			USBUART_1_PutString(str);
		}
		if (UARTWait(UART_TIMEOUT)){
			sprintf(str, "ang: %f, %f, %f\r\n\r\n", psy, phi, theta);
			USBUART_1_PutString(str);
		}*/
		Init_LED_Out_Write(0);
#endif
    }
}

int UARTWait(uint32 timeout){
	uint32 time = Timer_Global_ReadCounter();
	//タイマーがアンダーフローしても動くように毎回引き算する
	while(USBUART_1_CDCIsReady() == 0u && time - Timer_Global_ReadCounter() <= timeout);
	return USBUART_1_CDCIsReady() != 0u;
}

/* [] END OF FILE */
