#include "delay.h"
#include "sys.h"
//如果需要使用OS，则包含下面的头文件
#if SYSTEM_SUPPORT_OS
#include "includes.h"
#endif

static uint32_t fac_us = 72; //us延时倍数,默认72Mhz

#if SYSTEM_SUPPORT_OS
    static uint16_t fac_ms=0;
#endif

#if SYSTEM_SUPPORT_OS
/*当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
首先是三个宏定义
***delay_osrunning ：表示OS当前是否正在运行，已决定是否可以使用相关函数
***delay_ostickspersec： 用于表示OS设定的时钟节拍，delay_int将根据这个参数来初始化systick
***delay_osintnesting：用于表示中断嵌套级别，因为中断里不可以调度，delay_ms使用参数来决定如何运行
然后是3个函数：
***delay_osschedlock:用于锁定OS任务调度，禁止调度
***delay_osschedunlock:用于解锁OS任务调度，重新开始调度
***delay_ostimedly:用于OS延时，可以引起任务调度

********************仅定义了对UCOSIII的支持**********************
*/
#define delay_osrunning OSRunning
#define delay_ostickspersec OSCfg_TickRate_Hz
#define delay_osintnesting OSIntNestingCtr 

//us级延时，关闭任务调度（防止打算us级延时）
void delay_osschedlock(void)
{
    OS_ERR err;
    OSSchedLock(&err);
}

//us级延时，恢复任务调度
void delay_osschedunlock(void)
{
    OS_ERR err;
    OSSchedUnlock(&err);
}

//调用OS自带的延时函数延时
//ticks：延时的节拍数
void delay_ostimedly(uint32_t ticks)
{
    OS_ERR err;
    OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);
}

//systick中断服务函数，使用ucos时用到
void SysTick_Handler(void)
{
    if(delay_osrunning == )
    {
        OSIntEnter();
        OSTimerTick();
        OSIntExit();
    }
}
#endif

//初始化延时函数，当使用ucos时，此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定未AHB时钟
//SYSCLK：系统时钟频率
void delay_init(uint8_t SYSCLK)
{
#if SYSTEM_SUPPORT_OS
    uint32_t reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    fac_us = SYSCLK;
#if SYSTEM_SUPPORT_OS
    reload=SYSCLK;
    reload *= 1000000/delay_ostickspersec;
    fac_ms = 1000/delay_ostickspersec;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = reload;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
#endif
}

#if SYSTEM_SUPPORT_OS 
//延时nus
//nus:0~190887435(最大值即 2^32/fac_us @fac_us=22.5)	    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				   	 
	ticks=nus*fac_us; 						
	delay_osschedlock();					
	told=SysTick->VAL;        				
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;		
		}  
	};
	delay_osschedunlock();											    
}  

//nms:0~65535
void delay_ms(uint16_t nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)
	{		 
		if(nms>=fac_ms)						
		{ 
   			delay_ostimedly(nms/fac_ms);	
		}
		nms%=fac_ms;					
	}
	delay_us((uint32_t)(nms*1000));				
}
#else 
//延时nus
//nus:0~190887435(最大值即 2^32/fac_us @fac_us=22.5)	 
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD 的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			
		}  
	};
}

void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}
#endif