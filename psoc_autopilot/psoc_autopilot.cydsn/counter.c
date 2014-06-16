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

CY_ISR(ISR_COUNT_1){
	counter_value[INDEX_COUNT_1] = Counter_1_ReadCapture();
	//割り込みフラグクリア
	Counter_1_ReadStatusRegister();
}
CY_ISR(ISR_COUNT_2){
	counter_value[INDEX_COUNT_2] = Counter_2_ReadCapture();
	//割り込みフラグクリア
	Counter_2_ReadStatusRegister();
}
CY_ISR(ISR_COUNT_3){
	counter_value[INDEX_COUNT_3] = Counter_3_ReadCapture();
	//割り込みフラグクリア
	Counter_3_ReadStatusRegister();
}
CY_ISR(ISR_COUNT_4){
	counter_value[INDEX_COUNT_4] = Counter_4_ReadCapture();
	//割り込みフラグクリア
	Counter_4_ReadStatusRegister();
}

void initCounters(){
	int i;
	for(i = 0; i < COUNTERS; i++){
		counter_value[i] = 0;	
	}
	Counter_1_Start();
	Counter_2_Start();
	Counter_3_Start();
	Counter_4_Start();
}
/* [] END OF FILE */
