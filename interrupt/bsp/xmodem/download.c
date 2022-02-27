#include "printf.h"
#include "uart.h"
#include "imx6ul.h"
#include "common.h"

#define SOH		0x01
#define	EOT		0x04
#define	ACK		0x06
#define NAK		0x15
#define	CAN		0x18
#define	CSYM	0x43

int xmodem_download(unsigned int addr)
{
	unsigned char buf[256], temp, seq;
	unsigned int i, offset;
	int result;
	int flag = 0;
	
	printf("addr: 0x%x \r\n", addr);

	/* clear uart1 receive buf */
	while(1) {
		if ((UART1->USR2 & 0x1) == 0)
			break;
		i = UART1->URXD;
	}
	printf("Rdy\r\n");
	
	/* wait for download */
	offset = 0;
	seq = 0;	
	while(1)
	{
		result = getUartReceiveBufLenTimeout(buf, 132);
		if ( result == 0 )
		{
			if ( flag == 0 )
			{
				uart_putc(NAK); /*启动传输*/
			}
			else
			{
				uart_putc(ACK);
				uart_putc(ACK);
			}				
			
			continue;
		}
		else if ( result < 132 )
		{
			if ( buf[0] == EOT )/*传输完成*/
			{
				uart_putc(ACK); /*结束传输*/
				printf("\nEND\n");
				break;		
			}
			else if ( buf[0] == CAN )
			{
				uart_putc(ACK);
				printf("CNL\n");
				break;		
			}
			else
			{
				uart_putc(ACK);
			}
			
		}
		else
		{
			if ( flag == 0 )
			{
				flag = 1;
				uart_putc(ACK); /*接收方表示接收到数据包*/
				uart_putc(ACK); /*开始下一次包发送*/
			}
			
			uart_putc(ACK);
			if ( buf[0] != SOH )/*包头校验*/
			{
				continue;
			}
		
			temp = 0;
			for ( i = 3; i < 131 ; i ++)
			{
				temp += buf[i];
			}
			if ( temp != buf[i] ) /*校验和*/
			{
				uart_putc(CAN);
				printf("err\r\n");		
				break;
			}
			else
			{
				seq ++;
				if ( seq != buf[1] )/*信息包序号*/
				{
					uart_putc(CAN); /*传输错误，无条件停止传输*/
					printf("err\r\n");		
					break;
				}
	
				memcpy((char *)(addr + offset), buf + 3, 128);
				if ( memcmp((char *)(addr + offset), (char *)buf + 3, 128) != 0 )
				{
					uart_putc(CAN);
					printf("err\r\n");		
					break;
				}
				
				offset += 128;
			}
		}
	}
	return 0;
}
