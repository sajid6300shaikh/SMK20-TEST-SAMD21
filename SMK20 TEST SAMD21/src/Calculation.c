#include "compiler.h"
#include "GlobalVarsnPrototypes.h"
#include "Calculation.h"

uint32_t Zero_Correction(int32_t AdcCount, int32_t	ZeroCount){
	return(AdcCount-ZeroCount);	
}

/************************************************************************/
/* After Zero and Span Values are read from EEPROM Slope value of all sensors will be calculated once*/
/************************************************************************/
void ComputeEquation_ofall_Sensors(void){
	//AZ sensors requiring Slope from 1 point equation
	Orifice_DP.Slope	=ComputeSlopefrom1point(&Orifice_DP);
	Orifice_Gas_DP.Slope=ComputeSlopefrom1point(&Orifice_Gas_DP);
	Pitot_DP.Slope		=ComputeSlopefrom1point(&Pitot_DP);
	
	//nAZ sensors requiring slope form 2 point equation
	Absolute_Pres.Slope	=ComputeSlopefrom2Point(&Absolute_Pres);
	Stack_TC.Slope		=ComputeSlopefrom2Point(&Stack_TC);
	Orifice_PT100.Slope	=ComputeSlopefrom2Point(&Orifice_PT100);
	Ambient_PT100.Slope	=ComputeSlopefrom2Point(&Ambient_PT100);
	Aux_PT100.Slope		=ComputeSlopefrom2Point(&Aux_PT100);
	
	//nAZ Sensors requiring Intercept form Slope-point equation
	Absolute_Pres.Intercept	=ComputeIntercept(&Absolute_Pres);
	Stack_TC.Intercept		=ComputeIntercept(&Stack_TC);
	Orifice_PT100.Intercept	=ComputeIntercept(&Orifice_PT100);
	Ambient_PT100.Intercept	=ComputeIntercept(&Ambient_PT100);
	Aux_PT100.Intercept		=ComputeIntercept(&Aux_PT100);
}


/********************************************************************************************************************/
/* Calculates Sensor.Slope value for AZSensors, since they have 1 point calibration, the eqn is given by
Slope=STDupper/ADCUpper;
usage:
	Orifice_DP.Slope	=ComputeSlopefrom1point(&Orifice_DP);*/
/********************************************************************************************************************/
float ComputeSlopefrom1point(volatile AZSensors *Sensor){
	return((float)Sensor->STDUpper/Sensor->ADCUpper);
}

/*********************************************************************************************************/
/* Calculates nAZ Sensor.slope value from two points since they have 2 point calibration, their eqn is,
Slope= (STDUpper-STDLower) / (ADCUpper-ADCLower);*/
/*********************************************************************************************************/
float ComputeSlopefrom2Point(volatile nAZSensors *Sensor){
	return((float) (Sensor->STDUpper - Sensor->STDLower)/(Sensor->ADCUpper-Sensor->ADCLower));
}

/************************************************************************/
/* Calculates Intercept for nAZSensors
Intercept=STDUpper - Slope*ADCUpper
							****WARNING***
This Function requires Slope value, so ComputeSlopefrom2Point should be called before this function
i.e. Slope value for nAZ Sensors should be calculated first.*/
/************************************************************************/
float ComputeIntercept(volatile nAZSensors *Sensor){
	return((float) Sensor->STDUpper - (Sensor->Slope * Sensor->ADCUpper));
}