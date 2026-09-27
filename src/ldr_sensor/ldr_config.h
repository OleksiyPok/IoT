// src/ldr_sensor/ldr_config.h

#pragma once

// ---------------------------------

#define LDR_ADC_VALID_MIN 50    // device error
#define LDR_LUX_VALID_MIN 1     // device error
#define LDR_LUX_VALID_MAX 70000 // depends on current conditions
#define LDR_ADC_VALID_MAX 4045  // depends on current conditions

#define LDR_LUX_ALARM_MIN 10
#define LDR_LUX_THRESHOLD_LIGHT_LOW 600
#define LDR_LUX_ALARM_MAX 10000