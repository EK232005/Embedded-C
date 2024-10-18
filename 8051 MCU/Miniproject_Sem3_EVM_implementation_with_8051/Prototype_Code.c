#include <reg51.h>
#define display_port P2 //Data pins connected to port 2 on microcontroller
sbit rs = P3^2;  //RS pin connected to pin 2 of port 3
sbit rw = P3^3;  // RW pin connected to pin 3 of port 3
sbit e =  P3^4;  //E pin connected to pin 4 of port 3

void msdelay(unsigned int ms){ //Function to create delay
	unsigned int i;
	TMOD = 0x01;
	for(i = 0; i<ms; i++){
		TH0 = 0xFC;
		TL0 = 0x66;
		TR0 = 1;
		while(TF0 == 0);
		TR0 = 0;
		TF0 = 0;
	}
}
void lcd_cmd(unsigned char command){  //Function to send command instruction to LCD
	display_port = command;
  rs= 0;                   //send command mode
  rw=0;
  e=1;
  msdelay(1);
  e=0;
}
void lcd_data(unsigned char disp_data){  //Function to send display data to LCD
	display_port = disp_data;
  rs= 1;                   //send data mode
  rw=0;
  e=1;
  msdelay(1);
  e=0;
}
 void lcd_init(){ //Initialize lcd function
	lcd_cmd(0x38);  // for using 2 lines and 5X7 matrix of LCD
  msdelay(10);
  lcd_cmd(0x0F);  // turn display ON, cursor blinking
  msdelay(10);
	lcd_cmd(0x01);  //clear screen
  msdelay(10);
  lcd_cmd(0x81);  // bring cursor to position 1 of line 1
  msdelay(10);
}
void main(){
	while(1){
	P1 = 0x00;
	msdelay(1000);
	P1 = 0xFF;
	msdelay(1000);
	}
}
