#include "imu.h"
#include "os.h"

#define IMU_GYRO_FS_CFG       MPU6050_GYRO_FS_2000
#define IMU_DEG_PER_LSB_CFG   MPU6050_DEG_PER_LSB_2000
#define IMU_ACCEL_FS_CFG      MPU6050_ACCEL_FS_8
#define IMU_G_PER_LSB_CFG     MPU6050_G_PER_LSB_8
#define IMU_1G_RAW            (int16_t)(1.0 / MPU6050_G_PER_LSB_8)

void imuInit(void)
{
	uint8_t ret=0;
    /*init i2c */
    i2cdevInit(I2C3_DEV);
    /*init mpu6050*/
    mpu6050Init(I2C3_DEV);
    /*test connecttion of mpu6050*/
    if((ret=mpu6050TestConnection())==true)
    {
        log_msg("Connect sucessfully\r\n");
    }else
    {
        log_msg("%s:ret=connnect failed\r\n",__FUNCTION__,ret);
//        return;
    }
    /* reset mpu */
    mpu6050Reset();
    vTaskDelay(100);
    /* Activate MPU6050*/
    mpu6050SetSleepEnabled(false);
    /*Enable temp sensor*/
    mpu6050SetTempSensorEnabled(true);
    /*Disable interrupts*/
    mpu6050SetIntEnabled(false);
    /* Set x-axis gyro as clock source*/
    mpu6050SetClockSource(MPU6050_CLOCK_PLL_XGYRO);
    /*Set gyro full scale range*/
    mpu6050SetFullScaleGyroRange(IMU_GYRO_FS_CFG);
    /*Set accelerometer full scale range*/
    mpu6050SetFullScaleAccelRange(IMU_ACCEL_FS_CFG);
    /* To low DLPF bandwidth might cause instability and decrease agility
     * but it works well for handling vibrations and unbalanced propellers
     * Set output rate (1): 1000 / (1 + 1) = 500Hz*/
    mpu6050SetRate(1);
    /* Set digital low-pass bandwidth*/
    mpu6050SetDLPFMode(MPU6050_DLPF_BW_188);
}
void imuTask(void* pvParametter)
{
    while(xTaskGetTickCount() < 1000);
    imuInit();
    for(;;)
    {
    	vTaskDelay(1);
    }
}
