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

enum { //counter_value インデックス
	COUNTER_THR,
	COUNTER_ELV,
	COUNTER_RUD,
	COUNTER_BAG,
	
	COUNTERS,
};

void initCounters();
void Counters_StartEx();

CY_ISR_PROTO(ISR_THR);
CY_ISR_PROTO(ISR_ELV);
CY_ISR_PROTO(ISR_RUD);
CY_ISR_PROTO(ISR_BAG);

extern int16 counter_value[COUNTERS];
/* [] END OF FILE */
