#ifndef __X6_A55__INIT
#define __X6_A55__INIT


#define CONFIG_SYS_MALLOC_LEN   (4*1024*1024)
#define CONFIG_SYS_LOAD_ADDR 0x99999000

#define PHYS_SDRAM_0 		0x80000000
#define PHYS_SDRAM_0_SIZE 	512*1024*1024

#define CONFIG_SYS_SDRAM_BASE 	PHYS_SDRAM_0
#define CONFIG_SYS_INIT_SP_ADDR	0x87600000

#define CONFIG_SYS_HZ		1000

//#define CONFIG_BOARD_EARLY_INIT_F
//#define CONFIG_BOARD_LATE_INIT   

//#define CONFIG_ENV_SIZE    1024*8
//#define CONFIG_ENV_OFFSET  0xxxx /*相对于存储器首地址的偏移*/

#endif
