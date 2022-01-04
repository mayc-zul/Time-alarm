#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "timer.h"
#include "KeyMatrix.h"


unsigned char cnt_ProcessKey = 0;
bool ProcesarTecla = false;

const int col[4] = {5,4,3,2};
const int row[4] = {9,8,7,6};

unsigned char iRow = 0, iCol = 0;

char keys[4][4] = {
  { '1','2','3', 'A' },
  { '4','5','6', 'B' },
  { '7','8','9', 'C' },
  { '*','0','#', 'D' }
};

char value = '1';
volatile bool gpio_req = false;
volatile uint gpioRead;

void gpio_callback (uint gpio, uint32_t events){
    gpio_req = true;
    gpioRead = gpio;
}

void keyMatrixInit(void) {
	
    // Definción del las columnas como entradas en pull-up
    for (int i = 0; i < 4; i++){
        //---- Col --------
        gpio_init(col[i]);                                                                      // Difinición de los puertos
        gpio_set_dir(col[i], GPIO_IN);                                                          // Configuración las columnas como entradas
        gpio_pull_up(col[i]);                                                                   // Configuración entradas en pull up (columnas)
        gpio_set_irq_enabled_with_callback(col[i], GPIO_IRQ_EDGE_FALL, true, gpio_callback);    // Habilitar interrupción con callback cuando ocurra una flanco de bajada
        
        //---- Row --------
        gpio_init(row[i]);                                                                      // Definición de los puertos
        gpio_set_dir(row[i], GPIO_OUT);                                                         // Configuración las filas como salidas
        gpio_put(row[i], 1);                                                                    // Se inicializan en 1
    }
}

char keyMatrixRead(void){
    // recorre filas
    for(int i=0; i < 4; i++){
        gpio_put(row[i], 0);
        if (gpio_req){
            gpio_req = false;
            ProcesarTecla = true;
            if(i == 0){
                iRow = 3;
            }else{
                iRow = i - 1;
            }
        }
        if(cnt_ProcessKey == CNT_DEBOUNCE){
            cnt_ProcessKey = 0;
            ProcesarTecla = false;
            for (int j = 0; j < 4; j++){
                if(col[j] == gpioRead){ 
                    iCol = j;
                    return keys[iRow][iCol];
                    //value = keys[iRow][iCol]; 
                    
                }
            }
            
        
        }
        gpio_put(row[i], 1);  
    }
    return '.';
}