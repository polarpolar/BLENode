#include <REG51.H>
#include <stdio.h>
#include "Delay.h"
#include "LCD.h"
#include "BDE0700.h"
#include "KXTJ9.h"
#include "ADS1000.h"
#include "tmp100.h"
#include "UVMeter.h"

int cnt=0;
unsigned char payload;

void main()
{   
	int tmp;
	unsigned char i;	
	
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
	
	i=1;	
	while(1)
	{
		//tmpGetTmpCont(&tmp);
		getForceData();
		getALSVISData(&tmp);
		
		cnt++;
		if(cnt==1)
		{
			LCD_show(tmp);
			wait(3,2000);
		}
		if(cnt==2)
		{
			LCD_show(payload++);
			wait(3,1000);
			cnt=0;
		}	 		
	}	
}