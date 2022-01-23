#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "lcd.h"

#define RS     15
#define E      17
#define RW     16


// {D7,D6,D5,D4, E , RS , RW}
int LCDpins[7] = {21, 20, 19, 18, E, RS, RW};

int LCDdisplay = 0;

// Se envia RS->0 indicando que el dato ingresado es una instrucción
// Adicional se habilita el modulo LCD durante 1us 
void Instruction(){
    
    gpio_put(RS, false);
    gpio_put(RW, false);
    gpio_put(E, true);
    sleep_us(1);
    
    gpio_put(E, false);
    sleep_us(100);
}

// Se envia RS->1 indicando que el dato ingresado es una dato
// Adicional se habilita el modulo LCD durante 1us 
void Data(){
    gpio_put(RS, true);
    gpio_put(RW, false);
    gpio_put(E, true);
    sleep_us(1);

    gpio_put(E, false);
    sleep_us(500);
}

// Formato para la lectura de comandos de 4-bit
void CmdWrite4 (int BinNum){
    gpio_put(LCDpins[3], (BinNum & 0b00000001)>>0);
    gpio_put(LCDpins[2], (BinNum & 0b00000010)>>1);
    gpio_put(LCDpins[1], (BinNum & 0b00000100)>>2);
    gpio_put(LCDpins[0], (BinNum & 0b00001000)>>3);
    Instruction();
}


// Formato para la lectura de comandos de 8-bit
void CmdWrite8 (int BinNum){
    
    gpio_put(LCDpins[3], (BinNum & 0b00010000)>>4);
    gpio_put(LCDpins[2], (BinNum & 0b00100000)>>5);
    gpio_put(LCDpins[1], (BinNum & 0b01000000)>>6);
    gpio_put(LCDpins[0], (BinNum & 0b10000000)>>7);
    Instruction();

    gpio_put(LCDpins[3], (BinNum & 0b00000001)>>0);
    gpio_put(LCDpins[2], (BinNum & 0b00000010)>>1);
    gpio_put(LCDpins[1], (BinNum & 0b00000100)>>2);
    gpio_put(LCDpins[0], (BinNum & 0b00001000)>>3);
    Instruction();

}

// Formato para la lectura de datos de 8-bit
void DataWrite (int BinNum){
    gpio_put(LCDpins[3], (BinNum & 0b00010000)>>4);
    gpio_put(LCDpins[2], (BinNum & 0b00100000)>>5);
    gpio_put(LCDpins[1], (BinNum & 0b01000000)>>6);
    gpio_put(LCDpins[0], (BinNum & 0b10000000)>>7);
    Data();

    gpio_put(LCDpins[3], (BinNum & 0b00000001)>>0);
    gpio_put(LCDpins[2], (BinNum & 0b00000010)>>1);
    gpio_put(LCDpins[1], (BinNum & 0b00000100)>>2);
    gpio_put(LCDpins[0], (BinNum & 0b00001000)>>3);
    Data();

}


// Función de inicialización 
void LCDInit(){
    
    // Se configura como salidas y inicializan en 0
    for(int j = 0; j < 7; j++){
        gpio_init(LCDpins[j]);
        gpio_set_dir(LCDpins[j], GPIO_OUT);
        gpio_put(LCDpins[j], false);
    }
    sleep_ms(15);
    CmdWrite4(0b0011);                  //8-bit
    CmdWrite4(0b0011);                  //8-bit
    CmdWrite4(0b0011);                  //8-bit
    CmdWrite4(0b0010);                  //4-bit
    CmdWrite8(0b00101000);              // 4 bit,2 lines,5*8 bots
    CmdWrite8(0b00001100);              // lcd on, blink off, cursor off
    CmdWrite8(0b00000110);              // increment cursor, no display shift
    CmdWrite8(0b00000001);              // clear screen
    sleep_ms(2);

}

// Función para limpiar el LCD
void Clear (){
    CmdWrite8(0b00000001);
}
// Función activar el cursor
void cursorOn(){
    CmdWrite8(0b00001101);
}
// Función desactivar el cursor
void cursorOff(){
    CmdWrite8(0b00001100);
}
// Función Retornar a la primera posición de la memoria del LCD
void returnHome(){
    CmdWrite8(0b00000010);
}
// Función para hacer corrimiento a la izquierda
void shiftCursoLeft(){
    CmdWrite8(0b00010000);
}
// Función para hacer corrimiento a la derecha
void shiftCursoRight(){
    CmdWrite8(0b00010100);
}
// Función para apuntar a una posición de memoria especifica
void setCursor(uint8_t x,  uint8_t y){
	char logicForYaxis[2]={0,64};
    int tmp;
	x-=1;
	y-=1;
    tmp = 0x80 + logicForYaxis[y] + x;
	CmdWrite8(tmp);
}
// Función para hacer Scrolling a la izquierda
void scrollDisplayLeft(){
    CmdWrite8(0b00011000);
}

// Función para imprimir cadena de caracteres en el LCD
void WriteMessage(char massage[]){
    int tmp;
    for (int i = 0; i < strlen(massage); i++){
        tmp = (int) massage[i];
        DataWrite(tmp);

    }
}
// Función para escribir un caracter en el LCD
void WriteChar (char caracter){
    DataWrite((int) caracter);
}
