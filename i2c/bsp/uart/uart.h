#ifndef __UART__
#define __UART__

extern void uart1_init(void);
extern unsigned char uart_getc(void);
extern void uart_putc(char c);
extern void uart_puts(char *str);
extern int getUartReceiveBufLenTimeout(unsigned char *buf, int len);

#endif
