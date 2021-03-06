

#include "KXTJ9.h"

#include "I2C.h"
#include "LCD.h"
#include "Delay.h"

//uint KXTJ9_SLAVE_ADDR = 0x0F;


void KXTJ9_prep(){
	I2C_rst();
	I2C_initial();
	KXTJ9_ADDR = 1;

}

 
void KXTJ9_Init(){
	
	uchar cmd[5];
	
	KXTJ9_prep();
	
	cmd[0] = 0x80;
	I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG2, cmd, 1);

	do{	
		I2C_r(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG2, cmd, 1);
	}while(cmd[0]&0x80)	 ;
	
	

	//Initialize CTRL_REG1
	cmd[0] = 0x00;
	I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, cmd, 1);
	
	//Initialize CTRL_REG2
	cmd[0] = 0x00;
	I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG2, cmd, 1);

	//Initialize INT_CTRL1 and INT_CTRL2
	cmd[0]=0x10; cmd[1] = 0x3f;
	I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_INT_CTRL_REG1, cmd, 2);
	
	//Initialize DATA_ODR_RATE
	cmd[0]=0x03;
	I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_DATA_CTRL_REG1, cmd, 1);
	
}

void KXTJ9_Set_Resolution(uint high){
	
	uchar cmd;
	
	KXTJ9_prep();
	 
	//Load CTRL_REG1 to cmd
	I2C_r(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);

	if(cmd & 0x80){			//check whether PC1==1
		cmd &= (~0x80);		//Set PC1 = 0
		I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);
	}
	
	if(high)
		cmd |= 0x40;
	else
		cmd &= (~0x40);

	I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);

}


void KXTJ9_Set_Range(uint range){

	uchar cmd;
	
	//Load CTRL_REG1 to cmd
	I2C_r(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);

	if(cmd & 0x80){			//if PC1==1, Set PC1 = 0
		cmd &= (~0x80);		
		I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);
	}

	cmd &= 0xE7; 	//0x11100111
	if(range==4)
		cmd |= 0x08;
	else if(range == 8)
		cmd |= 0x10;


	I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);

}  	


void KXTJ9_Get_Acceleration(int* a_x,int* a_y, int* a_z){
	
	uchar cmd;

	uchar data_out[2];

	//Load CTRL_REG1 to cmd
	I2C_r(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);


	if((cmd & 0x80) == 0){			//if PC1==0; Set PC1 = 1
		cmd |= 0x80;		
		I2C_w(KXTJ9_SLAVE_ADDR, KXTJ9_CTRL_REG1, &cmd, 1);
	}

	//read acceleration of x-axis
	I2C_r(KXTJ9_SLAVE_ADDR, 0x06, data_out, 2);
	*a_x = (data_out[1] << 4) | (data_out[0] >> 4)	;
	
	//read acceleration of y-axis
	I2C_r(KXTJ9_SLAVE_ADDR, 0x08, data_out, 2);
	*a_y = (data_out[1] << 4) | (data_out[0] >> 4)	;

	//read acceleration of z-axis
	I2C_r(KXTJ9_SLAVE_ADDR, 0x0A, data_out, 2);
	*a_z = (data_out[1] << 4) | (data_out[0] >> 4)	;


}	
