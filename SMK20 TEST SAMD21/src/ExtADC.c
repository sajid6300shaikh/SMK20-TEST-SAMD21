#include "compiler.h"
#include "ExtADC.h"

int32_t ExtAnalogRead(EnumAdcChannel ReqChannel){
	//here channel is variable of type EnumAdcChannel
	switch(ReqChannel){
		case Ch_Orifice_DP:
		return(-32769);
		break;
		
		case Ch_Orifice_Gas_DP:
		return(2);	
		break;
		
		case Ch_Pitot_DP:
		return(3);
		break;
		
		case Ch_Stack_TC:
		return(4);
		break;
		
		case Ch_Absolute_Pres:
		return(5);
		break;
		
		case Ch_Orifice_PT100:
		return(6);
		break;
		
		case Ch_Ambient_PT100:
		return(7);
		break;
		
		case Ch_Aux_PT100:
		return(8);
		break;
		
		default:
		return(0);	
	}
	
}