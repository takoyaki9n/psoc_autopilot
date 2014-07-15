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
#include "counter.h"

CY_ISR(ISR_THR){
	counter_value[COUNTER_THR] = Counter_Thr_ReadCapture();
	//割り込みフラグクリア
	Counter_Thr_ReadStatusRegister();
}
CY_ISR(ISR_ELV){
	counter_value[COUNTER_ELV] = Counter_Elv_ReadCapture();
	//割り込みフラグクリア
	Counter_Elv_ReadStatusRegister();
}
CY_ISR(ISR_RUD){
	counter_value[COUNTER_RUD] = Counter_Rud_ReadCapture();
	//割り込みフラグクリア
	Counter_Rud_ReadStatusRegister();
}

void initCounters(){
	int i;
	for(i = 0; i < COUNTERS; i++){
		counter_value[i] = 0;	
	}
	Counter_Thr_Start();
	Counter_Elv_Start();
	Counter_Rud_Start();
}

void Counters_StartEx(){
	ISR_Thr_StartEx(ISR_THR);
	ISR_Elv_StartEx(ISR_ELV);
	ISR_Rud_StartEx(ISR_RUD);
}
/* [] END OF FILE */
