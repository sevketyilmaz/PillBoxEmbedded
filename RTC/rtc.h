#ifndef __RTC_H
#define __RTC_H

#include <time.h>

extern uint8_t rtc_alarm_irqhandler_flag;
extern uint8_t alarmtimecounter, alarmok, read_boxes;

void rtc_init(void);
void rtc_alarm_init(void);
void sendTime(void);

time_t get_current_time_ms(void);
#endif /* __RTC_H */

