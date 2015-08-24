#include "GlobalVarsnPrototypes.h"
#include "Delay.h"
#include "S_PORT.h"
#include "S_GCLOCK.h"
#include "CharLCD.h"
#include "MatrixKeypad.h"
#include "ExtADC.h"
#include "LCDAdvanced.h"
#include "UserInterface.h"
#include "EEPmap.h"
#include "Calibration.h"

#define Passw	1

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



void Cal_Disp2Points(volatile nAZSensors *Sensor){
	//Lower Count
	LCD_DispVariable(Sensor->ADCLower,0,5,3,7);
	//STD LOWER
	LCD_DispVariable(Sensor->STDLower,0,3,3,18);
	
	//Upper Count
	LCD_DispVariable(Sensor->ADCUpper, 0,5,4,7);
	
	//Std Upper
	LCD_DispVariable(Sensor->STDUpper,0,3,4,18);
};

void Cal_Edit2Points(volatile nAZSensors *Sensor){
	int32_t EnteredData=0;
	Cal_Disp2Points(Sensor);
	
	EnteredData=GetNumDataFromUserWithESC(Sensor->ADCLower,0,5,3,7);
	if (EnteredData==-1)	//ESC key is pressed
	goto RETWoSaving;
	else					//ENTER KEY IS PRESSED
	Sensor->ADCLower=EnteredData;	//store ADC lower Data in respective sensors variable
	
	EnteredData=GetNumDataFromUserWithESC(Sensor->STDLower, 0, 3, 3,18);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	Sensor->STDLower=EnteredData;	//Store Std Lower Data
	
	EnteredData=GetNumDataFromUserWithESC(Sensor->ADCUpper, 0, 5, 4, 7);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	Sensor->ADCUpper=EnteredData;	//store ADCUpper counts
	
	EnteredData=GetNumDataFromUserWithESC(Sensor->STDUpper, 0, 3, 4, 18);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	Sensor->STDUppe
	
	r=EnteredData;	//store StdUpper counts
	
	
	RETWoSaving:
	return;
}

void Cal_AbsEdit(void){		//Absolute Pres Edit Cal
	int32_t EnteredData=0;
	Cal_Disp2Points(&Absolute_Pres);
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.ADCLower,0,5,3,7);
	if (EnteredData==-1)	//ESC key is pressed
	goto RETWoSaving;
	else
	{	//ENTER KEY IS PRESSED, save value in EEP
		Absolute_Pres.ADCLower=EnteredData;	//store ADC lower Data in respective sensors variable		
	}	
	
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.STDLower, 0, 3, 3,18);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		Absolute_Pres.STDLower=EnteredData;	//Store Std Lower Data
	}
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.ADCUpper, 0, 5, 4, 7);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		Absolute_Pres.ADCUpper=EnteredData;	//store ADCUpper counts
	}
	
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.STDUpper, 0, 3, 4, 18);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		Absolute_Pres.STDUpper=EnteredData;	//store StdUpper counts
	}
	
	RETWoSaving:
	return;
}

void Calibrate_AbsPres(){
	
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"  Calibration of:   ",
		" Absolute Pressure  ",
		"Scan / Edit /Execute",
		"                    ");
		SelOpt =LCD_HorizontalScroll(1, 3,1,3,8,3,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan Mode: Abs.Pres.",
			"Calibration(in mmHg)",
			"Lower:       Std:   ",
			"Upper:       Std:   ");
			//display the calibration values.
			Cal_Disp2Points(&Absolute_Pres);
			WaitforAnyKey();
			ReleaseKey();
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Scan Mode: Abs.Pres.",
			"Calibration(in mmHg)",
			"Lower:       Std:   ",
			"Upper:       Std:   ");
			Cal_AbsEdit();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"ONLINE NOT IMPLEMENT");
			AnykeyPulse();
		}
	} while (SelOpt);
}

void Calibrate_PartDP(){
	
}
void DoCalibration(){
	int8_t selected=0;
	LCD_CursorOff();
	do
	{
		selected =LCD_MenuHandle(MenuSize(CalibrationMenu),CalibrationMenu);
		
		switch(selected){
			case 1:
			Calibrate_AbsPres();
			break;
			case 2:
			Calibrate_PartDP();
			break;
			case 3:
			//			Calibrate_GasDP();
			break;
			case 4:
			//			Calibrate_PitotDP();
			break;
			case 5:
			//			Calibrate_StackTC();
			break;
			case 6:
			//		Calibrate_AmbTemp();
			break;
			case 7:
			//		Calibrate_OrifTemp();
			break;
			case 8:
			//		Calibrate_AuxTemp();
			break;
			case 9:
			//			Calibrate_PartFlow();
			break;
			case 10:
			//		Calibrate_GasFlow();
			break;
			case 0:
			break;
			default:
			LCD_OneLine(2,"Invalid Cal Option");
		}
	} while (selected!=0);
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

