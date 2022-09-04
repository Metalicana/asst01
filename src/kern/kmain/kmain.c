#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"
#include <stdint.h>
#define endl kprintf((uint8_t*)"%s",(uint8_t*)"\n")
void kmain(void)
{
	__sys_init();
	uint32_t a=10240000;
	uint32_t b=0;
	float f = 43.234;
	int val=0;
	int tm1 = 0, tm2=0;
	kprintf((uint8_t*)"%s",(uint8_t*)"\n\n\n");
	kprintf((uint8_t*)"%s",(uint8_t*)"Booting duOS: ");
	kprintf((uint8_t*)"%s",(uint8_t*)"Version: 1.0\n");
	kprintf((uint8_t*)"%s",(uint8_t*)"Wellcome .... \n");

	while(1){
		
		//Test __NVIC_SetPriority
		
		//you can change the following line by replacing a delay function
		for(uint32_t i=0;i<5000000;i++){}	
	}
}

