#include "data.h"
#include "stdio.h"
#include "uart.h"
#include "string.h"
//�����ڽ��ܵ�����
void unpackage_data(u8* str,char* templ,char* temph)
{
	char* l=strchr(str,':');
	char* h=strrchr(str,':');
	templ[0]=*(l+1);templ[1]=*(l+2);templ[2]='\0';
	temph[0]=*(h+1);templ[1]=*(h+2);templ[2]='\0';
	str[0]='0';//�������ݴ��������������ݵ���
}
//�����Ƿ������ⵥƬ�������ݴ��������
void package_data(char templ,char temph,u16 temp_val)
{
	char send_data[25]={'0'};//��ʽΪ��{"L":23,"H":28,"N":25}��
	sprintf(send_data,"{\"L\"=%c,\"H\"=%c,\"N\"=%d}\0",templ,temph,(int)temp_val);
	uart_puts(send_data);
}