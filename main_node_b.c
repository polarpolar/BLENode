#include <REG51.H>
#include <stdio.h>
#include "Delay.h"
#include "LCD.h"
#include "BDE0700.h"
#include "KXTJ9.h"
#include "ADS1000.h"
#include "BlueUART.h"
#include "tmp100.h"
#include "UVMeter.h"

#define ANum 1
int a_time=0;
int a_count=0;
int cnt=0;
uint  temperature;
unsigned char payload[1+2+6*ANum+2];

void main()
{   
	
	//int	 acc_x,acc_y,acc_z;	
	int tmp;
	unsigned char i;	
	unsigned char ini_cnt;
	//Initialize
	Ht1621_Init();
	Ht1621WrAllData();
	LCD_show(0xFFFF);wait(1,1000);
	
	// tmp
	tmpInit();		// Init, Res12/RanH/Mod0
	tmpSetRes(3);		// Set Resolution (0-9/1-10/2-11/3-12)
	tmpSetRange(1);		// Set Range (0:L/1:H)
	tmpSetMode(0);		// Set Mode (0:Continuous/1:ShutDown)
	
	// UV_Meter
	uvPre();
	uvReset();
	setForceMode();
	setALSVISMode();

	LCD_show(0x7777);wait(3,1000);
	
	payload[0]=1;
	ini_cnt=0;
	i=1;	
	while(1)
	{
		//initialization
		if(ini_cnt==50)
		{		
		Ht1621_Init();
		ini_cnt=1;
		}

		//tmpGetTmpCont(&tmp);
		getForceData();
		getALSVISData(&tmp);
		
		cnt++;
		if(cnt==1)
		{
			LCD_show(tmp);
			BlueTooth_Send(payload, 1);
			wait(3,2000);
		}
		if(cnt==2)
		{
			LCD_show(payload[0]++);
			BlueTooth_Send(payload, 1);
			wait(3,1000);
			cnt=0;
		}
				 		
	}	
}