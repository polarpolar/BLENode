/****************************************/
/* Configure Funcion for UV_Meter(head)	*/
/* Light Sensor													*/
/* Author: Wu Tongda										*/
/* Tsinghua EE NICS											*/
/* Final Time: 2014/09/04								*/
/****************************************/

#include "reg51.h"
#include "I2C.h"
#include "delay.h"
#include "stdio.h"

// PINS
//#define INT INT0									// INT pin connects the INT0 of 0xB2

// Param Name 
#define	SLAVE_ADDR					0x60
#define CHLIST							0x01	// enable signal(uv, aux, als_ir, als_vis)
#define ALS_ENCODING				0x06	// select vis/ir
#define ALS_IR_ADCMUX				0x0E	// IR input
#define AUX_ADCMUX					0x0F	// AUX input:Temp, Vdd
#define ALS_VIS_ADC_COUNTER	0x10	// recover for ADC(VIS)
#define ALS_VIS_ADC_GAIN 		0x11	// recover for ADC(VIS)
#define ALS_VIS_ADC_MISC 		0x12	// range(VIS)
#define ALS_IR_ADC_COUNTER 	0x1D	// recover for ADC(IR)
#define ALS_IR_ADC_GAIN			0x1E	// recover for ADC(IR)
#define ALS_IR_ADC_MISC 		0x1F	// range(IR)

// ALS_ENCODING
#define ALS_VIS_ALIGN				0x10	// report 16bits ALS_VIS
#define ALS_IR_ALIGN				0x20	// report 16bits ALS_IR

// Instructions - Command Register
#define PARAM_QUERY					0x80	// Param read at add[4:0]
#define PARAM_SET						0xA0	// Param write at add[4:0]
#define SOFTWARE_RESET			0x01	// Software reset command
#define NOP									0x00	// Clear Responce Register
#define Reset								0x01	// Software Reset
#define ALS_FORCE						0x06	// Force a single ALS measurement
#define ALS_PAUSE						0x0A	// pause autonomous ALS
#define ALS_AUTO						0x0E	// starts/restarts an autonomous ALS Loop

// I2C regs
#define INT_CFG							0x03	// Interrupt enable
#define IRQ_ENABLE					0x04	// Interrupt enable
#define HW_KEY							0x07
#define MEAS_RATE0					0x08	// meas_rate
#define MEAS_RATE1					0x09	// meas_rate
#define UCOEF0							0x13
#define UCOEF1							0x14
#define UCOEF2							0x15
#define UCOEF3							0x16
#define PARAM_WR						0x17	// write reg for ram param
#define PARAM_RD						0x2E	// read reg for ram param
#define COMMAND							0x18	// command reg
#define RESPONSE						0x20
#define IRQ_STATUS					0x21	// Interrupt status
#define ALS_VIS_DATA0				0x22	// Data output
#define ALS_VIS_DATA1				0x23	// Data output
#define ALS_IR_DATA0				0x24	// Data output
#define ALS_IR_DATA1				0x25	// Data output
#define AUX_DATA0						0x2C	// Data output
#define AUX_DATA1						0x2D	// Data output

// functions
void uvReset();
void uvPre();
void setForceMode();
void setAutonomousMode(int meas_rate);
void setALSVISMode();
void setUVMode();
void getForceData();
void getALSVISData(int* als_visData);
void getUVData(int* uvData);
