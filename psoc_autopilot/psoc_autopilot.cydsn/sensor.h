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

#define I2C_TIMEOUT	1

//Fullscale range
#define GYR_RANGE 2000.0 //dps
#define ACC_RANGE 16.0   //g
#define MAG_RANGE 1200.0 //uT

//Sensitivity
#define GYR_SCALE (GYR_RANGE / 0x8000)
#define ACC_SCALE (ACC_RANGE / 0x8000)
#define MAG_SCALE (MAG_RANGE / 0x1000)

//I2C 7bit slave address
#define MPU_SLAVE_ADDR 0x68 //加速度・ジャイロ部
#define MAG_SLAVE_ADDR 0x0A //磁気
//データの先頭のレジスタ
#define ACC_DATA_TOP_REG 0x3B
#define GYR_DATA_TOP_REG 0x43 //(0x3B + 0x08)
#define MAG_DATA_TOP_REG 0x03
//初期化
#define MPU_WAKEUP_REG1 0x6B
#define MPU_WAKEUP_DAT1 0x00
#define MPU_WAKEUP_REG2 0x37
#define MPU_WAKEUP_DAT2 0x02
//設定
#define GYR_CONFIG_REG 0x1B
#define GYR_CONFIG_DAT 0x18 //range = 2000dps
#define ACC_CONFIG_REG 0x1C
#define ACC_CONFIG_DAT 0x18 //range = 16g
//磁気更新
#define MAG_UPDATE_REG 0x0A 
#define MAG_UPDATE_DAT 0x01 

void GetAccData(float *dat);
void GetGyrData(float *dat);
void GetMagData(float *dat);
void initSensors();
void updateSensors(float *acc, float *gyr, float *mag);

//[] END OF FILE
