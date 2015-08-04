
/************************************************************************/
/* These are various ADC channels */
/************************************************************************/
typedef volatile enum {// start numbering with 1
	Ch_Orifice_DP=1,		//stands for channel orifice_dp
	Ch_Orifice_Gas_DP,
	Ch_Pitot_DP,
	Ch_Stack_TC,
	Ch_Absolute_Pres,
	Ch_Orifice_PT100,
	Ch_Ambient_PT100,
	Ch_Aux_PT100
}EnumAdcChannel;

	

int32_t ExtAnalogRead(EnumAdcChannel Channel);