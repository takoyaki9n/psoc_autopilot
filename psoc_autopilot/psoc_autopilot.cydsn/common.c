/* ========================================
 *
 * Copyright Avionics Research Group, 2012
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Avionics Research Group.
 *
 * ========================================
*/
#include <stdio.h>
#include "common.h"

//バイト列を符号無し整数に変換
uint16 Byte2uint(uint8 msb, uint8 lsb) {
	return ((uint16)msb << 8) + (uint16)lsb;
}

//バイト列を実数に変換
float Byte2float(uint8 msb, uint8 lsb) {
	int16 tmp;
	tmp = (int16)Byte2uint(msb, lsb);
	return (float) tmp;
}

float Deg2Rad(float deg) {
	return deg * (3.14159 / 180.0);
}

void IntToString(char *str, int number){
	sprintf(str, "%d", number);
}
void FloatToString(char *str, float number){
	sprintf(str, "%f", number);
}

uint32 FloatToUint32(float value){
	uint32 *p;
	p = (uint32 *) &value;
	return *p;
}

int maxInt(int x, int y){
    return x > y ? x : y;
}
/* [] END OF FILE */
