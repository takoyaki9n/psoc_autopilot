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
#include <project.h>
#include <device.h>
#include <stdio.h>
#include "common.h"
#include "sensor.h"

#define I2C_TXBUFSIZE 2
#define I2C_RXBUFSIZE 8

uint8 I2Ctxbuf[I2C_TXBUFSIZE];
uint8 I2Crxbuf[I2C_RXBUFSIZE];

float gyr_offset[3];

//デバッグ用
int16 cnt;
uint32 dt;

void I2CWait(uint32 timeout, uint8 flagToWaitOn) {
	dt = Timer_Global_ReadCounter();
	timeout = Timer_Global_ReadCounter() - timeout;
	while(!(I2C_MasterStatus() & flagToWaitOn) && timeout <= Timer_Global_ReadCounter());
	dt = dt - Timer_Global_ReadCounter();
}
void I2CReadWait(uint32 timeout) {
	I2CWait(timeout, I2C_MSTAT_RD_CMPLT);
}
void I2CWriteWait(uint32 timeout) {
	I2CWait(timeout, I2C_MSTAT_WR_CMPLT);
}

void RegWrite(uint8 slaveAddr, uint8 addr, uint8 dat) {
	I2Ctxbuf[0] = addr;
	I2Ctxbuf[1] = dat;
	
	I2C_MasterClearStatus();
	I2C_MasterWriteBuf(slaveAddr, I2Ctxbuf, 2, I2C_MODE_COMPLETE_XFER);
	I2CWriteWait(I2C_TIMEOUT);
}
void UpdateMag(){
    RegWrite(MAG_SLAVE_ADDR, MAG_UPDATE_REG, MAG_UPDATE_DAT);
}

void RegRead(uint8 slaveAddr, uint8 addr, uint8 cnt) {
	I2Ctxbuf[0] = addr;
	I2C_MasterClearStatus();
	I2C_MasterWriteBuf(slaveAddr, I2Ctxbuf, 1, I2C_MODE_COMPLETE_XFER);
	I2CWriteWait(I2C_TIMEOUT);

	I2C_MasterClearStatus();
	I2C_MasterReadBuf(slaveAddr, I2Crxbuf, cnt, I2C_MODE_COMPLETE_XFER);
	I2CReadWait(I2C_TIMEOUT);
}
void GetAccData(float *dat) {
	RegRead(MPU_SLAVE_ADDR, ACC_DATA_TOP_REG, 6);
	dat[0] = Byte2float(I2Crxbuf[0], I2Crxbuf[1]) * ACC_SCALE;
	dat[1] = Byte2float(I2Crxbuf[2], I2Crxbuf[3]) * ACC_SCALE;
	dat[2] = Byte2float(I2Crxbuf[4], I2Crxbuf[5]) * ACC_SCALE;
}
void GetGyrData(float *dat) {
	RegRead(MPU_SLAVE_ADDR, GYR_DATA_TOP_REG, 6);
	dat[0] = Byte2float(I2Crxbuf[0], I2Crxbuf[1] - gyr_offset[0]) * GYR_SCALE;
	dat[1] = Byte2float(I2Crxbuf[2], I2Crxbuf[4] - gyr_offset[1]) * GYR_SCALE;
	dat[2] = Byte2float(I2Crxbuf[4], I2Crxbuf[5] - gyr_offset[2]) * GYR_SCALE;
}
void GetMagData(float *dat) {
	RegRead(MAG_SLAVE_ADDR, MAG_DATA_TOP_REG, 6);
	dat[0] = Byte2float(I2Crxbuf[1], I2Crxbuf[0]) * MAG_SCALE;
	dat[1] = Byte2float(I2Crxbuf[3], I2Crxbuf[2]) * MAG_SCALE;
	dat[2] = Byte2float(I2Crxbuf[5], I2Crxbuf[4]) * MAG_SCALE;
}

void initSensors() {
	uint8 i, j;
	float gyr_tmp[3];
	
	I2C_Start();
	
	//スリープモード解除
    RegWrite(MPU_SLAVE_ADDR, MPU_WAKEUP_REG1, MPU_WAKEUP_DAT1);
    RegWrite(MPU_SLAVE_ADDR, MPU_WAKEUP_REG2, MPU_WAKEUP_DAT2);
	//測定range設定
    RegWrite(MPU_SLAVE_ADDR, ACC_CONFIG_REG, ACC_CONFIG_DAT);
    RegWrite(MPU_SLAVE_ADDR, GYR_CONFIG_REG, GYR_CONFIG_DAT);

	for(i = 0; i < 3; i++) {
		gyr_offset[i] = 0;
	}
	for(i = 0; i < 10; ++i) {
		GetGyrData(gyr_tmp);
		for(j = 0; j < 3 ;j++) {
			gyr_offset[j] = gyr_tmp[j] + gyr_offset[j];
		}
		CyDelay(100);
	}
	for(i = 0; i < 3; i++) {
		gyr_offset[i] *= 0.1;
	}
}

void updateSensors(float *acc, float *gyr, float *mag) {
	GetAccData(acc);
	GetGyrData(gyr);
	GetMagData(mag);
	UpdateMag();
}
/* [] END OF FILE */
