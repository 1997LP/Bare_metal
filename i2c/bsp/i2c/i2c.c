#include "i2c.h"

void i2c_init(I2C_Type *base)
{
	base->I2CR &= ~(1<<7); /*Disable I2C*/
	base->IFDR = 0x15;  /*baterate*/
	base->I2CR |= (1<<7);  /*enable I2C*/
}

unsigned char i2c_master_start(I2C_Type *base, unsigned char address, enum i2c_direction direction)
{
	if (base->I2SR & (1<<5)) { /*I2C busy*/
		return 1;
	}

	base->I2CR |= (1<<5) | (1<<4); /*��ģʽ������*/

	base->I2DR = ((unsigned int)address << 1) | ((direction == kI2C_Read)? 1 : 0);
	return 0;
}

unsigned char i2c_master_repeated_start(I2C_Type *base, unsigned char address,  enum i2c_direction direction)
{
	if(base->I2SR & (1 << 5) && (((base->I2CR) & (1 << 5)) == 0))		
		return 1;
		
	base->I2CR |=  (1 << 4) | (1 << 2);
	
	base->I2DR = ((unsigned int)address << 1) | ((direction == kI2C_Read)? 1 : 0);

	return 0;
}

unsigned char i2c_check_and_clear_error(I2C_Type *base, unsigned int status)
{
		/* ����Ƿ����ٲö�ʧ���� */
	if(status & (1<<4))
	{
		base->I2SR &= ~(1<<4);		/* ����ٲö�ʧ����λ 			*/

		base->I2CR &= ~(1 << 7);	/* �ȹر�I2C 				*/
		base->I2CR |= (1 << 7);		/* ���´�I2C 				*/
		return I2C_STATUS_ARBITRATIONLOST;
	} 
	else if(status & (1 << 0))     	/* û�н��յ��ӻ���Ӧ���ź� */
	{
		return I2C_STATUS_NAK;		/* ����NAK(No acknowledge) */
	}
	return I2C_STATUS_OK;
}

unsigned char i2c_master_stop(I2C_Type *base)
{
	unsigned short timeout = 0xffff;

	base->I2CR &= ~( (1<<5) | (1<<4) | (1<<3));

	/* �ȴ�æ���� */
	while((base->I2SR & (1 << 5)))
	{
		timeout--;
		if(timeout == 0)	/* ��ʱ���� */
			return I2C_STATUS_TIMEOUT;
	}
	return I2C_STATUS_OK;
}

void i2c_master_read(I2C_Type *base, unsigned char *buf, unsigned int size)
{
	 �ȴ�������� */
	while(!(base->I2SR & (1 << 7))); 
	
	base->I2SR &= ~(1 << 1); 				/*����жϹ���λ*/
	base->I2CR &= ~((1 << 4) | (1 << 3));	/*�������� */
	
	/* ���ֻ����һ���ֽ����ݵĻ�����NACK�ź� */
	if(size == 1)
        base->I2CR |= (1 << 3);

	dummy = base->I2DR; /* �ٶ� */
	
	while(size--)
	{
		while(!(base->I2SR & (1 << 1))); 	/* �ȴ�������� */	
		base->I2SR &= ~(1 << 1);			/* �����־λ */

	 	if(size == 0)
        {
        	i2c_master_stop(base); 			/* ����ֹͣ�ź� */
        }

        if(size == 1)
        {
            base->I2CR |= (1 << 3);
        }
		*buf++ = base->I2DR;
	}
}

void i2c_master_write(I2C_Type *base, const unsigned char *buf, unsigned int size)
{
		/* �ȴ�������� */
	while(!(base->I2SR & (1 << 7))); 
	
	base->I2SR &= ~(1 << 1); 	/* �����־λ */
	base->I2CR |= 1 << 4;		/* �������� */
	
	while(size--)
	{
		base->I2DR = *buf++; 	/* ��buf�е�����д�뵽I2DR�Ĵ��� */
		
		while(!(base->I2SR & (1 << 1))); 	/* �ȴ�������� */	
		base->I2SR &= ~(1 << 1);			/* �����־λ */

		/* ���ACK */
		if(i2c_check_and_clear_error(base, base->I2SR))
			break;
	}
	
	base->I2SR &= ~(1 << 1);
	i2c_master_stop(base); 	/* ����ֹͣ�ź� */
}

unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer)
{
	unsigned char ret = 0;
	enum i2c_direction direction = xfer->direction;	

	base->I2SR &= ~((1 << 1) | (1 << 4));			/* �����־λ */

	/* �ȴ�������� */
	while(!((base->I2SR >> 7) & 0X1)){}; 

	/* ����Ƕ��Ļ���Ҫ�ȷ��ͼĴ�����ַ������Ҫ�Ƚ������Ϊд */
    if ((xfer->subaddressSize > 0) && (xfer->direction == kI2C_Read))
    {
        direction = kI2C_Write;
    }

	ret = i2c_master_start(base, xfer->slaveAddress, direction); /* ���Ϳ�ʼ�ź� */
    if(ret)
    {	
		return ret;
	}

	while(!(base->I2SR & (1 << 1))){};			/* �ȴ�������� */

    ret = i2c_check_and_clear_error(base, base->I2SR);	/* ����Ƿ���ִ������ */
    if(ret)
    {
      	i2c_master_stop(base); 						/* ���ͳ�������ֹͣ�ź� */
        return ret;
    }
	
    /* ���ͼĴ�����ַ */
    if(xfer->subaddressSize)
    {
        do
        {
			base->I2SR &= ~(1 << 1);			/* �����־λ */
            xfer->subaddressSize--;				/* ��ַ���ȼ�һ */
			
            base->I2DR =  ((xfer->subaddress) >> (8 * xfer->subaddressSize)); //��I2DR�Ĵ���д���ӵ�ַ
  
			while(!(base->I2SR & (1 << 1)));  	/* �ȴ�������� */

            /* ����Ƿ��д����� */
            ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	i2c_master_stop(base); 				/* ����ֹͣ�ź� */
             	return ret;
            }  
        } while ((xfer->subaddressSize > 0) && (ret == I2C_STATUS_OK));

        if(xfer->direction == kI2C_Read) 		/* ��ȡ���� */
        {
            base->I2SR &= ~(1 << 1);			/* ����жϹ���λ */
            i2c_master_repeated_start(base, xfer->slaveAddress, kI2C_Read); /* �����ظ���ʼ�źźʹӻ���ַ */
    		while(!(base->I2SR & (1 << 1))){};/* �ȴ�������� */

            /* ����Ƿ��д����� */
			ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	ret = I2C_STATUS_ADDRNAK;
                i2c_master_stop(base); 		/* ����ֹͣ�ź� */
                return ret;  
            }
           	          
        }
    }	

    /* �������� */
    if ((xfer->direction == kI2C_Write) && (xfer->dataSize > 0))
    {
    	i2c_master_write(base, xfer->data, xfer->dataSize);
	}

    /* ��ȡ���� */
    if ((xfer->direction == kI2C_Read) && (xfer->dataSize > 0))
    {
       	i2c_master_read(base, xfer->data, xfer->dataSize);
	}
	return 0;	
}
