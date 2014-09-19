/****************************************/
/* Configure Funcion for UV_Meter(head)	*/
/* Light Sensor													*/
/* Author: Wu Tongda										*/
/* Tsinghua EE NICS											*/
/* Final Time: 2014/09/04								*/
/****************************************/
#include "UVMeter.h"
#include "LCD.h"

/*****************************************************/
/* Data Write to I2C Regs 													 */
/*****************************************************/
void writeToRegister(uchar regAddr, uchar command[2], int n){
	I2C_w(SLAVE_ADDR, regAddr, command, n);
	DelayUS(10);
}
/*****************************************************/
/* Data read to I2C Regs 							  						 */
/*****************************************************/
void readFromRegister(uchar regAddr, uchar command[2], int n){
	I2C_r(SLAVE_ADDR, regAddr, command, n);
	DelayUS(10);
}
/*****************************************************/
/* Data Write to Ram Param  												 */
/*****************************************************/
void writeToParameter(uchar paramAddr, uchar command){
	uchar destAddr[2];
	destAddr[0] = 0xA0|paramAddr;
	// write command to PARAM_WR
	I2C_w(SLAVE_ADDR, PARAM_WR, &command, 1);
	DelayUS(10);
	// set command to COMMAND and the device write PARAM_WR to destiny
	I2C_w(SLAVE_ADDR, COMMAND, destAddr, 1);
	DelayUS(10);
}
/*****************************************************/
/* Data Read from Ram Param  												 */
/*****************************************************/
void readFromParameter(uchar paramAddr, uchar command){
	// set command to COMMAND and the device write destiny to PARAM_RD
	uchar destAddr[2];
	destAddr[0] = 0x80|paramAddr;
	I2C_w(SLAVE_ADDR, COMMAND, destAddr, 1);
	DelayUS(10);
	// read inform from PARAM_RD
	I2C_r(SLAVE_ADDR, PARAM_RD, &command, 1);
	DelayUS(10);
}
/*****************************************************/
/*Prepare for I2C bus for UVMeter to be standby-mode */
/*																									 */
/*	Set HW_KEY, Enter StandBy Mode finally					 */
/*																									 */
/*****************************************************/
void uvPre(){
	uchar command[2];
	
	I2C_rst();
	I2C_initial();
	DelayMS(100);	// wait from init to standby
	
	// prepare UV Meter/Send hardware key
	command[0] = 0x17;
	writeToRegister(HW_KEY, command, 1);
	readFromRegister(HW_KEY, command, 1);
}
/*****************************************************/
/* Software reset																		 */
/*																									 */
/*	Configure PART_ID, REV_ID, SEQ_ID to default		 */
/*																									 */
/*****************************************************/
void uvReset(){
	uchar command[2];
	
	// send software reset command
	command[0] = SOFTWARE_RESET;
	writeToRegister(COMMAND, command, 1);
	readFromRegister(COMMAND, command, 1);
}
/*****************************************************/
/* Set Force Measure Mode														 */
/*																									 */
/*	Set Meas_Rate to Zero to active the mode 				 */
/*																									 */
/*****************************************************/
void setForceMode(){
	uchar command[2];
	
	// MEAS_RATE set to force mode
	command[0] = 0x00;
	command[1] = 0x00;
	writeToRegister(MEAS_RATE0, command, 2);
	readFromRegister(MEAS_RATE0, command, 2);
	
	/********* I2C regs: Set interrupt enable *********/
	// interrupt output pin enable 
	command[0] = 0x01;
	writeToRegister(INT_CFG, command, 1);
	readFromRegister(INT_CFG, command, 1);
	// interrupt enable 
	command[0] = 0x01;
	writeToRegister(IRQ_ENABLE, command, 1);
	readFromRegister(IRQ_ENABLE, command, 1);
	
	// Reset the Interrupt
	//readFromRegister(IRQ_STATUS, command, 1);
	//writeToRegister(IRQ_STATUS, command, 1);
	
	// Send ALS_FORCE command for each measurement
}
/*****************************************************/
/* Set Autonomous Measure Mode											 */
/*																									 */
/*	1.Set Meas_Rate to non-Zero to active the mode   */
/*	2.Send ALS_AUTO command at standby mode to start */
/*																									 */
/*****************************************************/
/*void setAutonomousMode(int meas_rate){
	uchar command[2];
	
	// MEAS_RATE set to auto mode
	command[0] = meas_rate%16;
	command[1] = meas_rate/16;
	writeToRegister(MEAS_RATE0, &command, 2);
	
	// Need to be at standby-mode
	// Send ALS_AUTO Command
	command[0] = ALS_AUTO;
	writeToRegister(COMMAND, &command, 1);
}*/
/*****************************************************/
/* Set ALS_VIS Mode																	 */
/*																									 */
/*	1.I2C regs:																			 */
/*		1) Interrupt enable, interrupt output enable	 */
/*	2.RAM PARAM:
/*		1) CHLIST: choose VIS													 */
/*		2) ALS_ENCODING: choose VIS										 */
/*		2) ALS_VIS_ADC_GAIN: set resolution 1lx				 */
/*		2) ALS_VIS_ADC_MISC: Set range normal range 	 */
/*																									 */
/*****************************************************/
void setALSVISMode(){
	uchar command[2];
	
	/********* RAM PARAM: configuration CHLIST for param *********/
	// write command to CHILIST: set set als_via
	writeToParameter(CHLIST, 0x10);
	readFromParameter(CHLIST, command);
	
	/********* RAM PARAM: Set ALS output encoding *********/
	// write command to ALS_ENCODING: output set to VIS
	writeToParameter(ALS_ENCODING, 0x10);
	readFromParameter(ALS_ENCODING, command);
	
	/********* RAM PARAM: Set resolution/ADC recover time *********/
	// write command to ALS_VIS_ADC_COUNTER: 3.15us
	writeToParameter(ALS_VIS_ADC_COUNTER, 0x40);
	readFromParameter(ALS_VIS_ADC_COUNTER, command);
	// write command to ALS_VIS_ADC_GAIN: 1lx
	writeToParameter(ALS_VIS_ADC_GAIN, 0x00);
	readFromParameter(ALS_VIS_ADC_GAIN, command);
	
	
	/********* RAM PARAM: Set range *********/
	// write command to ALS_VIS_ADC_MISC: normal range
	writeToParameter(ALS_VIS_ADC_MISC, 0x00);
	readFromParameter(ALS_VIS_ADC_MISC, command);
}

// Set UV Mode
/*void setUVMode(){
	uchar command[2];
	
	/********* I2C regs: Set UV coefficience *********//*
	// write UCOEF[3:0]
	command[0] = 0x00;
	command[1] = 0x02;
	command[2] = 0x89;
	command[3] = 0x29;
	writeToRegister(UCOEF0, &command, 4);
	
	/********* I2C regs: Set interrupt enable *********//*
	// interrupt output pin enable 
	command[0] = 0x01;
	writeToRegister(INT_CFG, &command, 1);
	// interrupt enable 
	command[0] = 0x01;
	writeToRegister(IRQ_ENABLE, &command, 1);
	
	/********* RAM PARAM: Set range *********//*
	// write command to CHLIST: EN_UV
	writeToParameter(CHLIST, 0x80);
}*/

void startForceMeas(){
	uchar command[2];
	
	// Send ALS_FORCE Command (Need to be at standby-mode)
	command[0] = ALS_FORCE;
	writeToRegister(COMMAND, command,1);
	readFromRegister(COMMAND, command, 1);
}

void getALSVISData(int* als_visData){
	uchar tempData[2];
	uchar tempInt[1];
	// wait 285us?? didn't mentioned
	wait(1,1000);
	
	// Read the interrupt
	//while(INT);
	// Read vis data from regs
	readFromRegister(ALS_VIS_DATA0, tempData, 2);
	/*LCD_show(tempData[0]);
	wait(3,1000);
	LCD_show(0xAAAA);
	wait(3,1000);
	LCD_show(tempData[1]);
	wait(3,1000);*/
	*als_visData = (tempData[1]<<8)|tempData[0];
		
	// Clear INT
	// read the ALS_INT first
	I2C_r(SLAVE_ADDR, IRQ_STATUS, tempInt, 1);
	// re-write the ALS_INT
	I2C_w(SLAVE_ADDR, IRQ_STATUS, tempInt, 1);
}

/*void getUVData(int* uvData){
	uchar* tempData;
	// wait 285us
	DelayUS(285);
	
	// Read uv data from regs
	readFromRegister(AUX_DATA0, &tempData, 2);
	
	*uvData = tempData[1];
	*uvData = (*uvData<<8)|tempData[0]/100;
}*/

void circleData(int* tmp){
	uchar meas[2];
	
	readFromRegister(MEAS_RATE0, meas, 2);
	*tmp = (meas[0]<<8)|meas[1];
}