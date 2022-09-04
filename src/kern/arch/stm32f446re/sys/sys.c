//void sysTickInit(uint32_t loadVal){} to configure the systick, mainly the CTRL register, LOAD register = loadVal
//Have a default setting in there as well

//First work is to disable the systick.
    //0 th bit is enable, so setting it to 0 is disable
    //2 nd bit is clck source 1 for core, 0 for extern
    //16 cnt flag READONLY
    //This is in the ctrl reg
//Second work is  to modify LOAD reg
    //[23-0] per second it counts 180 million cycles
//third VAL , [23-0] make it equal to 0
//fourth step is enable the systick


//sysTick_enable(void)
    //CTRL reg's 0th bit |= 1 is enabling.

//sysTick_disable(void)
    //CTRL reg's 0th bit &= ~(1) is disabling

//update_reloadValue(uint32_t)
    //disable first
    //Modify the load register.
    //enable systick

//uint32_t get_counterValue() // returns systick val

//uint32_t getTickCount() //returns tickCountb           
/*
*/
#include "../include/sys/sys.h"
#include <stdint.h>
//Method that configures the systick
//Load value 
volatile uint32_t mscount;
//This method initializes the sysTick
void sysTick_init(uint32_t loadVal)
{
    //First disable the Systick
    sysTick_disable();
    //Set the clock source as the main AHB clock
    SYSTICK->CTRL |= SYSTICK_CTRL_CLKSOURCE_AHB;
    SYSTICK->CTRL |= SYSTICK_CTRL_TICKINT;
    //do math to figure out value for load from loadVal
    updateSysTick(loadVal);
    sysTick_enable();
}
//This enables the systick timer
void sysTick_enable()
{
    if(!(SYSTICK->CTRL & SYSTICK_CTRL_ENABLE))
    {
        mscount = 0;
    }
    SYSTICK->CTRL |= SYSTICK_CTRL_ENABLE;
}
//Disables the systick timer
void sysTick_disable()
{
    SYSTICK->CTRL &= SYSTICK_CTRL_DISABLE;
}
//Updates the systick. First disable it, change the load value
void updateSysTick(uint32_t val)
{
    sysTick_disable();
    SYSTICK->VAL= 0;
    SYSTICK->LOAD |= (val<<0);
}
//see the systick value
uint32_t getSysTickCount(void)
{
    return SYSTICK->VAL;
}
//See the total time consumed
uint32_t getTime(void)
{
    return mscount + (int)(((SYSTICK_LOAD_VAL_10MS - SYSTICK->VAL)/SYSTICK_LOAD_VAL_10MS)*10);
}
//This method gets invoked everytime the Systick handler gets called

void update_msCount()
{
    mscount += PER_TICK_TIME_DELAY_MS;
}

void SysTick_Handler(void)
{
	uint32_t *pICSR = (uint32_t*)0xE000ED04;
    *pICSR |= ( 1 << 26);
    // kprintf((uint8_t*)"%s",(uint8_t*)"SysTick_Handler before death\n\r");
	//This is the only code added to the previous code
    update_msCount();
        // uint32_t mscount = get_mscount();
        // kprintf((uint8_t*)"%s",(uint8_t*)"ms count: \n\r");
        // kprintf((uint8_t*)"%d",(uint8_t*) &mscount);
        // kprintf((uint8_t*)"%s",(uint8_t*)"\n\r");

    
    uint32_t a = (uint32_t)(SYSTICK->CTRL);
    // kprintf((uint8_t*)"%s",(uint8_t*)"SysTick_Handler after? death\n\r");
    

    // kprintf((uint8_t*)"%s",(uint8_t*)"SysTick_Handler after? death2\n\r");
    update_global_tick_count();

    // kprintf((uint8_t*)"%s",(uint8_t*)"SysTick_Handler after? death3\n\r");

    //unblock_tasks();

    //pend the pendsv exception
    *pICSR |= ( 1 << 25);
    // kprintf((uint8_t*)"%s",(uint8_t*)"SysTick_Handler after? death4\n\r");
}

void __NVIC_SetPriority(IRQn_TypeDef IRQn, uint32_t priority)
{
    int ipr = IRQn/4;
    int byteOffset = IRQn % 4;
    NVIC->IP[ipr] |= (priority << byteOffset);
}
uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn)
{
    int ipr = IRQn/4;
    int byteOffset = IRQn % 4;
    return ((NVIC->IP[ipr]) & ( 255U<< byteOffset));
}
void __NVIC_EnableIRQn(IRQn_TypeDef IRQn)
{
    int iser = IRQn/32;
    int offset = IRQn%8;
    NVIC->ISER[iser] |= (1 << offset);
}