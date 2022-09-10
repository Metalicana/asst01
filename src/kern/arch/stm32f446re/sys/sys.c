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
#include "../include/dev/usart.h"
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
void EXTI0_Handler(void)
{
	_USART_WRITE(USART2,(uint8_t*)"Interrupt triggered\n");
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
        int shp = ((uint32_t)IRQn & 15) - 4U;
        SCB->SHP[shp] = (uint8_t)((priority << (8U - __NVIC__PRIO_BITS)) & 255U );
    }
    else NVIC->IP[IRQn] = (uint8_t)( (priority << (8U-__NVIC__PRIO_BITS)) & 0xFFU);
}
//Tested
uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn)
{
    if(IRQn < 0)
    {
        int shp = ((uint32_t)IRQn & 15) - 4U;
        return (uint32_t)(((uint32_t)SCB->SHP[shp]) >> (8U-__NVIC__PRIO_BITS));
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
//Tested
void __set_BASEPRI(uint32_t value)
{
   __asm volatile("MOVS R12, %0"
                   :
                   : "r"(value & 0xFFU)
                   :);
    __asm volatile("MSR BASEPRI, R12"
                   :
                   :
                   :);
}
//tested
void __unset_BASEPRI()
{
    __set_BASEPRI(0);
}
uint32_t __NVIC_GetActive(IRQn_TypeDef IRQn)
{
  if(IRQn < 0)return 0;
  else
  {
    int iabr = IRQn/32;
    if(NVIC->IABR[iabr] & (1 << (IRQn&31)) != 0)return 1;
    else return 0;
  }
  
}
//Tested
uint32_t __get_BASEPRI(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, basepri" : "=r" (result) );
  return(result>>4);
}
//TODO TEST
void __enable_fault_irq(void)
{
  __asm volatile ("cpsie f" : : : "memory");
}
//TODO test
void __set_FAULTMASK(uint32_t faultMask)
{
  __asm volatile ("MSR faultmask, %0" : : "r" (faultMask) : "memory");
}
void __disable_fault_irq(void)
{
  __asm volatile ("cpsid f" : : : "memory");
}
uint32_t __get_FAULTMASK(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, faultmask" : "=r" (result) );
  return(result);
}
//Tested
void __NVIC_ClearPendingIRQ(IRQn_TypeDef IRQn)
{
  if(IRQn < 0)return;
  else
  {
    int icpr = IRQn/32;
    NVIC->ICPR[icpr] = (uint32_t)(1UL << (((uint32_t)IRQn) & 31));
  }
}
uint32_t __NVIC_GetPendingIRQ(IRQn_TypeDef IRQn)
{

  if(IRQn<0)return 0;
  else
  {
    int ispr = IRQn/32;
    if(NVIC->ISPR[ispr] & (1UL<< (IRQn&31)) != 0)return 1;
    else return 0;
  }
}
