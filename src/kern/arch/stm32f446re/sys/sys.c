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
//Tested but TODO handle for negative IRQs
void __NVIC_SetPriority(IRQn_TypeDef IRQn, uint32_t priority)
{
    //The way this works is, if someone performs  x & with 15 where x is negative,
    //x = -14 . x&15 =  2, 2-4 is negative, 
    //x = -13 has the same issue
    //However increasing the number to the extreme -1 , x & 15 = 15 , and 15-4 is 11, which is the last index of our SHP register
    // the shifting 5 bits, and & with 255 has periodic property 
    // like 7,15, 31 will result in 224
    if(IRQn < 0)
    {
        int shp = (IRQn & 15) - 4U;
        SCB->SHP[shp] = (uint8_t)((priority << (8U - __NVIC__PRIO_BITS)) & 255U );
    }
    else NVIC->IP[IRQn] = (uint8_t)( (priority << (8U-__NVIC__PRIO_BITS)) & 255U);
}
//Tested
uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn)
{
    if(IRQn < 0)
    {
        return (uint32_t)(((uint32_t)SCB->SHP[IRQn]) >> (8U-__NVIC__PRIO_BITS));
    }
    return (uint32_t)(((uint32_t)NVIC->IP[IRQn]) >> (8U-__NVIC__PRIO_BITS));
}
//Will not work with negative numbers
//Tested
void __NVIC_EnableIRQn(IRQn_TypeDef IRQn)
{   
    if(IRQn< 0)return;
    int iser = IRQn/32;
    NVIC->ISER[iser] = (1 << (IRQn & 31));
}
//Tested
void __NVIC_DisableIRQn(IRQn_TypeDef IRQn)
{
    if(IRQn < 0)return;
    int icer = IRQn/32;
    NVIC->ICER[icer] = (1 << (IRQn & 31));
}
//Tested
uint32_t isEnabled(IRQn_TypeDef IRQn)
{
    if(IRQn<0)return;
    int iser = IRQn/32;
    if( (NVIC->ISER[iser]&( 1<< (IRQn&31))) != 0)return 1;
    return 0;
}
//Tested
void __disable_irq(void)
{
    __asm volatile("cpsid i" : : : "memory");
}
//Tested
void __enable_irq(void)
{
    __asm volatile("cpsie i" : : : "memory");
}
//Tested
uint32_t __get_PRIMASK(void)
{
    uint32_t res;
    __asm volatile("MRS %0, primask" : "=r" (res) );
    return (res);
}
uint32_t __set_PRIMASK(uint32_t value)
{
    __asm volatile("MSR primask, %0" : : "r" (value) : "memory");
}
//TODO TEST
void __set_BASEPRI(uint32_t value)
{
    __asm volatile("MSR basepri, %0" : : "r" (value) : "memory");
}

//TODO implement __get_BASEPRI
//TODO implement __unset_BASEPRI

//TODO implement __enable_fault_irq(void)
//TODO implement __set_FAULTMASK(uint32_t value)
//TODO implement __disable_fault_irq(void)
//TODO __get_FAULTMASK

//TODO __clear_pending_IRQn(IRQn_TypeDef IRQn)
//TODO __get_pending_IRQn(IRQn_TypeDef IRQn)
//TODO __NVIC_getActive(IRQn_TypeDef IRQn)