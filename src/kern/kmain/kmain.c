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
		
		//NVIC priority bits will now be the upper four bits
		SCB->AIRCR |=(3<<8);
		
		kprintf((uint8_t*)"%s",(uint8_t*)"Testing set and get priority for USART\n");
		//Testing __NVIC_SetPriority , we set the USART_IRQn to have priority 1
		__NVIC_SetPriority(USART2_IRQn, 1);
		//Testing __NVIC_GetPriority, we check to see if USART_IRQn has priority 1
		uint32_t ans = __NVIC_GetPriority(USART2_IRQn);
		kprintf((uint8_t*)"%s",(uint8_t*)"USART2_IRQn Set priority is: ");
		kprintf((uint8_t*)"%d", (uint8_t*)&ans);
		endl;

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");
		

		kprintf((uint8_t*)"%s",(uint8_t*)"Testing enable priority of USART\n");
		//Testing Enable and Disable. We first enable two interrupts
		__NVIC_EnableIRQn(EXTI0_IRQn);	
		__NVIC_EnableIRQn(USART2_IRQn);
		//We also implemented a method that checks if an interrupt is enabled
		if(isEnabled(USART2_IRQn) == 1)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"USART2_IRQn Interrupt is enabled\n");
		}
		else  kprintf((uint8_t*)"%s",(uint8_t*)"USART2_IRQn Interrupt is disabled\n");
		
		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");




		kprintf((uint8_t*)"%s",(uint8_t*)"Testing disable priority of USART\n");
		//Now to test __NVIC_DisableIRQn
		__NVIC_DisableIRQn(USART2_IRQn);

		if(isEnabled(USART2_IRQn) == 1)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"USART2_IRQn Interrupt is enabled\n");
		}
		else  kprintf((uint8_t*)"%s",(uint8_t*)"USART2_IRQn Interrupt is disabled\n");

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");




		kprintf((uint8_t*)"%s",(uint8_t*)"Testing disable_irq with __get_primask as well\n");

		//Testing __disable_irq which disables all interrupts and exceptions that can have configurable priority
		__disable_irq();
		//This itself should set the primask to be non-0
		//This also tests the __get_PRIMASK
		int primsk = __get_PRIMASK();

		if(primsk != 0)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"All Exceptions with configurable priority is disabled\n");
		}
		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");


		kprintf((uint8_t*)"%s",(uint8_t*)"Testing enable_irq with __get_primask as well\n");
		//Testing __enable_irq 
		//This should set the PRIMASK 
		__enable_irq();

		primsk = __get_PRIMASK();
		if(primsk != 0)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"All Exceptions with configurable priority is disabled\n");
		}
		else kprintf((uint8_t*)"%s",(uint8_t*)"All Exceptions with configurable priority is enabled\n");
		
		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");


		kprintf((uint8_t*)"%s",(uint8_t*)"Testing set_primask with __get_primask as well\n");
		//Now testing if manually setting primask works
		__set_PRIMASK(1);

		primsk = __get_PRIMASK();
		if(primsk != 0)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"All Exceptions with configurable priority is disabled\n");
		}
		else kprintf((uint8_t*)"%s",(uint8_t*)"All Exceptions with configurable priority is enabled\n");
		//To check if a software triggeered interrupt works while all the interrupts are disabled
		//This function should not yeild the print function defined for EXTI0_Handler
		kprintf((uint8_t*)"%s",(uint8_t*)"Following line should not print: Interrupt triggered\n");
		NVIC->STIR = EXTI0_IRQn;
		//We clear pending since we do not want it to run after we re-enabled it
		__NVIC_ClearPendingIRQ(EXTI0_IRQn);
		__enable_irq();

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");


		kprintf((uint8_t*)"%s",(uint8_t*)"Testing if __NVIC_EnableIRQn works for system exceptions and __NVIC_getActive\n");
		//Testing set and get priority <0
		__NVIC_EnableIRQn(SVCall_IRQn);
		__NVIC_SetPriority(SVCall_IRQn,3);
		ans = __NVIC_GetPriority(SVCall_IRQn);
		kprintf((uint8_t*)"%s",(uint8_t*)"Set priority for SVCall_IRQn is: ");
		kprintf((uint8_t*)"%d", (uint8_t*)&ans);
		endl;

		ans = __NVIC_GetActive(USART2_IRQn);
		if(ans == 1)kprintf((uint8_t*)"%s",(uint8_t*)"USART2_IRQn status active\n ");
		else kprintf((uint8_t*)"%s",(uint8_t*)"USART2_IRQn status inactive \n");

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");

		kprintf((uint8_t*)"%s",(uint8_t*)"Testing __set_BASEPRI and check with __get_BASEPRI and having EXTI0_IRQn with lower priority than basepri\n");
		//Testing to see if basepri works
		//Firstly, any priority number given to EXTI0 will not matter, if we trigger
		//a software triggerd interrupt, it will call the function
		__NVIC_SetPriority(EXTI0_IRQn,5);
		ans = __NVIC_GetPriority(EXTI0_IRQn);
		kprintf((uint8_t*)"%s",(uint8_t*)"Set priority for EXTI0_IRQn is: ");
		kprintf((uint8_t*)"%d", (uint8_t*)&ans);
		endl;
		//here we test that
		kprintf((uint8_t*)"%s",(uint8_t*)"The following line should say: interrupt triggered\n");
		NVIC->STIR = EXTI0_IRQn;

		//Now if we set the basepri to be 3, then [3->higher values everyone will be masked]
		//Testing if __set_BASEPRI works and if __get_BASEPRI works
		__set_BASEPRI(3<<4);
		ans = __get_BASEPRI();
		kprintf((uint8_t*)"%s",(uint8_t*)"BASEPRI set to: ");
		kprintf((uint8_t*)"%d", (uint8_t*)&ans);
		endl;

		//this should not be triggered. as the basepri should mask it
		kprintf((uint8_t*)"%s",(uint8_t*)"The following line should not say: interrupt triggered\n");
		NVIC->STIR = EXTI0_IRQn;
		//Testing __NVIC_GetPendingIRQn , since we know that after masking
		//EXT01_IRQn is now pending, calling this method should return 1
		//and it should print that it is pending

		kprintf((uint8_t*)"%s",(uint8_t*)"The following tests __NVIC_GetPendingIRQn, it should print that it's pending\n");
		if(__NVIC_GetPendingIRQ(EXTI0_IRQn) == 1)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"EXTI0_IRQn is currently in pending\n");
		}
		__NVIC_ClearPendingIRQ(EXTI0_IRQn);

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");


		kprintf((uint8_t*)"%s",(uint8_t*)"Testing __unset_BASEPRI\n");
		//Test __unset_basepri which clears the basepri 
		__unset_BASEPRI();
		//printing if basepri is now 0
		ans = __get_BASEPRI();
		kprintf((uint8_t*)"%s",(uint8_t*)"BASEPRI set to: ");
		kprintf((uint8_t*)"%d", (uint8_t*)&ans);
		endl;

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");


		kprintf((uint8_t*)"%s",(uint8_t*)"Testing __disable_fault_irq with __get_FAULTMASK since fault mask is set when disabled\n");
		//testing disable_fault_irq 
		//This sets the fault mask to 1
		//So we can also test __get_FAULTMASK
		__disable_fault_irq();
		if(__get_FAULTMASK() == 1)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"Disabled all interrupts except NMI's\n");
		}
		//This should not get triggered
		NVIC->STIR = EXTI0_IRQn;
		__NVIC_ClearPendingIRQ(EXTI0_IRQn);
		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");


		kprintf((uint8_t*)"%s",(uint8_t*)"Testing __enable_fault_irq with __get_FAULTMASK since fault mask is unset when enabled\n");
		//Testing enable_fault_irq
		//Now it should call the function after software trigger
		__enable_fault_irq();
		kprintf((uint8_t*)"%s",(uint8_t*)"The following line should say: interrupt triggered\n");
		NVIC->STIR = EXTI0_IRQn;

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");

		kprintf((uint8_t*)"%s",(uint8_t*)"Testing __set_FAULTASK with __get_FAULTMASK since fault mask is unset when enabled\n");
		//testing manually setting fault mask to 1
		__set_FAULTMASK(1);
		//Testing if value is correct
		if(__get_FAULTMASK() == 1)
		{
			kprintf((uint8_t*)"%s",(uint8_t*)"Disabled all interrupts except NMI's\n");
		}
		//testing if this gets triggered
		kprintf((uint8_t*)"%s",(uint8_t*)"The following line should not say: interrupt triggered\n");
		NVIC->STIR = EXTI0_IRQn;
		__NVIC_ClearPendingIRQ(EXTI0_IRQn);

		//Enable again
		//Test again
		__enable_fault_irq();
		kprintf((uint8_t*)"%s",(uint8_t*)"The following line should not say: interrupt triggered\n");
		NVIC->STIR = EXTI0_IRQn;

		kprintf((uint8_t*)"%s",(uint8_t*)"#####################################\n\n");

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

