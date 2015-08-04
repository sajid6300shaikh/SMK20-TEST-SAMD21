#ifndef _Calculation_h
#define _Calculation_h

uint32_t Zero_Correction(int32_t AdcCount, int32_t ZeroCount);
void ComputeEquation_ofall_Sensors(void);
float ComputeSlopefrom1point(volatile AZSensors *Sensor);
void ComputeSlopeIntercept_ofAll_NAZSensors(void);
float ComputeSlopefrom2Point(volatile nAZSensors *Sensor);
float ComputeIntercept(volatile nAZSensors *Sensor);

#endif