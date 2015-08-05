#include "CharLCD.h"
#include "LCDAdvanced.h"
#include "MatrixKeypad.h"

/************************************************************************/
/* Has all high level function of LCD to Handle Menu based UI			*/
/************************************************************************/

void LCD_DispFourOptionsIncrementingFrom(uint8_t StartLine, const char MenuOptions[][LCDCOLS]){
	for (char i=0; i<=3; i++)
	{
		LCD_Setcursor(i+1,1);
		LCD_Print(MenuOptions[StartLine+i]);
	}
	
}

void LCD_DispMenuArrow(uint8_t Row){
	//arrow symbol pointing menu option is shown at extreme right on LCD
	//Clear other Rows
	LCD_Setcursor(1,20);
	LCD_DataWrite(' ');
	LCD_Setcursor(2,20);
	LCD_DataWrite(' ');
	LCD_Setcursor(3,20);
	LCD_DataWrite(' ');
	LCD_Setcursor(4,20);
	LCD_DataWrite(' ');
	
	LCD_Setcursor(Row,20);
	LCD_DataWrite(127);
}

int8_t CheckMenuIndexBounds(int8_t MenuIndex, int8_t TotalOptions){
	if (MenuIndex<1)
	MenuIndex=TotalOptions;
	else if (MenuIndex>TotalOptions)
	MenuIndex=1;
	return MenuIndex;
}

uint8_t LCD_MenuHandle(uint8_t TotalOptions, const char MenuOptions[][LCDCOLS])
{
	//TotalOptions is nothing but no. of items in menu
	//Keeps actual option no. from 1 to TotalOptions
	int8_t MenuIndex=1;		//keep it signed to facilitate bounds checking
	//keeps row no where arrow symbol is displayed (1-4, since 4 line LCD).
	int8_t ArrowLoc=1;		//kept it signed to facilitate bounds checking
	//In LCD_DispOptionsIncrementingFrom(optionNo, OptionStringArray) DispOptionno is used to keep track of displayed MSG group
	int8_t DispOptionNo=0;
	LCD_DispFourOptionsIncrementingFrom(0, MenuOptions);	//displays Strings from 0 to 3 of the given string 2D array
	//displays arrow symbol according to location on LCD at 20th Column and Row given by ArrowLoc
	LCD_DispMenuArrow(ArrowLoc);
	
	while(1){	//loop forever, this function will be exited by press EnterKey or ParaKey
		if(KeyDetected())
		{//comes here when key is pressed
			Keyscan();		//check which key is pressed
			if(IncrKey)		//increment key pressed
			{
				MenuIndex--;
				ArrowLoc--;
				MenuIndex=CheckMenuIndexBounds(MenuIndex, TotalOptions);	//Adjust MenuIndex if it is below zero or above Totaloptions
				if (ArrowLoc<1)
				{
					if (DispOptionNo==0)
					{
						DispOptionNo=TotalOptions-4;
						ArrowLoc=4;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);						
					}
					else
					{
						DispOptionNo--;
						ArrowLoc=1;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);						
					}
					
				}
			}

			else if(DecrKey)
			{
				MenuIndex++;
				ArrowLoc++;
				MenuIndex=CheckMenuIndexBounds(MenuIndex, TotalOptions);
				if (ArrowLoc>4)
				{
					if (DispOptionNo==(TotalOptions-4))
					{
						DispOptionNo=0;
						ArrowLoc=1;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);
						
					}
					else
					{
						DispOptionNo++;
						ArrowLoc=4;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);
						
					}
					
				}
			}
			
			else if(EnterKey)
			{
				return MenuIndex;
			}
			else if (ParaKey)
			{
				return 0;
			}
			LCD_DispMenuArrow(ArrowLoc);
		}
	}
}
