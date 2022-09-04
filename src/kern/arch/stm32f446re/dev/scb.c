#include "../include/dev/scb.h"

void DRV_FPU_ACTIVE(void) {
    SCB->CPACR |= ((3UL << 10*2) |  (3UL << 11*2)  );
}