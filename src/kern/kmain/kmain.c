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
		__NVIC_EnableIRQn(USART2_IRQn);
		//_USART_WRITE(USART2,(uint8_t*)"#######################\n");
		__NVIC_SetPriority(USART2_IRQn, 2);
		uint32_t ans = __NVIC_GetPriority(USART2_IRQn);
		kprintf((uint8_t*)"%d", (uint8_t*)&ans);
		//Testing integer
		
		//Testing time consumption, using the Systick handler

		// _USART_WRITE(USART2,(uint8_t*)"#######################\n");

		// //Systick show
		// _USART_WRITE(USART2,(uint8_t*)"SysTick Count: ");
		// val = getSysTickCount();
		// kprintf((uint8_t*)"%d",(uint8_t*)&val);
		// endl;

		// _USART_WRITE(USART2,(int8_t*)"Time before 1000000 loop: ");
		// tm1 = getTime();
		// kprintf((uint8_t*)"%d",(uint8_t*)&tm1);
		// endl;
		// int ans = 1;
		// //Using a big loop
		// for(int i=1;i<=1000000;i++)
		// {
		// 	ans++;
		// }
		// tm2 = getTime();
		// _USART_WRITE(USART2,(int8_t*)"\nTime after 1000000 loop: ");
		// kprintf((uint8_t*)"%d",(uint8_t*)&tm2);
		// endl;
		// _USART_WRITE(USART2,(int8_t*)"\nTotal time: ");
		// tm2 -= tm1;
		// kprintf((uint8_t*)"%d",(uint8_t*)&tm2);
		// endl;

		// _USART_WRITE(USART2,(int8_t*)"Time before 10000 loop: ");
		// tm1 = getTime(); 
		// kprintf((uint8_t*)"%d",(uint8_t*)&tm1);
		// ans = 1;
		// //Using a tiny loop
		// for(int i=1;i<=10000;i++)
		// {
		// 	ans++;
		// }
		// tm2 = getTime();
		// _USART_WRITE(USART2,(int8_t*)"\nTime after 10000 loop: ");
		// kprintf((uint8_t*)"%d",(uint8_t*)&tm2);
		// endl;

		// _USART_WRITE(USART2,(int8_t*)"\nTotal time: ");
		// tm2 -= tm1;
		// kprintf((uint8_t*)"%d",(uint8_t*)&tm2);
		// endl;
		// //Systick show
		// _USART_WRITE(USART2,(uint8_t*)"SysTick Count: ");
		// val = getSysTickCount();
		// kprintf((uint8_t*)"%d",(uint8_t*)&val);
		// endl;
		// //sysTick_disable();
		// _USART_WRITE(USART2,(uint8_t*)"-----------------------------#\n");

		// kprintf((uint8_t*)"%s", (uint8_t*) " ");
		// kprintf((uint8_t*)"%s", (uint8_t*) "Enter an integer, press # for end of line: ");
		// kscanf((uint8_t*) "%d", (uint8_t*) &b);
		// endl;

		// kprintf((uint8_t*)"%s", (uint8_t*) "You entered: ");
		// kprintf((uint8_t*)"%d", (uint8_t*) &b);
		// b++;
		// endl;

		// kprintf((uint8_t*)"%s", (uint8_t*) "After incrementing by 1: ");
		// kprintf((uint8_t*)"%d", (uint8_t*) &b);
		// endl;

		// //Testing hexadecimal
		// kprintf((uint8_t*)"%s", (uint8_t*) " ");
		// kprintf((uint8_t*)"%s", (uint8_t*) "Enter an hexadecimal number in the form of 0xLLLL, press # for end of line: ");
		// kscanf((uint8_t*) "%x", (uint8_t*) &b);
		// endl;

		// kprintf((uint8_t*)"%s", (uint8_t*) "You entered: ");
		// kprintf((uint8_t*)"%x", (uint8_t*) &b);
		// b++;
		// endl;

		// kprintf((uint8_t*)"%s", (uint8_t*) "After incrementing by 1: ");
		// kprintf((uint8_t*)"%x", (uint8_t*) &b);
		// endl;

		//Testing floating numbers
		kprintf((uint8_t*)"%s", (uint8_t*) " ");
		kprintf((uint8_t*)"%s", (uint8_t*) "Enter a floating point number, press # for end of line: ");
		kscanf((uint8_t*) "%f", (uint8_t*) &f);
		endl;

		kprintf((uint8_t*)"%s", (uint8_t*) "You entered: ");
		kprintf((uint8_t*)"%f", (uint8_t*) &f);
		f++;
		endl;

		kprintf((uint8_t*)"%s", (uint8_t*) "After incrementing by 1: ");
		kprintf((uint8_t*)"%f", (uint8_t*) &f);
		endl;
		
		// kprintf((uint8_t*)"%f", (uint8_t*) &x);
		

		// kprintf((uint8_t*)"%f", (uint8_t*) &x);


		// kprintf((uint8_t*)"%s", (uint8_t*) "Enter Input : ");

        // kscanf((uint8_t*) "%x", (uint8_t*) &b);

		// kprintf((uint8_t*)"%s", (uint8_t*) "Input : ");
		// kprintf((uint8_t*)"%x", (uint8_t*) &b);

		// kprintf((uint8_t*)"%s", (uint8_t*) "\n");

		// kscanf((uint8_t*)"%d",(uint8_t*) b);
		// kprintf((uint8_t*)"%d",(uint8_t*) b);
        //
		// kprintf((uint8_t*) "%c", (uint8_t*) &c);
        // kprintf((uint8_t*)"%c", &c);

		a++;
		b++;
		//you can change the following line by replacing a delay function
		for(uint32_t i=0;i<5000000;i++){}	
	}
}

