// ---------------------------------------------------------------------------
// ---------------------- 1. Importacion de librerias ------------------------
// ---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"


#include "KeyMatrix.h" 
#include "timer.h"
#include "lcd.h"
#include "rtc_datetime.h"

// --------------------------------------------------------------------------------------
// ---------------- 2. Deifinicion e inicializacion de variables ------------------------
// --------------------------------------------------------------------------------------

// ------- Varible para leer la tecla presionada -------------
char key = '.';
// --------------------- Banderas --------------------------- 
bool Flagshow = true;
bool Flagpress = false;
// ------- Variables para el manejo del LCD -----------------
int rowlcd = 1;
int collcd = 1;
int cursorvirtual = 1;
int start = 0;
int end = 16;
char buffer[17] ="";
char buffer_alarm[17] = "";
// --- Variaable mantener activa la señal de alarma por 1 min  ----
int contAlarm = 0;
// ------ Pines del led --------
const uint32_t GP_LED_OUTPUT = 1;
bool LedOn = true; 


// ---------------------------------------------------------
// ---------------------- 3. Main --------------------------
// ---------------------------------------------------------

int main(void) {
    int contDisplay = 0;            // Variable para tipo contador para activar el scrolling shift
    int weekday = 0;                // Variable contiene el número asociado al día de la semana de la fecha
    int weekdayAlarm = 0;           // Variable contiene el número asociado al día de la semana de la fecha de la alarma    
    
    // Inicialización salidas leds
    gpio_init(GP_LED_OUTPUT);
    gpio_set_dir(GP_LED_OUTPUT, GPIO_OUT);

    // Inicializar las funciones de entrada/salida estándar
    stdio_init_all();
    // Inicialización del modulo rtc, KeyMatrix y LCD
    rtc_datetimeInit();
    keyMatrixInit();
    LCDInit();
    // Inicialización del Timer
    TimerInit(CNT_SPLITREAD);

    while (1){
        // Se detecta la tecla oprimida
        key = keyMatrixRead();
        
        // -----------------------------------------------------------------------------------
        // ------------- Seccion que se ejecuta al detectar interrupcion del timer -----------
        // -----------------------------------------------------------------------------------
        if (timer_req){
            timer_req = false;                                  // Se desactiva la bandera de la interrupción del Timer
            if (ProcesarTecla == true){                         // Se identifica si una tecla fue presionada
                cnt_ProcessKey++;                               // Activa el debounce
            } 
            if(contDisplay == 500 && Flagshow){                 // Cada 500ms se hacer Scrolling shift
                setCursor(1,1);                                 // Se escribe en la posicion columna 1, fila 1
                WriteMessage(rtc_datetimeGet());
                setCursor(1,2);                                 // Se escribe en la posicion columna 1, fila 2
                WriteMessage("Press D to configure");
                scrollDisplayLeft();                            // Se ejecuta el scrolling
                cursorOff();
                strcpy(datetime_str, "");                       // Se limpia la cadena de caracter que contiene la fecha
                contDisplay = 0;                                // Reset 
            }else if(Flagshow == false){
                contDisplay = 0;
            }

            contDisplay++;
        }

        // -----------------------------------------------------------------------------------
        // ------------- Seccion que se ejecuta en el modo configuración ---------------------
        // -----------------------------------------------------------------------------------
        // Se presiona una tecla númerica
        if(Flagshow == false && key != '.'&& key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '#'&& key != '*'){
            // Se modifica la primera fila (fecha)
            if(rowlcd == 1){
                // El cursor se encuentra en la primera columna (cambio de día de la semana)
                if(cursorvirtual == 1 && key != '7' && key != '8' && key != '9'){
                    weekday = (int)key - 48;
                    assingDay(weekday, false);                  // Funciona que modifica el buffer que se escribe en el LCD el día de la semana correspondiente
                    setCursor(1,rowlcd);
                    WriteMessage(buffer);
                    setCursor(1,rowlcd);
                }else if(cursorvirtual > 1){
                    if (datetime_str[cursorvirtual-1] != '/' && datetime_str[cursorvirtual-1] != ':' && datetime_str[cursorvirtual-1] != ' ' && cursorvirtual != 2 && cursorvirtual != 3){
                        // Modificación del resto de los campos (mes, día, año, hora, minuto) 
                        datetime_str[cursorvirtual-1] = key;
                        strncpy(buffer,&datetime_str[start],end);
                        buffer[16] = '\0';
                        setCursor(1,rowlcd);
                        WriteMessage(buffer);
                    }
                }
            // Se modifica la segunda fila (fecha alarma)
            }else{
                // El cursor se encuentra en la primera columna (cambio de día de la semana)
                if(cursorvirtual == 1 && key != '7' && key != '8' && key != '9'){
                    weekdayAlarm = (int)key - 48;
                    assingDay(weekdayAlarm, true);              // Funciona que modifica el buffer que se escribe en el LCD el día de la semana correspondiente
                    setCursor(1,rowlcd);
                    WriteMessage(buffer_alarm);
                    setCursor(1,rowlcd);
                }else if(cursorvirtual > 1){
                    if (datetime_str[cursorvirtual-1] != '/' && datetime_str[cursorvirtual-1] != ':' && datetime_str[cursorvirtual-1] != ' ' && cursorvirtual != 2 && cursorvirtual != 3){
                        // Modificación del resto de los campos (mes, día, año, hora, minuto)
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

        // -----------------------------------------------------------------------------------
        // -------- Seccion que se ejecuta al detectar interrupcion del RTC (alarma) ---------
        // -----------------------------------------------------------------------------------
        if(FlagAlarm){
            // Se ejecuta por 1 minuto
            if(contAlarm == TIMER_ALARM || key == '1'){
                contAlarm = 0;
                gpio_put(GP_LED_OUTPUT, 0);           // Led off en caso de que se desactive la alarma y el led este encendido
                FlagAlarm = false;
            }
            if(contAlarm % 400 == 0){
                LedOn = !LedOn;
                gpio_put(GP_LED_OUTPUT, LedOn);           // Se asigna el valor actual
            }
            contAlarm++;
        }

        if(key != '.'){
            printf("Tecla oprimida: %c\n", key);
        } 

        // Selección de opciones
        switch (key){
        case 'D':                                       // Selección del modo configuración
            Clear();
            returnHome();
            cursorOn();
            setCursor(1,1);
            WriteMessage(rtc_datetimeGet());
            setCursor(1,2);
            WriteMessage(rtc_datetimeGetAlarm());
            cursorOn();
            returnHome();
            Flagshow = false;
            break;
        case 'A':                                       // Cursor hacía arriba
            if(!Flagshow){
                if(rowlcd > 1){
                    rowlcd--;
                    setCursor(collcd,rowlcd);
                }
            }
            break;
        case 'B':                                       // Cursor hacía abajo
            if(!Flagshow){
                if(rowlcd < 2){
                    rowlcd++;
                    setCursor(collcd,rowlcd);
                }
            }
            break;
        case '#':                                       // Cursor hacía la derecha 
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
        case '*':                                       // Cursor hacía la izquierda
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
        
        case 'C':                                       // Modo modificación
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

