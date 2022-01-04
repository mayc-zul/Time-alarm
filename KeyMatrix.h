#ifndef KEYMATRIX_H_
#define KEYMATRIX_H_

// Counter mod-10 for debouncing and for the time between two consecutive executions
// of function KBI_ReadKey 
#define CNT_DEBOUNCE			205		// 10 ms -> RTC: 1ms
#define CNT_SPLITREAD			1		// 1 ms

// Columns configuration: C0, C1, C2, C3 -> GPIO5, GPIO4, GPIO3, GPIO2
// Rows configuration: R0, R1, R2, R3 -> GPIO9, GPIO8, GPIO7, GPIO6


void keyMatrixInit(void);
char keyMatrixRead(void);

extern volatile bool gpio_req;
extern bool ProcesarTecla;
extern unsigned char cnt_ProcessKey;


#endif
