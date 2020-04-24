#include "uart.h"
void uart_put(unsigned char ch)//字符发送函数
{
	SBUF=ch;//将要发送的数据缓冲到SBUF寄存器准备发送
	while(TI==0)//等待发送完成，数据发送完成时，TI硬件置1，触发中断，进入中断服务函数
	continue;
	TI=0;//TI软件置零，为下一次发送做准备
}
void uart_puts(unsigned char* str)//字符串发送函数，调用字符发送函数实现
{
	while(*str!='\0')
		uart_put(*str++);
}

void uart_init(void)//初始化各个寄存器
{
	EA=1;//使能中断总开关
	ES=1;//使能串口中断
	ET1=1;//使能定时器T1
	
	TMOD=0X20;//定时器T1工作在定时状态模式2
	SCON=0X50;//串口工作在模式1
	PCON=0X80;//波特率倍增
	
	TH1=0XF3;//波特率计算参考网上公式12MHz 4800
	TL1=0XF3;
	
	TR1=1;//运行定时器T1
}