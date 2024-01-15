//[*]----------------------------------------------------------------------[*]
//   "[*]-----------------------------------------------------[*]",
//   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                     [*]",
//   "[*]  Program : PARK. JongKil   Date : 1998. 1. 19~      [*]",
//   "[*]-----------------------------------------------------[*]",
//   "[*]  DEFAULT CONFIG SETUP FILE                          [*]",
//   "[*]  DIP-SW 0 --> CONFIG_0 Function Select              [*]",
//   "[*]  DIP-SW 1 --> CONFIG_1 Function Select              [*]",
//   "[*]  DIP-SW 2 --> CONFIG_2 Function Select              [*]",
//   "[*]  DIP-SW 3 --> CONFIG_3 Function Select              [*]",
//   "[*]-----------------------------------------------------[*]",
//[*]----------------------------------------------------------------------[*]
//   Sensor Code List(Default.Hpp file Used)
//[*]----------------------------------------------------------------------[*]
//   Used Sensor Name..
//[*]----------------------------------------------------------------------[*]
// TEMPERATURE
// PRECIPITATION
// PRE_DETECT
// PRESSURE
// HUMIDITY
// DEW_POINT
// SUN_RADA
// SUN_DETECT
// EVAPORATION
// GND_HUMIDITY
// SKY_EMISS
// GND_EMISS
//[*]----------------------------------------------------------------------[*]
void config_0(void)
{
	// Sujip Information
	DEFAULT.SUJIP_COUNT             = 600;		// 10 MIN AVERAGE DATA MADE

	// --- CH0 Information ---
	DEFAULT.CH0_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH0_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH0_SENSOR_ID           = 2;
	DEFAULT.CH0_REFERENCE1          = 0;
	DEFAULT.CH0_REFERENCE2          = 0;
	DEFAULT.CH0_ON_OFF              = ON;
	// --- CH1 Information ---
	DEFAULT.CH1_SENSOR_NAME         = PRECIPITATION;
	DEFAULT.CH1_RUN_FUNCTION_NUMBER = PRECIPITATION_0;
	DEFAULT.CH1_SENSOR_ID           = 3;
	DEFAULT.CH1_REFERENCE1          = 0;
	DEFAULT.CH1_REFERENCE2          = 0;
	DEFAULT.CH1_ON_OFF              = ON;
	// --- CH2 Information ---
	DEFAULT.CH2_SENSOR_NAME         = PRE_DETECT;
	DEFAULT.CH2_RUN_FUNCTION_NUMBER = PRE_DETECT_0;
	DEFAULT.CH2_SENSOR_ID           = 4;
	DEFAULT.CH2_REFERENCE1          = 0;
	DEFAULT.CH2_REFERENCE2          = 0;
	DEFAULT.CH2_ON_OFF              = ON;
	// --- CH3 Information ---
	DEFAULT.CH3_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH3_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH3_SENSOR_ID           = 5;
	DEFAULT.CH3_REFERENCE1          = 0;
	DEFAULT.CH3_REFERENCE2          = 0;
	DEFAULT.CH3_ON_OFF              = ON;
	// --- CH4 Information ---
	DEFAULT.CH4_SENSOR_NAME         = HUMIDITY;
	DEFAULT.CH4_RUN_FUNCTION_NUMBER = HUMIDITY_0;
	DEFAULT.CH4_SENSOR_ID           = 6;
	DEFAULT.CH4_REFERENCE1          = 0;
	DEFAULT.CH4_REFERENCE2          = 0;
	DEFAULT.CH4_ON_OFF              = ON;
	// Imagination Channel Information...
	// --- CH5 Information ---
	DEFAULT.CH5_SENSOR_NAME         = DEW_POINT;
	DEFAULT.CH5_RUN_FUNCTION_NUMBER = DEW_POINT_0;
	DEFAULT.CH5_SENSOR_ID           = 7;
	DEFAULT.CH5_REFERENCE1          = 1;
	DEFAULT.CH5_REFERENCE2          = 2;
	DEFAULT.CH5_ON_OFF              = OFF;
	// --- CH6 Information ---
	DEFAULT.CH6_SENSOR_NAME         = SUN_DETECT;
	DEFAULT.CH6_RUN_FUNCTION_NUMBER = SUN_DETECT_0;
	DEFAULT.CH6_SENSOR_ID           = 6;
	DEFAULT.CH6_REFERENCE1          = 3;
	DEFAULT.CH6_REFERENCE2          = 4;
	DEFAULT.CH6_ON_OFF              = OFF;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
void config_1(void)
{
	// Sujip Information
	DEFAULT.SUJIP_COUNT             = 600;		// 10 MIN AVERAGE DATA MADE

	// --- CH0 Information ---
	DEFAULT.CH0_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH0_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH0_SENSOR_ID           = 2;
	DEFAULT.CH0_REFERENCE1          = 0;
	DEFAULT.CH0_REFERENCE2          = 0;
	DEFAULT.CH0_ON_OFF              = ON;
	// --- CH1 Information ---
	DEFAULT.CH1_SENSOR_NAME         = PRECIPITATION;
	DEFAULT.CH1_RUN_FUNCTION_NUMBER = PRECIPITATION_0;
	DEFAULT.CH1_SENSOR_ID           = 3;
	DEFAULT.CH1_REFERENCE1          = 0;
	DEFAULT.CH1_REFERENCE2          = 0;
	DEFAULT.CH1_ON_OFF              = ON;
	// --- CH2 Information ---
	DEFAULT.CH2_SENSOR_NAME         = PRE_DETECT;
	DEFAULT.CH2_RUN_FUNCTION_NUMBER = PRE_DETECT_0;
	DEFAULT.CH2_SENSOR_ID           = 4;
	DEFAULT.CH2_REFERENCE1          = 0;
	DEFAULT.CH2_REFERENCE2          = 0;
	DEFAULT.CH2_ON_OFF              = ON;
	// --- CH3 Information ---
	DEFAULT.CH3_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH3_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH3_SENSOR_ID           = 5;
	DEFAULT.CH3_REFERENCE1          = 0;
	DEFAULT.CH3_REFERENCE2          = 0;
	DEFAULT.CH3_ON_OFF              = ON;
	// --- CH4 Information ---
	DEFAULT.CH4_SENSOR_NAME         = HUMIDITY;
	DEFAULT.CH4_RUN_FUNCTION_NUMBER = HUMIDITY_0;
	DEFAULT.CH4_SENSOR_ID           = 6;
	DEFAULT.CH4_REFERENCE1          = 0;
	DEFAULT.CH4_REFERENCE2          = 0;
	DEFAULT.CH4_ON_OFF              = ON;
	// Imagination Channel Information...
	// --- CH5 Information ---
	DEFAULT.CH5_SENSOR_NAME         = DEW_POINT;
	DEFAULT.CH5_RUN_FUNCTION_NUMBER = DEW_POINT_0;
	DEFAULT.CH5_SENSOR_ID           = 7;
	DEFAULT.CH5_REFERENCE1          = 1;
	DEFAULT.CH5_REFERENCE2          = 2;
	DEFAULT.CH5_ON_OFF              = OFF;
	// --- CH6 Information ---
	DEFAULT.CH6_SENSOR_NAME         = SUN_DETECT;
	DEFAULT.CH6_RUN_FUNCTION_NUMBER = SUN_DETECT_0;
	DEFAULT.CH6_SENSOR_ID           = 6;
	DEFAULT.CH6_REFERENCE1          = 3;
	DEFAULT.CH6_REFERENCE2          = 4;
	DEFAULT.CH6_ON_OFF              = OFF;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
void config_2(void)
{
	// Sujip Information
	DEFAULT.SUJIP_COUNT             = 600;		// 10 MIN AVERAGE DATA MADE

	// --- CH0 Information ---
	DEFAULT.CH0_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH0_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH0_SENSOR_ID           = 2;
	DEFAULT.CH0_REFERENCE1          = 0;
	DEFAULT.CH0_REFERENCE2          = 0;
	DEFAULT.CH0_ON_OFF              = ON;
	// --- CH1 Information ---
	DEFAULT.CH1_SENSOR_NAME         = PRECIPITATION;
	DEFAULT.CH1_RUN_FUNCTION_NUMBER = PRECIPITATION_0;
	DEFAULT.CH1_SENSOR_ID           = 3;
	DEFAULT.CH1_REFERENCE1          = 0;
	DEFAULT.CH1_REFERENCE2          = 0;
	DEFAULT.CH1_ON_OFF              = ON;
	// --- CH2 Information ---
	DEFAULT.CH2_SENSOR_NAME         = PRE_DETECT;
	DEFAULT.CH2_RUN_FUNCTION_NUMBER = PRE_DETECT_0;
	DEFAULT.CH2_SENSOR_ID           = 4;
	DEFAULT.CH2_REFERENCE1          = 0;
	DEFAULT.CH2_REFERENCE2          = 0;
	DEFAULT.CH2_ON_OFF              = ON;
	// --- CH3 Information ---
	DEFAULT.CH3_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH3_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH3_SENSOR_ID           = 5;
	DEFAULT.CH3_REFERENCE1          = 0;
	DEFAULT.CH3_REFERENCE2          = 0;
	DEFAULT.CH3_ON_OFF              = ON;
	// --- CH4 Information ---
	DEFAULT.CH4_SENSOR_NAME         = HUMIDITY;
	DEFAULT.CH4_RUN_FUNCTION_NUMBER = HUMIDITY_0;
	DEFAULT.CH4_SENSOR_ID           = 6;
	DEFAULT.CH4_REFERENCE1          = 0;
	DEFAULT.CH4_REFERENCE2          = 0;
	DEFAULT.CH4_ON_OFF              = ON;
	// Imagination Channel Information...
	// --- CH5 Information ---
	DEFAULT.CH5_SENSOR_NAME         = DEW_POINT;
	DEFAULT.CH5_RUN_FUNCTION_NUMBER = DEW_POINT_0;
	DEFAULT.CH5_SENSOR_ID           = 7;
	DEFAULT.CH5_REFERENCE1          = 1;
	DEFAULT.CH5_REFERENCE2          = 2;
	DEFAULT.CH5_ON_OFF              = OFF;
	// --- CH6 Information ---
	DEFAULT.CH6_SENSOR_NAME         = SUN_DETECT;
	DEFAULT.CH6_RUN_FUNCTION_NUMBER = SUN_DETECT_0;
	DEFAULT.CH6_SENSOR_ID           = 6;
	DEFAULT.CH6_REFERENCE1          = 3;
	DEFAULT.CH6_REFERENCE2          = 4;
	DEFAULT.CH6_ON_OFF              = OFF;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
void config_3(void)
{
	// Sujip Information
	DEFAULT.SUJIP_COUNT             = 600;		// 10 MIN AVERAGE DATA MADE

	// --- CH0 Information ---
	DEFAULT.CH0_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH0_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH0_SENSOR_ID           = 2;
	DEFAULT.CH0_REFERENCE1          = 0;
	DEFAULT.CH0_REFERENCE2          = 0;
	DEFAULT.CH0_ON_OFF              = ON;
	// --- CH1 Information ---
	DEFAULT.CH1_SENSOR_NAME         = PRECIPITATION;
	DEFAULT.CH1_RUN_FUNCTION_NUMBER = PRECIPITATION_0;
	DEFAULT.CH1_SENSOR_ID           = 3;
	DEFAULT.CH1_REFERENCE1          = 0;
	DEFAULT.CH1_REFERENCE2          = 0;
	DEFAULT.CH1_ON_OFF              = ON;
	// --- CH2 Information ---
	DEFAULT.CH2_SENSOR_NAME         = PRE_DETECT;
	DEFAULT.CH2_RUN_FUNCTION_NUMBER = PRE_DETECT_0;
	DEFAULT.CH2_SENSOR_ID           = 4;
	DEFAULT.CH2_REFERENCE1          = 0;
	DEFAULT.CH2_REFERENCE2          = 0;
	DEFAULT.CH2_ON_OFF              = ON;
	// --- CH3 Information ---
	DEFAULT.CH3_SENSOR_NAME         = TEMPERATURE;
	DEFAULT.CH3_RUN_FUNCTION_NUMBER = TEMPERATURE_0;
	DEFAULT.CH3_SENSOR_ID           = 5;
	DEFAULT.CH3_REFERENCE1          = 0;
	DEFAULT.CH3_REFERENCE2          = 0;
	DEFAULT.CH3_ON_OFF              = ON;
	// --- CH4 Information ---
	DEFAULT.CH4_SENSOR_NAME         = HUMIDITY;
	DEFAULT.CH4_RUN_FUNCTION_NUMBER = HUMIDITY_0;
	DEFAULT.CH4_SENSOR_ID           = 6;
	DEFAULT.CH4_REFERENCE1          = 0;
	DEFAULT.CH4_REFERENCE2          = 0;
	DEFAULT.CH4_ON_OFF              = ON;
	// Imagination Channel Information...
	// --- CH5 Information ---
	DEFAULT.CH5_SENSOR_NAME         = DEW_POINT;
	DEFAULT.CH5_RUN_FUNCTION_NUMBER = DEW_POINT_0;
	DEFAULT.CH5_SENSOR_ID           = 7;
	DEFAULT.CH5_REFERENCE1          = 1;
	DEFAULT.CH5_REFERENCE2          = 2;
	DEFAULT.CH5_ON_OFF              = OFF;
	// --- CH6 Information ---
	DEFAULT.CH6_SENSOR_NAME         = SUN_DETECT;
	DEFAULT.CH6_RUN_FUNCTION_NUMBER = SUN_DETECT_0;
	DEFAULT.CH6_SENSOR_ID           = 6;
	DEFAULT.CH6_REFERENCE1          = 3;
	DEFAULT.CH6_REFERENCE2          = 4;
	DEFAULT.CH6_ON_OFF              = OFF;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
