#ifndef _CHAR_LCD_H
#define _CHAR_LCD_H

#include "S_PORT.h"
#define	ENPulse	2			//delay between LCD Enable high and low

/************************************************************************/
/*		Hardware definitions                                            */
/************************************************************************/ 
//Any random pin can be assigned to LCDBUS
//It need not be consecutive pins, even pins of different port can be assigned

#define LCD_D7			PA19		//Busy flag
#define LCD_D6			PA18		
#define LCD_D5			PA17		
#define LCD_D4			PA16		

#define LCD_RS			PA10
#define LCD_EN			PA20
#define LCD_RWb			PA21

// Old way requiring Consecutive port pins
//#define LCD_BUS			REG_PORT_OUT0	//write to lcd_bus means write to PORTA (change it to reg_port_out1 for port B)

////Software Definitions
//#define LCD_BUS_Mask	0x000f0000		//In binary it is 1111 at position 16,17,18,19(i.e. pins connected to LCD D4...D7).
//#define LCD_Shift		16				//since bus starts at pin 16 connected to D4 of LCD, we will shift the nibble to be sent to LCD by this many positions

uint8_t _data_pins[4];				//array used in low level LCD_Command and data write

void LCD_DataWrite(uint8_t Data);
void LCD_Init(void);
void PulseLCD_EN(void);
void write4bits(uint8_t value);
void LCD_CmdWrite(uint8_t Command );
void checkbusy(void);
void LCD_Setcursor(char Row, char Column);
void LCD_Print(const unsigned char *str);
void LCD_DispAscii(char a);
void LCD_CursorOn(void);
void LCD_CursorOff(void);
void LCD_CursorBlink(void);
void LCD_FullDisp(const unsigned char *str0,const unsigned char *str1,const unsigned char *str2,const unsigned char *str3 );
void LCD_build(uint8_t location, uint8_t *ptr);
void LCD_build2(void);
void LCD_Clrscr(void);
void LCD_Frame(void);
void LCD_4BitCmdWrite(uint8_t Command );
void LCD_PowerOff(void);
void LCD_PowerOn(void);
void LCD_Putchar(char Data);

#endif