#include <reg51.h>

//function prototypes
void lcd_cmd(unsigned char);
void lcd_init();
void lcd_data(unsigned char);
void msdelay(unsigned char);
char keypad_scan();
void boot();
void result_mode();
void voting_mode();

//LCD FUNCTIONS and CONSTANTS- Send Data and Commands along with Initialization

#define display_port P2 //Data pins connected to port 2 on microcontroller
sbit rs = P3^2;  //RS pin connected to pin 2 of port 3
sbit rw = P3^3;  // RW pin connected to pin 3 of port 3
sbit e =  P3^4;  //E pin connected to pin 4 of port 3

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
void send_msg(unsigned char *a){
  int l=0;
  lcd_init();
  while(*a != '\0') // searching the null terminator in the sentence
	{
	 lcd_data(*a);
   a++;
   msdelay(5);
	}
}
//KEYPAD FUNCTIONS and CONSTANTS
sbit row1 = P1^0;
sbit row2 = P1^1;
sbit row3 = P1^2;
sbit row4 = P1^3;

sbit col1 = P1^4;
sbit col2 = P1^5;
sbit col3 = P1^6;
sbit col4 = P1^7;

unsigned char keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char keypad_scan() {
	row1 = 0; row2 = 1; row3 = 1; row4 = 1;  // Activate row 1
  if (col1 == 0) { delay(); while(col1==0); return keypad[0][0]; }
  if (col2 == 0) { delay(); while(col2==0); return keypad[0][1]; }
  if (col3 == 0) { delay(); while(col3==0); return keypad[0][2]; }
  if (col4 == 0) { delay(); while(col4==0); return keypad[0][3]; }

  row1 = 1; row2 = 0; row3 = 1; row4 = 1;  // Activate row 2
  if (col1 == 0) { delay(); while(col1==0); return keypad[1][0]; }
  if (col2 == 0) { delay(); while(col2==0); return keypad[1][1]; }
  if (col3 == 0) { delay(); while(col3==0); return keypad[1][2]; }
  if (col4 == 0) { delay(); while(col4==0); return keypad[1][3]; }

  row1 = 1; row2 = 1; row3 = 0; row4 = 1;  // Activate row 3
  if (col1 == 0) { delay(); while(col1==0); return keypad[2][0]; }
  if (col2 == 0) { delay(); while(col2==0); return keypad[2][1]; }
  if (col3 == 0) { delay(); while(col3==0); return keypad[2][2]; }
  if (col4 == 0) { delay(); while(col4==0); return keypad[2][3]; }

  row1 = 1; row2 = 1; row3 = 1; row4 = 0;  // Activate row 4
  if (col1 == 0) { delay(); while(col1==0); return keypad[3][0]; }
  if (col2 == 0) { delay(); while(col2==0); return keypad[3][1]; }
  if (col3 == 0) { delay(); while(col3==0); return keypad[3][2]; }
  if (col4 == 0) { delay(); while(col4==0); return keypad[3][3]; }

  return 0;  // No key pressed
}
//LOGIC MODES - FUNCTIONS and CONSTANTS
unsigned int candidate_votes[4]; //stores individual candidate votes in the array (upto 4 candidates)

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
void modechoose(){	//called on power on, and after each time user finishes voting
	while(1){
		unsigned char temp = keypad_scan();
		if(temp != 0){
			if(temp != keypad[3][2]){
				voting_mode();
				break;
			}
			else{
				result_mode();
				break;
			}
		}
	}
}

void voting_mode(){
	//take valid user vote and increment party counter.
}

void main(){
	
}
