#ifndef LCD_H_
#define LCD_H_

void Instruction(void);
void Data(void);
void LCDInit(void);
void Clear(void);
void WriteMessage(char massage[]);
void DataWrite (int BinNum);
void CmdWrite4 (int BinNum);
void CmdWrite8 (int BinNum);
void setCursor(uint8_t x,  uint8_t y);
void scrollDisplayLeft();
void cursorOn(void);
void cursorOff(void);
void returnHome(void);
void shiftCursoRight(void);
void shiftCursoLeft(void);
void WriteChar (char caracter);

#endif