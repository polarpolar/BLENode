#include "BlueUART.h"

void delay(uint i){
	while(i--);
}
void WByte(uchar input){
    uchar i = 8;
    uchar j = 0;
    TX = 0;                    
    delay(8);
    j++;
    j++;
    j++;
    j++;
    j++;
    j++;
    while(i--){
        TX = input & 0x01;  
        delay(8); 
        j++;
        j++;
        j++;
        j++;
        input = input >> 1;
    }
    TX = 1;
    delay(50);                    
}
extern void BlueTooth_Send(char* msg, uint len){
	uint i;
	for(i=0;i<len;i++){
		WByte(msg[i]);
	}
}
