#include "BDE0700.h"
#include "LCD.h"
#include "Delay.h"

uint get_Temperature(){
	uint cnt = 0;
	LP_set = 0;
	DelayMS(20);
	LP_set = 1;	 		//Set released as input mode
	while(1){
		cnt += 1;
		if(LP_rd == 1)break;
	}
	return cnt;
}