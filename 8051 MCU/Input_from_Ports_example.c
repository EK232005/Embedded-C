#include <reg51.h>

void main(){
	unsigned char a,b;
	P1 = 0xFF; //setting all the pins in port as HIGH during setup configures them as input
	P2 = 0xFF;
  
	while(1){
		a = P1;
		b = P2;
		P3 = a+b;
	}
}
