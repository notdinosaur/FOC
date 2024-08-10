#ifndef __AS5600_H__
#define __AS5600_H__

#include "main.h"
#include <stdio.h>
#include "stm32f4xx_hal_i2c.h"
#define PI					3.14159265358979f
#define cpr (float)(2.0f*PI)
#define Slave_Addr                0x36<<1//设备从地址
#define Write_Bit                 0	   //写标记	
#define Read_Bit                  1    //读标记
#define Angle_Hight_Register_Addr 0x0C //寄存器高位地址
#define Angle_Low_Register_Addr   0x0D //寄存器低位地址
extern I2C_HandleTypeDef hi2c1;
#define AS5600i2c hi2c1
unsigned char read_reg(unsigned char reg, unsigned char* buf, unsigned short len);
float i2c_AS5600_get_angle(void);
unsigned char write_reg(unsigned char reg, unsigned char value);
float AS5600_get(void);
float as5600_work(void);
#endif
//---

//# 总结
//相比模拟IIC，代码很简单，已经经过验证，另外，程序使用的STM32的IIC1。
