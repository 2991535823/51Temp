#ifndef _uart_H
#define _uart_H
#include "reg52.h"

void uart_put(unsigned char ch);//字符发送函数
void uart_puts(unsigned char* str);//字符串发送函数
void uart_init(void);//串口初始化函数



#endif