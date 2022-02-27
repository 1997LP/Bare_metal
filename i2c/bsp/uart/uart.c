#include "imx6ul.h"

void uart1_init(void)
{
	CCM->CSCDR1 &= ~(0x7f);  /* pll3_80m clock, div 1*/

	SW_MUX_UART1_TX = 0x0;    /*TXD IO init*/
	SW_PAD_UART1_TX = 0x10B0; 
	
	SW_MUX_UART1_RX = 0x0;	  /*RXD IO init*/
	SW_PAD_UART1_RX = 0x10B0;
	UART1_RX_DATA_SELECT_INPUT = 0x3;  /*Selecting Pad: UART1_RX_DATA*/

	UART1->UCR1 &= ~(1 << 0); /*Disable UART1*/

	UART1->UCR2 &= ~(1 << 0); /*Reset UART1*/	
	while( (UART1->UCR2 & 0x1) == 0); /*等待复位完成*/

	UART1->UCR1 &= ~(1<<14);  /*Disable Automatic baterate*/
	
	UART1->UCR2 |= (1<<14) | (1<<5) | (1<<2) | (1<<1);
	UART1->UCR3 |= 1<<2; 

	/*Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)) */
	UART1->UFCR = 5<<7; //ref freq等于ipg_clk/1=80Mhz
	UART1->UBIR = 71;
	UART1->UBMR = 3124;

	UART1->UCR1 |= (1 << 0); /*Enable UART1*/
}

unsigned char uart_getc(void)
{
	while((UART1->USR2 & 0x1) == 0);/* 等待接收完成 */
	return UART1->URXD;				/* 返回接收到的数据 */
}
void uart_putc(char c)
{
	while(((UART1->USR2 >> 3) &0X01) == 0);/* 等待上一次发送完成 */
	UART1->UTXD = c & 0XFF; 			   /* 发送数据 */
}

void uart_puts(char *str)
{
	char *p = str;

	while(*p)
		uart_putc(*p++);
}

int getUartReceiveBufLenTimeout(unsigned char *buf, int len)
{
	int i = 0, k = 0;
	
	while( i < len )
	{
		if (UART1->USR2 & 0x1) /*接收到数据*/
		{
			buf[i++] = (unsigned char)UART1->URXD;
			k = 0;
		}
		if ( k++ > 50000 )
			break;
	}
		
	return i;
}

