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
/*Prepare for I2C bus for UVMeter to be standby-mode */
/*																									 */
/*	Set HW_KEY, Enter StandBy Mode finally					 */
/*																									 */
/*****************************************************/
void uvPre(){
	uchar* command;
	
	I2C_rst();
	I2C_initial();
	
	//	wait 25ms at least for init-mode to standby-mode
	DelayMS(25);
	
	// prepare UV Meter
	command[0] = 0x17;
	I2C_w(SLAVE_ADDR, HW_KEY, command, 1);
	DelayUS(10);
}
/*****************************************************/
/* Software reset																		 */
/*																									 */
/*	Configure PART_ID, REV_ID, SEQ_ID to default		 */
/*																									 */
/*****************************************************/
void uvReset(){
	uchar* command;
	
	// send software reset command
	command[0] = SOFTWARE_RESET;
	I2C_write(SLAVE_ADDR, command, 1);
	DelayUS(10);
}
/*****************************************************/
/* Set Force Measure Mode														 */
/*																									 */
/*	Set Meas_Rate to Zero to active the mode 				 */
/*																									 */
/*****************************************************/
void setForceMode(){
	uchar* command;
	
	// MEAS_RATE set to force mode
	command[0] = 0x00;
	command[1] = 0x00;
	I2C_w(SLAVE_ADDR, (MEAS_RATE0|0x40), command, 2);
	DelayUS(10);
	
	// Reset the Interrupt
	I2C_r(SLAVE_ADDR, IRQ_STATUS, command, 1);
	command[0] = command[0]|0x20;	// CMD_INT = 1
	I2C_w(SLAVE_ADDR, IRQ_STATUS, command, 1);
	DelayUS(10);
	
	// Send ALS_FORCE command for each measurement
}
/*****************************************************/
/* Set Autonomous Measure Mode											 */
/*																									 */
/*	1.Set Meas_Rate to non-Zero to active the mode   */
/*	2.Send ALS_AUTO command at standby mode to start */
/*																									 */
/*****************************************************/
void setAutonomousMode(int meas_rate){
	uchar* command;
	
	// MEAS_RATE set to auto mode
	command[0] = meas_rate%16;
	command[1] = meas_rate/16;
	I2C_w(SLAVE_ADDR, (MEAS_RATE0|0x40), command, 2);
	DelayUS(10);
	
	// Need to be at standby-mode
	// Send ALS_AUTO Command
	command[0] = ALS_AUTO;
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
}
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
	uchar* command;
	
	/********* I2C regs: Set interrupt enable *********/
	// interrupt output pin enable 
	command[0] = 0x01;
	I2C_w(SLAVE_ADDR, INT_CFG, command, 1);
	DelayUS(10);
	
	// interrupt enable 
	command[0] = 0x01;
	I2C_w(SLAVE_ADDR, IRQ_ENABLE, command, 1);
	DelayUS(10);
	
	/********* RAM PARAM: configuration CHLIST for param *********/
	// write command to PARAM_WR: set set als_via
	command[0] = 0x10;		// CHLIST command
	I2C_w(SLAVE_ADDR, PARAM_WR, command, 1);
	DelayUS(10);
	// set command to COMMAND and the device write PARAM_WR to CHILIST_ADDR
	command[0] = (0x80|CHLIST);	// set CHLIST addr
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
	
	/********* RAM PARAM: Set ALS output encoding *********/
	// write command to PARAM_WR: output set to VIS
	command[0] = 0x10;	// set ALS_ENCODING to VIS
	I2C_w(SLAVE_ADDR, PARAM_WR, command, 1);
	DelayUS(10);
	// set command to COMMAND and the device write PARAM_WR to ALS_ENCODING
	command[0] = (0x80|ALS_ENCODING);	// set ALS_ENCODING addr
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
	
	/********* RAM PARAM: Set resolution/ADC recover time *********/
	// write command to PARAM_WR: 3.15us
	command[0] = 0x40;	// set ALS_VIS_ADC_COUNTER to 3.15
	I2C_w(SLAVE_ADDR, PARAM_WR, command, 1);
	DelayUS(10);
	// set command to COMMAND and the device write PARAM_WR to ALS_VIS_ADC_COUNTER
	command[0] = (0x80|ALS_VIS_ADC_COUNTER);	// set ALS_VIS_ADC_COUNTER addr
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
	// write command to PARAM_WR: 1lx
	command[0] = 0x00;	// set ALS_VIS_ADC_GAIN to 0x0,1lx
	I2C_w(SLAVE_ADDR, PARAM_WR, command, 1);
	DelayUS(10);
	// set command to COMMAND and the device write PARAM_WR to ALS_VIS_ADC_GAIN
	command[0] = (0x80|ALS_VIS_ADC_GAIN);	// set ALS_VIS_ADC_GAIN addr
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
	
	/********* RAM PARAM: Set range *********/
	// write command to PARAM_WR: normal range
	command[0] = 0x00;	// set ALS_VIS_ADC_MISC to normal range
	I2C_w(SLAVE_ADDR, PARAM_WR, command, 1);
	DelayUS(10);
	// set command to COMMAND and the device write PARAM_WR to ALS_VIS_ADC_MISC
	command[0] = (0x80|ALS_VIS_ADC_MISC);	// set ALS_VIS_ADC_MISC addr
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
}

// Set UV Mode
void setUVMode(){
	uchar* command;
	
	// configuration i2c regs
	// write UCOEF[3:0]
	command[0] = 0x00;
	command[1] = 0x02;
	command[2] = 0x89;
	command[3] = 0x29;
	I2C_w(SLAVE_ADDR, (UCOEF0|0x40), command, 4);
	DelayUS(10);
	
	// configuration param rams
	// write command to PARAM_WR
	command[0] = 0x80;		// CHLIST command, set EN_UV;
	I2C_w(SLAVE_ADDR, PARAM_WR, command, 1);
	DelayUS(10);
	
	command[0] = 0x81;	// set CHLIST addr
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
	
	/********* I2C regs: Set interrupt enable *********/
	// interrupt output pin enable 
	command[0] = 0x01;
	I2C_w(SLAVE_ADDR, INT_CFG, command, 1);
	DelayUS(10);
	
	// interrupt enable 
	command[0] = 0x01;
	I2C_w(SLAVE_ADDR, IRQ_ENABLE, command, 1);
	DelayUS(10);
}

void getForceData(){
	uchar* command;
	
	// Send ALS_FORCE Command (Need to be at standby-mode)
	command[0] = ALS_FORCE;
	I2C_w(SLAVE_ADDR, COMMAND, command, 1);
	DelayUS(10);
}

void getALSVISData(int* als_visData){
	uchar* tempData;
	//uchar* tempInt;
	// wait 285us?? didn't mentioned
	//DelayUS(285);
	
	// Read the interrupt
	//while(INT);
	// Read vis data from regs
	I2C_r(SLAVE_ADDR, (ALS_VIS_DATA0|0x00), tempData, 2);
	*als_visData = tempData[1];
	*als_visData = (*als_visData<<8)|tempData[0];
		
	// Clear INT
	// read the ALS_INT first
	//I2C_r(SLAVE_ADDR, (IRQ_STATUS|0x00), tempInt, 1);
	// re-write the ALS_INT
	//I2C_w(SLAVE_ADDR, (IRQ_STATUS|0x00), tempInt, 1);
}

void getUVData(int* uvData){
	uchar* tempData;
	// wait 285us
	DelayUS(285);
	
	// Read uv data from regs
	I2C_r(SLAVE_ADDR, (AUX_DATA0|0x40), tempData, 2);
	
	*uvData = tempData[1];
	*uvData = (*uvData<<8)|tempData[0]/100;
}

