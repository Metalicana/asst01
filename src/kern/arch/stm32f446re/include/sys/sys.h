
#ifndef __SYS_H
#define __SYS_H
#include <stdint.h>
#include "../sys/stm32_peps.h"

//Copied from given source
typedef struct {
  volatile uint32_t CTRL;  // Enable SysTick
  volatile uint32_t LOAD;  // 24
  volatile uint32_t VAL;
  volatile uint32_t CALIB;
} SysTick_Typedef;
//SCB datastructure
typedef struct {
  uint32_t CPUID;
  uint32_t ICSR;
  uint32_t VTOR;
  uint32_t AIRCR;
  uint32_t SCR;
  uint32_t CCR;
  uint32_t SHPR1;
  uint32_t SHPR2;
  uint32_t SHPR3;
  uint32_t SHCSR;
  uint32_t CFSR;
  uint32_t HFSR;
  uint32_t MMAR;
  uint32_t BFAR;
  uint32_t AFSR;
  uint8_t reserved[76];
  uint32_t CPACR;
}SCB_typeDef;
//Given in the pdf , NVIC Datastructure
typedef struct
{
//define NVIC register compenenets -- use volatile data type
volatile uint32_t ISER[8]; /*!< Offset: 0x000 Interrupt Set Enable Register*/
uint32_t RESERVED0[88];
volatile uint32_t ICER[8]; /*!< Offset: 0x180 Interrupt Clear Enable Register*/
uint32_t RSERVED1[24];
volatile uint32_t ISPR[8]; /*!< Offset: 0x200 Interrupt Set Pending Register*/
uint32_t RESERVED2[88];
volatile uint32_t ICPR[8]; /*!< Offset: 0x280 Interrupt Clear Pending Register*/
uint32_t RESERVED3[24];
volatile uint32_t IABR[8]; /*!< Offset: 0x300 Interrupt Active bit Register*/
uint32_t RESERVED4[56];
volatile uint8_t IP[240]; /*!< Offset: 0x400 Interrupt Priority Register (8Bit wide) */
uint32_t RESERVED5[644];
volatile uint32_t STIR; /*!< Offset: 0xF00 Software Trigger Interrupt Register*/
}NVIC_TypeDef;



#define SYSTICK ((SysTick_Typedef*) 0xE000E010)
#define SYSTICK_CTRL_CLKSOURCE_AHB (1 << 2)
#define SYSTICK_CTRL_TICKINT (1 << 1)
#define SYSTICK_CTRL_ENABLE (1 << 0)
#define SYSTICK_CTRL_DISABLE ~(1 << 0)
#define PER_TICK_TIME_DELAY_MS 10U
//We assume we're incrementing to a 10ms difference.

#define SYSTICK_LOAD_VAL_10MS (((180 * 1000000)*(PER_TICK_TIME_DELAY_MS)/1000) - 1)
volatile static uint32_t tickCount;
void sysTick_init(uint32_t loadVal);
void sysTick_enable(void);
void sysTick_disable(void);
void updateSysTick(uint32_t val);
uint32_t getTime(void);
uint32_t getSysTickCount(void);
void update_msCount();
void SysTick_Handler(void);
#endif