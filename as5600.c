#include "AS5600.h"

unsigned char write_reg(unsigned char reg, unsigned char value)
{
	return HAL_I2C_Mem_Write(&AS5600i2c, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 50);
}

unsigned char write_regs(unsigned char reg, unsigned char *value, unsigned char len)
{
	return HAL_I2C_Mem_Write(&AS5600i2c, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, value, len, 50);
}

unsigned char read_reg(unsigned char reg, unsigned char* buf, unsigned short len)
{
	return HAL_I2C_Mem_Read(&AS5600i2c, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 50);
}



float i2c_AS5600_get_angle(void)
{   float angle_d;
		int16_t in_angle;
    uint8_t temp[2]={0,0};
		read_reg( Angle_Hight_Register_Addr, temp, 2);
    //printf("%d  %d\n",temp[0],temp[1]);
    in_angle = ((int16_t)temp[0] <<8) | (temp[1]);
    angle_d = (float)in_angle * cpr / 4096;
		//printf("angle=%.2f\n",angle_d);

		return angle_d;
	//angle_d为弧度制，范围在0-6.28	
}
float AS5600_get(void)
{
	return i2c_AS5600_get_angle()/PI*180;
}



float as5600_work(void)
{
	static float angle_motor1 = 0 , angle_motor1_pre = 0 , angle_return , angle_cnt = 0;
	angle_motor1  = AS5600_get();

	if((angle_motor1 -  angle_motor1_pre)> (0.8f * 360.0f))
	{
		angle_cnt -= 1;
	}
	else if((angle_motor1 -  angle_motor1_pre) < (-0.8f * 360.0f) )
	{
		angle_cnt += 1;
	}
	angle_return = angle_cnt * 360.0f + angle_motor1;
	angle_motor1_pre = angle_motor1;
	return angle_return;
	
}

