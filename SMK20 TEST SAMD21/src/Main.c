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
#include <math.h>
#include "GlobalVarsnPrototypes.h"
#include "Delay.h"
#include "S_PORT.h"
#include "S_GCLOCK.h"
#include "CharLCD.h"
#include "EEProm.h"
#include "MatrixKeypad.h"
#include "ExtADC.h"
#include "LCDAdvanced.h"
#include "Calculation.h"
#include "UserInterface.h"
#include "EEPmap.h"

void config_timer(void);
void config_lcd(void);

uint8_t  bell[8]={4,14,14,14,31,0,4};
uint8_t  smiley[8]={0,0,10,0,17,14,0,0};
uint8_t  PILOGO[8]={24,24,0,31,25,31,24,24};
uint8_t  framelement[8]={10,10,10,10,10,10,10,10};
uint8_t	DegreeSym[8]={12, 18, 18, 12, 0, 0, 0, 0};
	
const char MSG[][LCDCOLS]=	{	{"1. Line in Lcd disp"}, 
								{"2. Line in lCd disp"}, 
								{"3. Line in lcD disp"},
								{"4. Line in LCD Disp"},
								{"5. Option.........."}	
							};

const char ms[]={"Hello world"};

const char CalibrationMenu[][20]=
					{								
						{"1.Absolute Pressure"},	//0
						{"2.Parti. Orif.DP   "},	//1
						{"3.Gaseous Orif.DP  "},	//2
						{"4.Pitot DP         "},	//3
						{"5.Stack ThermoCup  "},	//4
						{"6.Ambient Temp.    "},	//5
						{"7.Meter Temp.      "},	//6
						{"8.Auxillary Temp.  "},	//7
						{"9.Particulate Flow "},	//8
						{"10.Gaseous Flow    "},	//9													
					};


#define Passw	1	
									
/************************************************************************/
/* Sets up Timer to output PWM wave to drive LCD contrast charge pump*/
/************************************************************************/
void Config_Contrast_PWM(void){
	
	//Enable TC5 in power managers
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
	//Config_Contrast_PWM();	//not needed for OLED, since no contrast pin
	//LCD_Init();
	LCD_Init();
	LCD_CursorOff();
	//Build symbol in LCD's CGRAM to be used later on
	LCD_build(0, framelement);	//build '||' symbol at location 0, used to draw frame
	LCD_build(1, PILOGO);		//build Polltech LOGO at location 1
	LCD_build(2,DegreeSym);
}

void Disp_IntroLcdMsgs(void){
	LCD_PowerOff();
	LCD_Frame1();			//make good looking frame around LCD
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
	
	OrifPart_DP.Zero= ExtAnalogRead(Ch_Orifice_DP);
	LCD_Setcursor(4,2);
	LCD_Print("------");	//Graphic loading Effect ;)
	delay_sec(1);
	Pitot_DP.Zero=	ExtAnalogRead(Ch_Pitot_DP);
	LCD_Print("------");
	delay_sec(1);
	OrifGas_DP.Zero= ExtAnalogRead(Ch_Orifice_Gas_DP);
	LCD_Print("------");
	LCD_FullDisp(	"                    ",	
					"   Auto-Zero done   ",
					"  Press Enter Key   ",				
					"                    ");
	WaitFor(EnterKey);
}


void DoCalibration(){
	int8_t selected=0;
	LCD_CursorOff();
	selected =LCD_MenuHandle(MenuSize(CalibrationMenu),CalibrationMenu);
	
	switch(selected){
		case 1:
			Calibrate_AbsPres();
			break;
		case 2:
			Calibrate_PartDP();
			break;
		case 3:
			Calibrate_GasDP();
			break;
		case 4:
			Calibrate_PitotDP();
			break;
		case 5:
			Calibrate_StackTC();
			break;
		case 6:
			Calibrate_AmbTemp();
			break;
		case 7:
			Calibrate_OrifTemp();
			break;
		case 8:
			Calibrate_AuxTemp();
			break;
		case 9:
			Calibrate_PartFlow();
			break;
		case 10:
			Calibrate_GasFlow();
			break;
		default:
			LCD_OneLine(2,"Invalid Cal Option");
	}
	
}


void CalibrationModeChk(){	//chk for key combo and password
	//check if key combo is pressed and proceed with calibration
	int32_t EnteredPass=0;
if (KeyDetected())
{ //chk for key combo is any key press is detected
	KeyscanWORelese();
if ((ParaKey&&UpdateKey)&&!(RunKey||StopKey||StartKey||EnterKey)) //enter cal only when para and update key is pressed
{	//calibration key combination  detected
	
	LCD_Clrscr();
	LCD_OneLine(1,"  Calibration Mode  ");
	//wait for key combo to be released
	Release(ParaKey||UpdateKey);
	do
	{
		LCD_FullDisp(	"  Calibration Mode  ",
						"   Enter Password   ",
						  LCD_BLANK_LINE_MACRO,
						  LCD_BLANK_LINE_MACRO);
		
		EnteredPass=GetNumDataFromUserWithESC(0,0,7,3,8);
		if (EnteredPass==-1)	//-1 is just  used to check when esc key is pressed
		{
			return;			//exit cal mode if para/ESC key is pressed
		}
		
		else if (EnteredPass!=Passw)
		{
			LCD_OneLine(4,"   Wrong Password!! ");
			delay_sec(1);
			LCD_OneLine(4,LCD_BLANK_LINE_MACRO);
		}
	}while(EnteredPass!=Passw);
		
	if (EnteredPass==Passw)
		DoCalibration();

}	//if (keycombo chk) close brac
}	//if (keydetect) close brac
}	//function close brac



int main (void)
{
	system_init();
	
	Config_MatrixKeypad();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	#ifndef BypassLCD
	Config_LCD();	
	configure_EEPROM();
	CalibrationModeChk();
	Disp_IntroLcdMsgs();	//Display company name, model name 
	
//	DoAutoZero();
	LCD_Clrscr();
	#endif
	LCD_Setcursor(3,20);
	LCD_DataWrite(ARROW);
	
	LCD_Setcursor(1,1);

//	LCD_CursorOn();
	LCD_FullDisp(	"  Calibration of:   ",
					" Absolute Pressure  ",
					"Scan / Edit /Execute",
					"                    ");
					
	//
	//LCD_FullDisp(	LCD_BLANK_LINE_MACRO,
	//" Erase stored data? ",
	//"    Yes   /   No    ",
	//LCD_BLANK_LINE_MACRO);
	//LCD_Frame2();
						
	
volatile uint8_t selected=0;
	selected =LCD_HorizontalScroll(1, 3,1,3,8,3,14);
//	selected =LCD_MenuHandle(MenuSize(CalibrationMenu),CalibrationMenu);
//	LCD_FullDisp(MSG[0],MSG[1],MSG[2],MSG[3]);
//	LCD_FullDisp(CalibrationMenu[0],CalibrationMenu[1],CalibrationMenu[2],CalibrationMenu[3]);
//	LCD_MenuDisplay(CalibrationMenu, 4);
	
	LCD_Setcursor(1,2);
	LCD_CursorOn();
	LCD_DispAscii(selected);
//	LCD_DispVariable(12345,2,10,0,1);

	Ambient_PT100.STDvalue=10021;
	uint32_t ans=GetNumDataFromUser(32768,2,7,1,10);	
	LCD_DispVariable(ans, 2, 7, 3, 10);
	volatile uint8_t a,b,c,d;

	a=EE_MISCAddr(c);
	b=EE_CALAddr(EE_OrifPart_Z);
	c=EE_CALAddr(EE_AuxTemp_STDUP);
	d=EE_CALAddr(EE_Abs_ADCLOW);
	
while (1)                                                                                                                                                               
{	
	
}
	// Insert application code here, after the board has been initialized.
}