#include <reg51.h>

sfr P0 = 0x80; //port addresses
sfr P1 = 0x90;
sfr P2 = 0xA0;

void delay(unsigned int itime){
	unsigned int i,j;
	for(i=0;i<itime;i++){
		for(j=0;j<1275;j++);
	}
}

void main(){
	while(1){
		P0 = 0x55;
		P2 = 0x55;
		delay(250);
		P0 = 0xAA;
		P2 = 0xAA;
		delay(250);
	}
}

