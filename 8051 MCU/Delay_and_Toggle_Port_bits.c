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

/*delay calculation explained
- MCU Frequency - 11.0592 Mhz
- thus, T = 0.09 microsecond
- 1 T state takes 0.09 microsecond to complete, then 12 T states ( or 1 Machine Cycle) takes 1.08 micrseconds 
- thus 1 millisecond = 1000/1.08  = 926 loops 
*/
