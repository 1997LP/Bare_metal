#ifndef INT_INIT
#define INT_INIT

#include "core_ca7.h"

#define NUMBER_OF_INT_VERTORS 1000

typedef void (*system_irq_handler_ptr)(uint32_t giccIar, void *param);

typedef struct _sys_irqHandler{
	system_irq_handler_ptr irqHandler;
	void *user_param;
}sys_irq_type;


void int_init(void);
void system_register_irqhandler(uint32_t giccIar, system_irq_handler_ptr func, void *param);
void system_irqhandler(unsigned int giccIar);


#endif