#include "headfile.h"


void jiancha(void)
{
	int x;
	if(MPU_Init()==0)
		printf("MPU_Init is OK\n");
	else 
	{
		x= MPU_Init();
		printf("MPU_Init is ON:%d\n",x);
	}
//检查MPU_Init()
	if(mpu_dmp_init()==0)
		printf("mpu_dmp_init is OK\n");
	else 
	{
		x= mpu_dmp_init();
		printf("mpu_dmp_init is ON:%d\n",x);
	}
//mpu_dmp_init()
}



int main(void)
{	
	int Point_LB = 500;
	static float pitch,roll,yaw; 		//欧拉角
	static short aacx,aacy,aacz;		//加速度传感器原始数据
	static short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	//	static short temp;					//温度	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	TIM1_PWM_Init();
	TIM7_Init(1000);//控制定时器
	Encoder_Init();
	C5_C12_init();
	Control_init();
	delay_init();	    	 //延时函数初始化	  
//	uart_init(115200);	 //串口初始化为115200
	uart3_init(115200);
	/*在此处写初始化函数*/
	MPU_Init();					//初始化MPU6050
	mpu_dmp_init();			//初始化MPU6050的IIC
//	hcsr04_init();
	
	/*在这里写初始化函数*/
//	jiancha();
	while(1)
	{	
//		hcsr04_trig();
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			vofa_printmpu6050(pitch,roll,yaw);
		}
		printf("POINT:%d,%d\n",Point_LB,M_LB);
//		delay_ms(100);
	}	 
}

