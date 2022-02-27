#ifndef __IMX__
#define __IMX__

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#define     __I     volatile const      /* 只读 */
#define     __O     volatile            /* 只写 */
#define     __IO    volatile            /* 读写 */

/* 
 * IOMUX相关寄存器地址 
 */
#define SW_MUX_GPIO1_IO03 	*((volatile unsigned int *)0X020E0068U)
#define SW_PAD_GPIO1_IO03 	*((volatile unsigned int *)0X020E02F4U)

#define SW_MUX_GPIO1_IO18   *((volatile unsigned int *)0X020E008cU)                     
#define SW_PAD_GPIO1_IO18   *((volatile unsigned int *)0X020E0318U)  

#define SW_MUX_UART1_TX 	*((volatile unsigned int *)0x020E0084U)
#define SW_MUX_UART1_RX		*((volatile unsigned int *)0x020E0088U)

#define SW_PAD_UART1_TX		*((volatile unsigned int *)0x020E0310U)
#define SW_PAD_UART1_RX		*((volatile unsigned int *)0x020E0314U)

#define UART1_RX_DATA_SELECT_INPUT *((volatile unsigned int *)0x020e0624)

/** CCM - Register Layout Typedef */
typedef struct {
  __IO uint32_t CCR;                               /**< CCM Control Register, offset: 0x0 */
  __IO uint32_t CCDR;                              /**< CCM Control Divider Register, offset: 0x4 */
  __I  uint32_t CSR;                               /**< CCM Status Register, offset: 0x8 */
  __IO uint32_t CCSR;                              /**< CCM Clock Switcher Register, offset: 0xC */
  __IO uint32_t CACRR;                             /**< CCM Arm Clock Root Register, offset: 0x10 */
  __IO uint32_t CBCDR;                             /**< CCM Bus Clock Divider Register, offset: 0x14 */
  __IO uint32_t CBCMR;                             /**< CCM Bus Clock Multiplexer Register, offset: 0x18 */
  __IO uint32_t CSCMR1;                            /**< CCM Serial Clock Multiplexer Register 1, offset: 0x1C */
  __IO uint32_t CSCMR2;                            /**< CCM Serial Clock Multiplexer Register 2, offset: 0x20 */
  __IO uint32_t CSCDR1;                            /**< CCM Serial Clock Divider Register 1, offset: 0x24 */
  __IO uint32_t CS1CDR;                            /**< CCM SAI1 Clock Divider Register, offset: 0x28 */
  __IO uint32_t CS2CDR;                            /**< CCM SAI2 Clock Divider Register, offset: 0x2C */
  __IO uint32_t CDCDR;                             /**< CCM D1 Clock Divider Register, offset: 0x30 */
  __IO uint32_t CHSCCDR;                           /**< CCM HSC Clock Divider Register, offset: 0x34 */
  __IO uint32_t CSCDR2;                            /**< CCM Serial Clock Divider Register 2, offset: 0x38 */
  __IO uint32_t CSCDR3;                            /**< CCM Serial Clock Divider Register 3, offset: 0x3C */
       uint8_t RESERVED_0[8];
  __I  uint32_t CDHIPR;                            /**< CCM Divider Handshake In-Process Register, offset: 0x48 */
       uint8_t RESERVED_1[8];
  __IO uint32_t CLPCR;                             /**< CCM Low Power Control Register, offset: 0x54 */
  __IO uint32_t CISR;                              /**< CCM Interrupt Status Register, offset: 0x58 */
  __IO uint32_t CIMR;                              /**< CCM Interrupt Mask Register, offset: 0x5C */
  __IO uint32_t CCOSR;                             /**< CCM Clock Output Source Register, offset: 0x60 */
  __IO uint32_t CGPR;                              /**< CCM General Purpose Register, offset: 0x64 */
  __IO uint32_t CCGR0;                             /**< CCM Clock Gating Register 0, offset: 0x68 */
  __IO uint32_t CCGR1;                             /**< CCM Clock Gating Register 1, offset: 0x6C */
  __IO uint32_t CCGR2;                             /**< CCM Clock Gating Register 2, offset: 0x70 */
  __IO uint32_t CCGR3;                             /**< CCM Clock Gating Register 3, offset: 0x74 */
  __IO uint32_t CCGR4;                             /**< CCM Clock Gating Register 4, offset: 0x78 */
  __IO uint32_t CCGR5;                             /**< CCM Clock Gating Register 5, offset: 0x7C */
  __IO uint32_t CCGR6;                             /**< CCM Clock Gating Register 6, offset: 0x80 */
       uint8_t RESERVED_2[4];
  __IO uint32_t CMEOR;                             /**< CCM Module Enable Overide Register, offset: 0x88 */
} CCM_Type;

/** GPIO - Register Layout Typedef */
typedef struct {
  __IO uint32_t DR;                                /**< GPIO data register, offset: 0x0 */
  __IO uint32_t GDIR;                              /**< GPIO direction register, offset: 0x4 */
  __I  uint32_t PSR;                               /**< GPIO pad status register, offset: 0x8 */
  __IO uint32_t ICR1;                              /**< GPIO interrupt configuration register1, offset: 0xC */
  __IO uint32_t ICR2;                              /**< GPIO interrupt configuration register2, offset: 0x10 */
  __IO uint32_t IMR;                               /**< GPIO interrupt mask register, offset: 0x14 */
  __IO uint32_t ISR;                               /**< GPIO interrupt status register, offset: 0x18 */
  __IO uint32_t EDGE_SEL;                          /**< GPIO edge select register, offset: 0x1C */
} GPIO_Type;

/** UART - Register Layout Typedef */
typedef struct {
  __I  uint32_t URXD;                              /**< UART Receiver Register, offset: 0x0 */
       uint8_t RESERVED_0[60];
  __IO uint32_t UTXD;                              /**< UART Transmitter Register, offset: 0x40 */
       uint8_t RESERVED_1[60];
  __IO uint32_t UCR1;                              /**< UART Control Register 1, offset: 0x80 */
  __IO uint32_t UCR2;                              /**< UART Control Register 2, offset: 0x84 */
  __IO uint32_t UCR3;                              /**< UART Control Register 3, offset: 0x88 */
  __IO uint32_t UCR4;                              /**< UART Control Register 4, offset: 0x8C */
  __IO uint32_t UFCR;                              /**< UART FIFO Control Register, offset: 0x90 */
  __IO uint32_t USR1;                              /**< UART Status Register 1, offset: 0x94 */
  __IO uint32_t USR2;                              /**< UART Status Register 2, offset: 0x98 */
  __IO uint32_t UESC;                              /**< UART Escape Character Register, offset: 0x9C */
  __IO uint32_t UTIM;                              /**< UART Escape Timer Register, offset: 0xA0 */
  __IO uint32_t UBIR;                              /**< UART BRM Incremental Register, offset: 0xA4 */
  __IO uint32_t UBMR;                              /**< UART BRM Modulator Register, offset: 0xA8 */
  __I  uint32_t UBRC;                              /**< UART Baud Rate Count Register, offset: 0xAC */
  __IO uint32_t ONEMS;                             /**< UART One Millisecond Register, offset: 0xB0 */
  __IO uint32_t UTS;                               /**< UART Test Register, offset: 0xB4 */
  __IO uint32_t UMCR;                              /**< UART RS-485 Mode Control Register, offset: 0xB8 */
} UART_Type;

typedef struct {
  __IO uint16_t IADR;                              /**< I2C Address Register, offset: 0x0 */
       uint8_t RESERVED_0[2];
  __IO uint16_t IFDR;                              /**< I2C Frequency Divider Register, offset: 0x4 */
       uint8_t RESERVED_1[2];
  __IO uint16_t I2CR;                              /**< I2C Control Register, offset: 0x8 */
       uint8_t RESERVED_2[2];
  __IO uint16_t I2SR;                              /**< I2C Status Register, offset: 0xC */
       uint8_t RESERVED_3[2];
  __IO uint16_t I2DR;                              /**< I2C Data I/O Register, offset: 0x10 */
} I2C_Type;

/* CCM - Peripheral instance base addresses */
/** Peripheral CCM base address */
#define CCM_BASE                                 (0x20C4000u)
/** Peripheral CCM base pointer */
#define CCM                                      ((CCM_Type *) CCM_BASE)


/* GPIO - Peripheral instance base addresses */
/** Peripheral GPIO1 base address */
#define GPIO1_BASE                               (0x209C000u)
/** Peripheral GPIO1 base pointer */
#define GPIO1                                    ((GPIO_Type *)GPIO1_BASE)
/** Peripheral GPIO2 base address */
#define GPIO2_BASE                               (0x20A0000u)
/** Peripheral GPIO2 base pointer */
#define GPIO2                                    ((GPIO_Type *)GPIO2_BASE)
/** Peripheral GPIO3 base address */
#define GPIO3_BASE                               (0x20A4000u)
/** Peripheral GPIO3 base pointer */
#define GPIO3                                    ((GPIO_Type *)GPIO3_BASE)
/** Peripheral GPIO4 base address */
#define GPIO4_BASE                               (0x20A8000u)
/** Peripheral GPIO4 base pointer */
#define GPIO4                                    ((GPIO_Type *)GPIO4_BASE)
/** Peripheral GPIO5 base address */
#define GPIO5_BASE                               (0x20AC000u)
/** Peripheral GPIO5 base pointer */
#define GPIO5                                    ((GPIO_Type *)GPIO5_BASE)
/** Array initializer of GPIO peripheral base addresses */
#define GPIO_BASE_ADDRS                          { 0u, GPIO1_BASE, GPIO2_BASE, GPIO3_BASE, GPIO4_BASE, GPIO5_BASE }

/* UART - Peripheral instance base addresses */
/** Peripheral UART1 base address */
#define UART1_BASE                               (0x2020000u)
/** Peripheral UART1 base pointer */
#define UART1                                    ((UART_Type *)UART1_BASE)
/** Peripheral UART2 base address */
#define UART2_BASE                               (0x21E8000u)
/** Peripheral UART2 base pointer */
#define UART2                                    ((UART_Type *)UART2_BASE)
/** Peripheral UART3 base address */
#define UART3_BASE                               (0x21EC000u)
/** Peripheral UART3 base pointer */
#define UART3                                    ((UART_Type *)UART3_BASE)
/** Peripheral UART4 base address */
#define UART4_BASE                               (0x21F0000u)
/** Peripheral UART4 base pointer */
#define UART4                                    ((UART_Type *)UART4_BASE)
/** Peripheral UART5 base address */
#define UART5_BASE                               (0x21F4000u)
/** Peripheral UART5 base pointer */
#define UART5                                    ((UART_Type *)UART5_BASE)
/** Peripheral UART6 base address */
#define UART6_BASE                               (0x21FC000u)
/** Peripheral UART6 base pointer */
#define UART6                                    ((UART_Type *)UART6_BASE)
/** Peripheral UART7 base address */
#define UART7_BASE                               (0x2018000u)
/** Peripheral UART7 base pointer */
#define UART7                                    ((UART_Type *)UART7_BASE)
/** Peripheral UART8 base address */
#define UART8_BASE                               (0x2288000u)
/** Peripheral UART8 base pointer */
#define UART8                                    ((UART_Type *)UART8_BASE)

#define I2C1_BASE                                (0x21A0000u)
#define I2C1                                     ((I2C_Type *)I2C1_BASE)

#endif





