#include "uart.h"
void uart_put(unsigned char ch)//�ַ����ͺ���
{
	SBUF=ch;//��Ҫ���͵����ݻ��嵽SBUF�Ĵ���׼������
	while(TI==0)//�ȴ�������ɣ����ݷ������ʱ��TIӲ����1�������жϣ������жϷ�����
	continue;
	TI=0;//TI������㣬Ϊ��һ�η�����׼��
}
void uart_puts(unsigned char* str)//�ַ������ͺ����������ַ����ͺ���ʵ��
{
	while(*str!='\0')
		uart_put(*str++);
}

void uart_init(void)//��ʼ�������Ĵ���
{
	EA=1;//ʹ���ж��ܿ���
	ES=1;//ʹ�ܴ����ж�
	ET1=1;//ʹ�ܶ�ʱ��T1
	
	TMOD=0X20;//��ʱ��T1�����ڶ�ʱ״̬ģʽ2
	SCON=0X50;//���ڹ�����ģʽ1
	PCON=0X80;//�����ʱ���
	
	TH1=0XF3;//�����ʼ���ο����Ϲ�ʽ12MHz 4800
	TL1=0XF3;
	
	TR1=1;//���ж�ʱ��T1
}