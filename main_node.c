#include <REG51.H>
#include <stdio.h>
#include "Delay.h"
#include "LCD.h"
#include "BlueUART.h"
#include "tmp100.h"

void main()
{
	int payload = 0;
	int tmp = 0;
	
	//Initialize
	Ht1621_Init();
	Ht1621WrAllData();
	LCD_show(0xFFFF);
	wait(1,1000);
	
	// tmp100
	tmpInit();
	tmpSetRes(3);
	tmpSetRange(1);
	tmpSetMode(0);
	
	LCD_show(0x7777);
	wait(3,1000);
	
	while(1)
	{
		//tmpGetTmpCont(&tmp);
		
		LCD_show(payload++);
		wait(3,1000);
		LCD_show(tmp++);
		wait(3,1000);
	}	
}