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
//���MPU_Init()
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
	static float pitch,roll,yaw; 		//ŷ����
	static short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	static short gyrox,gyroy,gyroz;	//������ԭʼ����
	//	static short temp;					//�¶�	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM1_PWM_Init();
	TIM7_Init(1000);//���ƶ�ʱ��
	Encoder_Init();
	C5_C12_init();
	Control_init();
	delay_init();	    	 //��ʱ������ʼ��	  
//	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	uart3_init(115200);
	/*�ڴ˴�д��ʼ������*/
	MPU_Init();					//��ʼ��MPU6050
	mpu_dmp_init();			//��ʼ��MPU6050��IIC
//	hcsr04_init();
	
	/*������д��ʼ������*/
//	jiancha();
	while(1)
	{	
//		hcsr04_trig();
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			vofa_printmpu6050(pitch,roll,yaw);
		}
		printf("POINT:%d,%d\n",Point_LB,M_LB);
//		delay_ms(100);
	}	 
}

