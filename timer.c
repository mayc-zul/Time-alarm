#include <stdio.h>
#include "pico/stdlib.h"

#include "timer.h"

// Defición variables para la interrupción del timer
struct repeating_timer timer;
volatile bool timer_req = false;

// Función tipo callback para atender la interrupción
bool timer_callback (repeating_timer_t *t){
    timer_req = true;
    return true;
    
}

// Inicialización de la interrupción del Timer
bool TimerInit(int32_t delay_ms) {
    if (add_repeating_timer_ms(-delay_ms, timer_callback, NULL, &timer)){
        printf("El Timer fue inicializado correctamente");
        return true;
    }
    else{
        printf("El Timer no fue inicializaco correctamente");
        return false;
    }
}