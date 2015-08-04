/************************************************************************/
/*	Project Name: STACK MONITORING KIT 
	STARTED ON: JUNE, 2015
	MODEL NAME: PEM-SMK 20
	Version: 1.0
	AUTHOR: SAJID SHAIKH
	COMPANY: POLLTECH INTRUMENTS PVT. LTD.,
			 MULUND(WEST), MUMBAI-400080 */
/************************************************************************/
//#define BypassLCD

#include <asf.h>
#include "GlobalVarsnPrototypes.h"
#include "Delay.h"
#include "S_PORT.h"
#include "S_GCLOCK.h"
#include "CharLCD.h"
//#include "EEProm.h"
#include "MatrixKeypad.h"
#include "ExtADC.h"
#include "LCDAdvanced.h"
#include "Calculation.h"






void config_timer(void);
void config_lcd(void);

uint8_t  bell[8]={4,14,14,14,31,0,4};
uint8_t  smiley[8]={0,0,10,0,17,14,0,0};
uint8_t  polltech[8]={24,24,0,31,25,31,24,24};
uint8_t  framelement[8]={10,10,10,10,10,10,10,10};
uint8_t	degree[8]={12, 18, 18, 12, 0, 0, 0, 0};

	
const char MSG[][LCDCOLS]={	{"1. Line in Lcd disp"}, 
							{"2. Line in lCd disp"}, 
							{"3. Line in lcD disp"},
							{"4. Line in LCD Disp"},
							{"5. Option.........."}	
					  };
					  
const char ms[]={"Hello world"};

const char CalibrationMenu[][21]=
					{	
						{"  Calibration of:  "},	//0
						{"1.Absolute Pressure"},	//1
						{"2.Parti. Orif.DP   "},	//2
						{"3.Gaseous Orif.DP  "},	//3
						{"4.Pitot DP         "},	//4
						{"5.Stack-T/C        "},	//5
						{"6.Ambient Temp.    "},	//6
						{"7.Meter Temp.      "},	//7
						{"8.Auxillary Temp.  "},	//8
						{"9.Particulate Flow "},	//9
						{"10.Gaseous Flow    "}		//10			
							
					};


					





/************************************************************************/
/* Sets up Timer to output PWM wave to drive LCD contrast charge pump*/
/************************************************************************/
void Config_Contrast_PWM(void){
	
	//Enable TC5 in power manager
	REG_PM_APBCMASK |= (1<<13);		
	
	//Assign GCLK0 to Timer //GCLK0 runs at 48 MHz
	GCLKGen_Peripheral(GCLKGEN0 , GCLK_TC4_GCLK_TC5);
	
	//Reset Timer
	REG_TC5_CTRLA=(1<<0);	
	//	Start Timer
	REG_TC5_CTRLBSET=(1<<6);
	
	//PWM output pin
	pinMux(PB15,ETCmux);			
	
					//presync |prescale|wave mode|TC Mode| Enable
	REG_TC5_CTRLA = (0<<12) | (5<<8) | (2<<5) | (1<<2) | (1<<1);
	
	//127 =50%, will need value from EEPROM
	REG_TC5_COUNT8_CC1= 127;	 //This register's not enable protected
}

/***************************************************************************************************/
/*	Contrast value will be between 0 to 5
	Hence corresponding PWM value will be between 0 to 130
	which gives 0 to 51% duty cycle, charge pump's o/p(-ve voltage) is highest with 50 % duty cycle
	So LCD contrast is highest with 50 % duty cycle */
/***************************************************************************************************/
void Set_LcdContrast(uint8_t ContrastValue){

	REG_TC5_COUNT8_CC1= 26*ContrastValue;
}
/************************************************************************/
/* Initializes LCD, Contrast, and Builds some required symbols          */
/************************************************************************/
void Config_LCD(void){
	Config_Contrast_PWM();
	//LCD_Init();
	LCD_Init();
	LCD_CursorOff();
	//Build symbol in LCD's CGRAM to be used later on
	LCD_build(0, framelement);	//build '||' symbol at location 0, used to draw frame
	LCD_build(1, polltech);		//build Polltech LOGO at location 1
	LCD_build(2,degree);
}

void Disp_IntroLcdMsgs(void){
	LCD_PowerOff();
	LCD_Frame();			//make good looking frame around LCD
	
	LCD_Setcursor(1,9);
	LCD_Print("   ");
	LCD_Setcursor(1,10);
	LCD_DataWrite(1);		//prints polltech logo that is stored at location 1
	LCD_Setcursor(2,7);
	LCD_Print("Polltech    ");
	LCD_Setcursor(3,6);
	LCD_Print("Instruments   ");
	LCD_PowerOn();
	delay_sec(1);
	LCD_Setcursor(2,2);
	LCD_Print("StackMonitoringKit");
	LCD_Setcursor(3,2);
	LCD_Print(" Model PEM-SMK 20 ");
	
	delay_sec(1);
}

void DoAutoZero(void){
		
	LCD_FullDisp(	" Ready for Auto-Zero",
					"(Keep Pitob Ass. Out",
					"& Sampling Pump Off)",
					"  Press Enter Key   ");		
	
	//w8 for user to press ENTER key
	WaitFor(EnterKey);
	//Enter key pressed, go to AUTOZERO of Pitot_DP, Orifice_DP and Orifice_GAS_DP.

	//User is expected to remove any input to system, hence read values are offsets
	LCD_FullDisp(	"  Doing Auto-Zero   ",
					"(Keep Pitob Ass. Out",
					"& Sampling Pump Off)",
					"                    ");	
	delay_sec(1);
	
	Orifice_DP.Zero= ExtAnalogRead(Ch_Orifice_DP);
	LCD_Setcursor(4,2);
	LCD_Print("------");	//Graphic loading Effect ;)
	delay_sec(1);
	Pitot_DP.Zero=	ExtAnalogRead(Ch_Pitot_DP);
	LCD_Print("------");
	delay_sec(1);
	Orifice_Gas_DP.Zero= ExtAnalogRead(Ch_Orifice_Gas_DP);
	LCD_Print("------");
	LCD_FullDisp(	"                    ",	
					"   Auto-Zero done   ",
					"  Press Enter Key   ",				
					"                    ");
	WaitFor(EnterKey);
}

uint8_t LCD_MenuHandle(const char MenuOptions[][LCDCOLS])//, uint8_t TotalOptions, uint8_t NoofOptionstoScroll, uint8_t Index){
	{
		//volatile uint8_t TotalOptions=sizeof(MenuOptions);
		//uint8_t ScrollStartRow= LCDRows+1-NoofItemstoScroll;
		
		LCD_Print(MenuOptions[4]);
		
		
		
		return(0);
	}


void LCD_MenuDisplay(const char MenuOptions[][LCDCOLS], uint8_t StartLine){
	//	LCD_FullDisp(MenuOptions[StartLine]);

	}


int main (void)
{
	system_init();
	
	Config_MatrixKeypad();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	#ifndef BypassLCD

	Config_LCD();	
	
	Disp_IntroLcdMsgs();	//Display company name, model name 
	//DoAutoZero();
	LCD_Clrscr();
	#endif
	LCD_Setcursor(3,20);
	LCD_DataWrite(ARROW);
	
	LCD_Setcursor(1,1);
	LCD_CursorOn();
//	LCD_MenuHandle(MSG);
//	LCD_FullDisp(MSG[0],MSG[1],MSG[2],MSG[3]);
	LCD_FullDisp(CalibrationMenu[0],CalibrationMenu[1],CalibrationMenu[2],CalibrationMenu[3]);
	LCD_MenuDisplay(CalibrationMenu, 4);
while (1)
{
}
	// Insert application code here, after the board has been initialized.
}



