#include <common.h>
#include <init.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <serial.h>
#include <asm/arch-a55/a55-regs.h>

#include <cpu_func.h>
#include <debug_uart.h>
#include <env.h>
#include <log.h>
#include <net.h>
#include <sata.h>
#include <ahci.h>
#include <scsi.h>
#include <wdt.h>
#include <config.h>


DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
    return 0;
}

int print_cpuinfo(void)
{
   printf("x6_a55 print_cpuinfo\n");
   return 0;
}

ulong get_tbclk(void)
{
  return CONFIG_SYS_HZ;
}

void reset_cpu(ulong ignored)
{
  return ;
}

int dram_init(void)
{
  gd->ram_size = PHYS_SDRAM_0_SIZE;
  return 0;
 }

 /************串口配置*************/ 
int Serial_start(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;
    
	CCM->CSCDR1 &= ~(0x7f);  /* pll3_80m clock, div 1*/
	 
	SW_MUX_UART1_TX = 0x0;    /*TXD IO init*/
	SW_PAD_UART1_TX = 0x10B0; 
	 
	SW_MUX_UART1_RX = 0x0;    /*RXD IO init*/
	SW_PAD_UART1_RX = 0x10B0;
	UART1_RX_DATA_SELECT_INPUT = 0x3;	/*Selecting Pad: UART1_RX_DATA*/
	 
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
	UART1->UTXD = 'a' & 0XFF; 			  
	return 0;
}

int Serial_stop(void)
{
	return 0;
}

void Serial_setbrg(void)
{
	return ;
}

int Serial_getc(void)
{
	while((UART1->USR2 & 0x1) == 0);
	return UART1->URXD;				
}

int Serial_tstc(void)
{
	return 0;
}

void uart_putc(char c)
{
	while(((UART1->USR2 >> 3) &0X01) == 0);/* 等待上一次发送完成 */
	UART1->UTXD = c & 0XFF; 			   /* 发送数据 */
}

void Serial_putc(const char c)
{
	if (c == '\n') {
		uart_putc('\r');
	}	
	uart_putc(c);
}

void Serial_puts(const char *s)
{
	const char *p = s;

	while(*p) {
		if (*p == '\n') {
			uart_putc('\r');
		}
		uart_putc(*p++);
	}
 }

struct serial_device x6_a55_serial_device = {
	"x6_a55_serial",
	Serial_start,
   	Serial_stop,
   	Serial_setbrg,
   	Serial_getc,
   	Serial_tstc,
   	Serial_putc,
   	Serial_puts,
   	NULL,
};
 
 struct serial_device *default_serial_console(void)
 {
	return &x6_a55_serial_device;
 }
 
 /*****************************不一定用的***************************/
 unsigned long get_timer_masked(void)
 {
	 return 0;
 }
 unsigned long long get_ticks(void)
 {
	 return 0;
 }
 unsigned long get_timer(unsigned long base)
 {
	 return 0;
 }


