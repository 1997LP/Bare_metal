#include "int.h"
#include "imx6ul.h"
#include "printf.h"
#include "uart.h"
#include "common.h"
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
	int_init();
	clk_init();
	led_init();
	key_init();
	uart1_init();
	exit_init();
	printf(" Init OK %s %d\r\n",__func__,__LINE__);
	cmdLoop();

	return 0;
}

