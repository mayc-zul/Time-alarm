#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

#include "rtc_datetime.h"

// Definción de variables
char datetime_str[21] = "";
char datetime_strAlarm[21] = "";
volatile bool FlagAlarm = false;
char days[7][3] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char string[5] = "";

// Definición estructurada Datetime
datetime_t t = {
    .year = 2022, // Year
    .month = 1,  // Month 
    .day = 23,    // Day  
    .dotw = 0,    // Day-week 0:Sun - 6:Sat
    .hour = 15,   // Hour
    .min = 45,    // Min
    .sec = 00,    // Sec
};
// Definición estructurada Datetime para la alarma
datetime_t alarm = {
    .year = 2022, // Year
    .month = 1,  // Month 
    .day = 23,    // Day  
    .dotw = 0,    // Day-week
    .hour = 15,   // Hour
    .min = 46,    // Min
    .sec = 00,    // Sec
};

// Definción del callback para atender la interrupción del RTC
void rtc_callback(){
    FlagAlarm = true;
}

// Inicialización del RTC
void rtc_datetimeInit(){

    // Start el RTC
    rtc_init();
    rtc_set_datetime(&t);
    rtc_set_alarm(&alarm, rtc_callback);

}

// Formato para imprimir fecha en el LCD
char* rtc_datetimeGet(){

    rtc_get_datetime(&t);
    //strcat(datetime_str," ");
    Weekday(t.dotw, false);
    strcat(datetime_str," ");
    ValidFormat(t.day);
    strcat(datetime_str, string);
    strcat(datetime_str,"/"); 
    ValidFormat(t.month); 
    strcat(datetime_str, string);
    strcat(datetime_str,"/");
    ValidFormat(t.year);
    strcat(datetime_str, string);
    strcat(datetime_str," ");
    ValidFormat(t.hour);
    strcat(datetime_str, string);
    strcat(datetime_str,":");
    ValidFormat(t.min);
    strcat(datetime_str, string); 
    datetime_str[20] = '\0';
    strcpy(string, "");
    
}

// Modificación de la estructura Datetime de la fecha
void rtc_datetimeSet(int dotw){
    char year[5];
    char month[3];
    char day [3];
    char hour[3];
    char min[3]; 
    
    strncpy(year, &datetime_str[10],4);
    year[4] ='\0';
    strncpy(month, &datetime_str[7],2);
    month[2] ='\0';
    strncpy(day, &datetime_str[4],2);
    day[2] ='\0';
    strncpy(hour, &datetime_str[15],2);
    hour[2] ='\0';
    strncpy(min, &datetime_str[18],2);
    min[2] ='\0';
    
    t.year = atoi(year);
    if(atoi(month) >= 1 && atoi(month) <= 12){
        t.month = atoi(month);
        if(t.month == 2){
            if(atoi(day) >= 1 && atoi(day) <= 28){
                t.day = atoi(day);
            }
        }else if(t.month == 4 || t.month == 6 || t.month == 9 || t.month == 11){
            if(atoi(day) >= 1 && atoi(day) <= 30){
                t.day = atoi(day);
            }
        }else{
            if(atoi(day) >= 1 && atoi(day) <= 31){
                t.day = atoi(day);
            }
        }
    }
    t.dotw = dotw;
    if(atoi(hour) < 24){
        t.hour = atoi(hour);
    }
    if(atoi(min) < 60){
        t.min = atoi(min);
    }
    rtc_set_datetime(&t);
    strcpy(datetime_str, "");
    strcpy(buffer, "");
}

// Definción de formato para imprimir el día de la semana
void Weekday (int day, bool alarm){
    char bufferweekday[4] = "";

    switch (day){
    case 0:
        strcat(bufferweekday,"Sun");
        break;
    case 1:
        strcat(bufferweekday,"Mon");
        break;
    case 2:
        strcat(bufferweekday,"Tue");
        break;
    case 3:
        strcat(bufferweekday,"Wed");
        break;
    case 4:
        strcat(bufferweekday,"Thu");
        break;
    case 5:
        strcat(bufferweekday,"Fri");
        break;
    case 6:
        strcat(bufferweekday,"Sat");
        break;
    default:
        break;
    }
    if (alarm){
        strcat(datetime_strAlarm,bufferweekday);
    }else{
        strcat(datetime_str,bufferweekday);
    }
    
}

// Función para validar si se ingresa un digito antes de un cero
void ValidFormat(int value){
    if(value > 9){
        itoa(value, string, 10);
    }else{
        string[0] = '0';
        string[1] = (char)value + 48;
    }
}  

// Funcióna para asignar el día de la semana correspondiente al número correspondiente al día de la semana
void assingDay(int cont, bool alarm){
    if (alarm){
        buffer_alarm[0] = days[cont][0];
        buffer_alarm[1] = days[cont][1];
        buffer_alarm[2] = days[cont][2];

        datetime_strAlarm[0] = days[cont][0];
        datetime_strAlarm[1] = days[cont][1];
        datetime_strAlarm[2] = days[cont][2];

    }else{
        buffer[0] = days[cont][0];
        buffer[1] = days[cont][1];
        buffer[2] = days[cont][2];

        datetime_str[0] = days[cont][0];
        datetime_str[1] = days[cont][1];
        datetime_str[2] = days[cont][2]; 
    }
    

}

// --------------------- Alarma --------------------
// Formato para imprimir la fehca de la alarma
char* rtc_datetimeGetAlarm(){

    //strcat(datetime_str," ");
    Weekday(alarm.dotw, true);
    strcat(datetime_strAlarm," ");
    ValidFormat(alarm.day);
    strcat(datetime_strAlarm, string);
    strcat(datetime_strAlarm,"/");
    ValidFormat(alarm.month);  
    strcat(datetime_strAlarm, string);
    strcat(datetime_strAlarm,"/");
    ValidFormat(alarm.year);
    strcat(datetime_strAlarm, string);
    strcat(datetime_strAlarm," ");
    ValidFormat(alarm.hour); 
    strcat(datetime_strAlarm, string);
    strcat(datetime_strAlarm,":");
    ValidFormat(alarm.min); 
    strcat(datetime_strAlarm, string);
    datetime_strAlarm[20] = '\0'; 
    strcpy(string, "");
    
}

// Modificación de la fecha de la alarma
void rtc_datetimeSetAlarm(int dotw){
    char year[5];
    char month[3];
    char day [3];
    char hour[3];
    char min[3]; 
    
    strncpy(year, &datetime_strAlarm[10],4);
    year[4] ='\0';
    strncpy(month, &datetime_strAlarm[7],2);
    month[2] ='\0';
    strncpy(day, &datetime_strAlarm[4],2);
    day[2] ='\0';
    strncpy(hour, &datetime_strAlarm[15],2);
    hour[2] ='\0';
    strncpy(min, &datetime_strAlarm[18],2);
    min[2] ='\0';
    
    alarm.year = atoi(year);
    if(atoi(month) >= 1 && atoi(month) <= 12){
        alarm.month = atoi(month);
        if(alarm.month == 2){
            if(atoi(day) >= 1 && atoi(day) <= 28){
                alarm.day = atoi(day);
            }
        }else if(alarm.month == 4 || alarm.month == 6 || alarm.month == 9 || alarm.month == 11){
            if(atoi(day) >= 1 && atoi(day) <= 30){
                alarm.day = atoi(day);
            }
        }else{
            if(atoi(day) >= 1 && atoi(day) <= 31){
                alarm.day = atoi(day);
            }
        }
    }
    alarm.dotw = dotw;
    if(atoi(hour) < 24){
        alarm.hour = atoi(hour);
    }
    if(atoi(min) < 60){
        alarm.min = atoi(min);
    }
    rtc_set_alarm(&alarm, rtc_callback);
    strcpy(datetime_strAlarm, "");
    strcpy(buffer_alarm, "");
}



