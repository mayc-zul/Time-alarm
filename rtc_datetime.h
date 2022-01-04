#ifndef RTC_DATETIME_H_
#define RTC_DATETIME_H_

#define TIMER_ALARM        60000

extern datetime_t t;
extern char datetime_str[21];
extern char datetime_strAlarm[21];
extern volatile bool FlagAlarm;
extern char buffer[17];
extern char buffer_alarm[17];


void rtc_callback(void);
void ValidFormat(int value);
void rtc_datetimeInit(void);
char *rtc_datetimeGet(void);
char *rtc_datetimeGetAlarm(void);
void Weekday (int day, bool alarm);
void assingDay(int cont, bool alarm);
void rtc_datetimeSet(int dotw);
void rtc_datetimeSetAlarm(int dotw);



#endif