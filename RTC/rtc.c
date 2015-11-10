#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_rtc.h"

#include "stdio.h"
#include "rtc.h"
#include "usart.h"
#include "delay.h"

char str_rtc[50];

void rtc_init() {
    RTC_InitTypeDef RTC_InitStructure;
//  RTC_TimeTypeDef RTC_TimeStructure;
//  RTC_DateTypeDef RTC_DateStructure;	
	
	NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_LSEConfig(RCC_LSE_ON);
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        
  /* EXTI configuration */
    EXTI_ClearITPendingBit(EXTI_Line22);
    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);

		RTC_WaitForSynchro();
			
				RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line22);
			
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv =  0xFF;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);
		
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);

		RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x7FF);
  
    RTC_ITConfig(RTC_IT_WUT, ENABLE);
    RTC_WakeUpCmd(ENABLE); 
}

void rtc_alarm_init() {
	
//	RTC_AlarmTypeDef RTC_AlarmStructure;
	NVIC_InitTypeDef NVIC_InitStructur;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* EXTI configuration */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructur.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructur.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructur.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructur.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructur);

	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
  // *If you select trigger for alarm every week, then this parameter has value between
  // *1 and 7, representing days in a week, Monday to Sunday
  // *If you select trigger for alarm every month, then this parameter has value between
  // *1 - 31, representing days in a month.
 
	/* Enable the alarm */
	//RTC_AlarmStructure.RTC_AlarmDateWeekDay = 2;
	//RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
	//RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	
	//RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_HourFormat_24;
	//RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 12;
	//RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 34;
	//RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 30;
	//RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

void sendTime(void){
 
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

	sprintf(str_rtc,"     #:%d:%d:%d:%d:%d:%d:%d:\n",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds,
	RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_WeekDay);
	USART_puts(USART1,str_rtc);
}

void RTC_Alarm_IRQHandler(void) {
	
	/* Clear the EXTIL line 17 */
	/* Check on the AlarmA falg and on the number of interrupts per Second (60*8) */
	if (RTC_GetITStatus(RTC_IT_ALRA) != RESET){
		/* Clear RTC AlarmA Flags */
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
		
		STM_EVAL_LEDOn(LED3);
	}	
}

void RTC_WKUP_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET){
		
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line22);
		
	}	
}