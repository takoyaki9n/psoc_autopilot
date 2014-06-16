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
	INDEX_COUNT_1,
	INDEX_COUNT_2,
	INDEX_COUNT_3,
	INDEX_COUNT_4,
	
	COUNTERS,
};

void initCounters();

CY_ISR_PROTO(ISR_COUNT_1);
CY_ISR_PROTO(ISR_COUNT_2);
CY_ISR_PROTO(ISR_COUNT_3);
CY_ISR_PROTO(ISR_COUNT_4);

extern int16 counter_value[COUNTERS];
/* [] END OF FILE */
