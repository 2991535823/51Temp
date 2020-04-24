#include "data.h"
#include "stdio.h"
#include "uart.h"
#include "string.h"
//处理串口接受的数据
void unpackage_data(u8* str,char* templ,char* temph)
{
	char* l=strchr(str,':');
	char* h=strrchr(str,':');
	templ[0]=*(l+1);templ[1]=*(l+2);templ[2]='\0';
	temph[0]=*(h+1);templ[1]=*(h+2);templ[2]='\0';
	str[0]='0';//结束数据处理，除非有新数据到达
}
//这里是发往另外单片机的数据打包函数，
void package_data(char templ,char temph,u16 temp_val)
{
	char send_data[25]={'0'};//格式为“{"L":23,"H":28,"N":25}”
	sprintf(send_data,"{\"L\"=%c,\"H\"=%c,\"N\"=%d}\0",templ,temph,(int)temp_val);
	uart_puts(send_data);
}