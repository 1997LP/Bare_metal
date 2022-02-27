#include "int.h"
#include "core_ca7.h"

sys_irq_type irqTable[NUMBER_OF_INT_VERTORS];

void default_irq_handler_t(uint32_t giccIar, void *param)
{
	return;
}

void irqtable_init(void)
{
	for (uint32_t i = 0; i < NUMBER_OF_INT_VERTORS; i++) {
		irqTable[i].irqHandler = default_irq_handler_t;
		irqTable[i].user_param = NULL;
	}	
}

void system_register_irqhandler(uint32_t giccIar, system_irq_handler_ptr func, void *param)
{
	irqTable[giccIar].irqHandler = func;
	irqTable[giccIar].user_param = param;
}

void system_irqhandler(unsigned int giccIar)
{
	uint32_t int_num = giccIar & 0x3FFUL;

	if ( int_num == 1023 || int_num >= NUMBER_OF_INT_VERTORS) {
		return;
	}

	irqTable[int_num].irqHandler(int_num, irqTable[int_num].user_param);	
}

void int_init(void)
{
	GIC_Init();
	irqtable_init();
	__set_VBAR((uint32_t)0x87800000);
}


