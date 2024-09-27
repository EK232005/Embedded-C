//Using Delay with empty for loops
//Toggle Port values with delay
#include <reg51.h>

void delay(unsigned int milli){
	unsigned int i, j;
	for( i = 0; i < milli; i++){
		for( j = 0; j < 1275; j++); //do nothing until for loops complete
	}
}

void main(){
	while(1){
	P1 = 0xAA;
	delay(10000);
	P1 = 0x55;
	delay(10000);
	}
}
