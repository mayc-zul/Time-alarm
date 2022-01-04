#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"


#include "KeyMatrix.h" 
#include "timer.h"
#include "lcd.h"
#include "rtc_datetime.h"

char key = '.';
bool Flagshow = true;
bool Flagpress = false;
int rowlcd = 1;
int collcd = 1;
int cursorvirtual = 1;

int start = 0;
int end = 16;
int contAlarm = 0;
char buffer[17] ="";
char buffer_alarm[17] = "";

int main(void) {
    int contDisplay = 0;
    int contDatetime = 0;
    int weekday = 0;
    int weekdayAlarm = 0;    
    
    // Inicializar las funciones de entrada/salida estándar
    stdio_init_all();
    
    rtc_datetimeInit();
    keyMatrixInit();
    LCDInit();

    TimerInit(CNT_SPLITREAD);

    while (1){
        key = keyMatrixRead();
        
        if (timer_req){
            timer_req = false;
            if (ProcesarTecla == true){
                cnt_ProcessKey++; 
            } 
            if(contDisplay == 500 && Flagshow){
                setCursor(1,1);
                WriteMessage(rtc_datetimeGet());
                setCursor(1,2);
                WriteMessage("Press D to configure");
                scrollDisplayLeft();
                cursorOff();
                strcpy(datetime_str, "");
                contDisplay = 0;
            }else if(Flagshow == false){
                contDisplay = 0;
            }

            contDisplay++;
        }

        if(Flagshow == false && key != '.'&& key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#'&& key != '*'){
            if(rowlcd == 1){
                if(cursorvirtual == 1 && key != '7' && key != '8' && key != '9'){
                    weekday = (int)key - 48;
                    assingDay(weekday, false);
                    setCursor(1,rowlcd);
                    WriteMessage(buffer);
                    setCursor(1,rowlcd);
                }else if(cursorvirtual > 1){
                    if (datetime_str[cursorvirtual-1] != '/' && datetime_str[cursorvirtual-1] != ':' && datetime_str[cursorvirtual-1] != ' ' && cursorvirtual != 2 && cursorvirtual != 3){
                        datetime_str[cursorvirtual-1] = key;
                        strncpy(buffer,&datetime_str[start],end);
                        buffer[16] = '\0';
                        setCursor(1,rowlcd);
                        WriteMessage(buffer);
                    }
                }

            }else{
                if(cursorvirtual == 1 && key != '7' && key != '8' && key != '9'){
                    weekdayAlarm = (int)key - 48;
                    assingDay(weekdayAlarm, true);
                    setCursor(1,rowlcd);
                    WriteMessage(buffer_alarm);
                    setCursor(1,rowlcd);
                }else if(cursorvirtual > 1){
                    if (datetime_str[cursorvirtual-1] != '/' && datetime_str[cursorvirtual-1] != ':' && datetime_str[cursorvirtual-1] != ' ' && cursorvirtual != 2 && cursorvirtual != 3){
                        datetime_strAlarm[cursorvirtual-1] = key;
                        strncpy(buffer_alarm,&datetime_strAlarm[start],end);
                        buffer_alarm[16] = '\0';
                        setCursor(1,rowlcd);
                        WriteMessage(buffer_alarm); 
                    }
                }
            }
            cursorOn();
        }

        if(FlagAlarm){
            if(contAlarm == TIMER_ALARM){
                contAlarm = 0;
                FlagAlarm = false;
            }
            contAlarm++;
            printf("Alarm!\n");
        }

        if(key != '.'){
            printf("Tecla oprimida: %c\n", key);
        } 

        switch (key){
        case 'D':
            Clear();
            returnHome();
            cursorOn();
            setCursor(2,1);
            WriteMessage(rtc_datetimeGet());
            setCursor(1,2);
            WriteMessage(rtc_datetimeGetAlarm());
            cursorOn();
            returnHome();
            Flagshow = false;
            break;
        case 'A':
            if(!Flagshow){
                if(rowlcd > 1){
                    rowlcd--;
                    setCursor(collcd,rowlcd);
                }
            }
            break;
        case 'B':
            if(!Flagshow){
                if(rowlcd < 2){
                    rowlcd++;
                    setCursor(collcd,rowlcd);
                }
            }
            break;
        case '#':
            if(!Flagshow){  
                if(collcd < 16){             
                    collcd++;
                    setCursor(collcd,rowlcd);
                    cursorvirtual++;
                }else if(end < strlen(datetime_str)){
                    end++;
                    start++;
                    strncpy(buffer,&datetime_str[start],end);
                    buffer[16] = '\0';
                    setCursor(1,1);
                    WriteMessage(buffer);
                    // -- Alarm -----
                    strncpy(buffer_alarm,&datetime_strAlarm[start],end);
                    buffer_alarm[16] = '\0';
                    setCursor(1,2);
                    WriteMessage(buffer_alarm);
                    setCursor(collcd,rowlcd);
                    cursorvirtual++;
                }
            }
            break;
        case '*':
            if(!Flagshow){
                if(collcd > 1){
                    collcd--;
                    setCursor(collcd,rowlcd);
                    cursorvirtual--;
                }else if(start > 0){
                    end--;
                    start--;
                    strncpy(buffer,&datetime_str[start],end);
                    buffer[16] = '\0';
                    setCursor(1,1);
                    WriteMessage(buffer);
                    // -- Alarm -----
                    strncpy(buffer_alarm,&datetime_strAlarm[start],end);
                    buffer_alarm[16] = '\0';
                    setCursor(1,2);
                    WriteMessage(buffer_alarm);
                    setCursor(collcd,rowlcd);
                    cursorvirtual--;
                }
            }
            break;
        
        case 'C':
            if(!Flagshow){
                Flagshow = true;
                rtc_datetimeSet(weekday);
                rtc_datetimeSetAlarm(weekdayAlarm);
                Clear();
                returnHome();
                cursorvirtual = 1;
                rowlcd = 1;
                collcd = 1;
                start = 0;
                end = 16;
            }
        default:
            break;
        }

        __asm volatile("wfi");
    }

        
    return 0;
}

