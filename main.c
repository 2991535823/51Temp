#include "public.h"//引入头文件
#include "lcd.h"
#include "temp.h"
#include "i2c.h"
#include "uart.h"
#include "data.h"

sbit k3 = P3 ^ 2; 
sbit k1 = P3 ^ 4; 
sbit k2 = P3 ^ 5; 

sbit led = P2 ^ 4; 

sbit beep = P1 ^ 5;	 
sbit relay = P1 ^ 4; 
sbit moto = P1 ^ 0;	 

char set_templ = 23, set_temph = 28; 
u16 temp_val;						 
u8 mode;							 
u8 recive_data[16] = {'0'};//接受格式为“{"L":23,"H":28}”
u8 position = 0;
unsigned int time=0;
void Temp_DataPros()
{
	short temp;
	u8 temp_buf[5];
	temp = Ds18b20ReadTemp();
	temp_val = temp;
	if (temp < 0)
	{
		temp = -temp;
		LCD_Dispstring(2 + 5, 0, "-");
	}
	else
	{
		LCD_Dispstring(2 + 5, 0, " ");
	}

	temp_buf[0] = temp / 100 + 0x30;
	temp_buf[1] = temp % 100 / 10 + 0x30;
	temp_buf[2] = '.';
	temp_buf[3] = temp % 100 % 10 + 0x30;
	temp_buf[4] = '\0';
	LCD_Dispstring(2 + 6, 0, temp_buf);

	temp_buf[0] = set_temph / 10 + 0x30;
	temp_buf[1] = set_temph % 10 + 0x30;
	temp_buf[2] = '\0';
	LCD_Dispstring(5, 1, temp_buf);

	temp_buf[0] = set_templ / 10 + 0x30;
	temp_buf[1] = set_templ % 10 + 0x30;
	temp_buf[2] = '\0';
	LCD_Dispstring(14, 1, temp_buf);
}

#define K1_MODE 1
#define K2_ADD 2
#define K3_DEC 3

u8 KEY_Scan(u8 mode)
{
	static u8 key = 1;
	if (key && (k1 == 0 || k2 == 0 || k3 == 0))
	{
		delay(1000);
		key = 0;
		if (k3 == 0)
		{
			return K1_MODE;
		}
		else if (k1 == 0)
		{
			return K2_ADD;
		}
		else if (k2 == 0)
		{
			return K3_DEC;
		}
	}
	else if (k1 == 1 && k2 == 1 && k3 == 1)
	{
		key = 1;
	}
	if (mode)
	{
		key = 1;
	}
	return 0;
}

void KEY_Pros()
{
	u8 key;
	u8 temph_buf[3];

	key = KEY_Scan(0);

	if (key == K1_MODE)
	{
		mode++;
		LCD_Clear();

		if (mode == 1)
		{
			LCD_Dispstring(0, 0, "SETH:   C");
		}
		else if (mode == 2)
		{
			LCD_Dispstring(0, 1, "SETL:   C");
		}
		else
		{
			mode = 0;
			LCD_Dispstring(2, 0, "Temp:     C");
			LCD_Dispstring(0, 1, "SETH:  ");
			LCD_Dispstring(9, 1, "SETL:  ");
		}
	}
	if (mode == 1) 
	{
		switch (key)
		{
		case K2_ADD: 
			set_temph++;
			if (set_temph >= 80)
				set_temph = 80;
			break;
		case K3_DEC:
			set_temph--;
			if (set_temph <= 0)
				set_temph = 0;
			break;
		}

		temph_buf[0] = set_temph / 10 + 0x30;
		temph_buf[1] = set_temph % 10 + 0x30;
		temph_buf[2] = '\0';
		LCD_Dispstring(6, 0, temph_buf);
		At24c02Write(0, set_temph);
		
	}

	else if (mode == 2)
	{
		switch (key)
		{
		case K2_ADD:
			set_templ++;
			if (set_templ >= 80)
				set_templ = 80;
			break;
		case K3_DEC:
			set_templ--;
			if (set_templ <= 0)
				set_templ = 0;
			break;
		}

		temph_buf[0] = set_templ / 10 + 0x30;
		temph_buf[1] = set_templ % 10 + 0x30;
		temph_buf[2] = '\0';
		LCD_Dispstring(6, 1, temph_buf);
		At24c02Write(2, set_templ);
	}
}

void sound()
{
	u8 i = 50;
	while (i--)
	{
		beep = !beep;
		delay(10);
	}
}
void TempData_Compare()
{
	if (temp_val > set_temph * 10)
	{
		led = 1;
		moto = 0;
		relay = 1;
		sound();
	}
	else if (temp_val < set_templ * 10)
	{
		led = 1;
		moto = 0;
		relay = 1;
		sound();
	}
	else 
	{
		moto = 1;
		led = 0;
		relay = 0;
	}
}

void kai_display()
{
	if (At24c02Read(255) != 18)
	{
		At24c02Write(0, set_temph);
		At24c02Write(2, set_templ);
		At24c02Write(255, 18);
	}
	else
	{
		set_temph = At24c02Read(0);
		set_templ = At24c02Read(2);
	}

	LCD_Dispstring(2, 0, "Temp:     C");
	LCD_Dispstring(0, 1, "SETH:  ");
	LCD_Dispstring(9, 1, "SETL:  ");
}

void main()
{
	moto = 0;
	led = 0;
	relay = 1;

	LCD_Init();
	uart_init();
	kai_display();
	while (1)
	{
		time++;
		if (mode == 0)
		Temp_DataPros();
		KEY_Pros();
		TempData_Compare();
		if(recive_data[0]!='0')
			{
				unpackage_data(recive_data,&set_templ,&set_temph);
			}
		if(time%30000==0)
			{
				package_data(set_templ,set_temph,temp_val);
				if(time>60000){time=0;}
			}
	}
}
void UARTInterrupt(void) interrupt 4
{
	if (RI)
	{
		RI = 0;
		recive_data[position] = SBUF;
		position++;
		if (SBUF == '}')
		{
			recive_data[position] = '\0';
			position = 0;
		}
	}
}
