
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