#include "int.h"
#include "imx6ul.h"
#include "printf.h"
/*
 * @description	: 短时间延时函数
 * @param - n	: 要延时循环次数(空操作循环次数，模式延时)
 * @return 		: 无
 */
void delay_short(volatile unsigned int n)
{
	while(n--){}
}

/*
 * @description	: 延时函数,在396Mhz的主频下
 * 			  	  延时时间大约为1ms
 * @param - n	: 要延时的ms数
 * @return 		: 无
 */
void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}

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

void clk_init(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;
}

void led_init(void)
{
	SW_MUX_GPIO1_IO03 = 0x5;	
	SW_PAD_GPIO1_IO03 = 0X10B0;		
	GPIO1->GDIR |= (1 << 3);	 
	GPIO1->DR &= ~(1 << 3);	
}

void key_init(void)
{
	SW_MUX_GPIO1_IO18 = 0x5;	/* 复用为GPIO1_IO18 */
	SW_PAD_GPIO1_IO18 = 0xf080; 
	GPIO1->GDIR &= ~(1 << 18);	
}

void gpio1_io18_irqhandler(uint32_t giccIar, void *param)
{
	static unsigned char state = 0;
	delay(10);
	if ( (GPIO1->DR & (1 << 18)) == 0) {
		state  = !state;
		if (state) {
			GPIO1->DR &= ~(1 << 3);
		}else {
			GPIO1->DR|= (1 << 3);
		}
	}
	GPIO1->ISR |= (1 << 18); /*Clear the interrupt sign*/
}
void exit_init(void)
{
	GPIO1->ICR2 |= (3U << 4);  /*interrupt trigger mode*/
	GIC_EnableIRQ(99);  /*Enalbe the corresponding breaks in the gic*/
	system_register_irqhandler(99, gpio1_io18_irqhandler, NULL); /*register the interrupt service function*/
	GPIO1->IMR |= (1 << 18);  /*enable GPIO1-IO18 interrupt function*/
}
int main(void) 
{
	char a;
	int_init();
	clk_init();
	led_init();
	key_init();
	uart1_init();
	exit_init();
	printf(" Init OK %s %d\r\n",__func__,__LINE__);
	while(1) {
		printf("char input:");
		a=uart_getc();
		printf("%c\r\n",a);	//回显功能

		//显示输入的字符
		printf("char is:");
		printf("%c\r\n\r\n",a);
	}
	return 0;
}

