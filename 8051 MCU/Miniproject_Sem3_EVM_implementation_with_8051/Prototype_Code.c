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
sbit rs = P3^2;
sbit rw = P3^3;
sbit e =  P3^4;

void lcd_init(){ //Initialize lcd function
	lcd_cmd(0x38);              // for using 2 lines and 5X7 matrix of LCD
  msdelay(10);
  lcd_cmd(0x0F);              // turn display ON, cursor blinking
  msdelay(10);
	lcd_cmd(0x01);              //clear screen
  msdelay(10);
  lcd_cmd(0x81);              // bring cursor to position 1 of line 1
  msdelay(10);
}
void lcd_cmd(unsigned char command){  //Function to send command instruction to LCD
	display_port = command;
  rs= 0;                      //send command mode
  rw=0;
  e=1;
  msdelay(1);
  e=0;
}
void lcd_data(unsigned char disp_data){  //Function to send display data to LCD
	display_port = disp_data;
  rs= 1;                      //send data mode
  rw=0;
  e=1;
  msdelay(1);
  e=0;
}
void send_msg(unsigned char *a){
  unsigned int l=0;
	unsigned char char_len=0;
  lcd_cmd(0x01);     		      //clear screen
	lcd_cmd(0x80);     		      //force cursor back to start of first line 
  while(*a != '\0'){ 		      // searching the null terminator in the sentence
		lcd_data(*a);
		a++;
		char_len++;
		if(char_len==16){         //if message is greater than 16, go to 2nd line
			lcd_cmd(0xC0);
		}
		msdelay(5);
		if(char_len==32){         //message is greater than lcd can display
			break;
		}
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
  if (col1 == 0) { ; while(col1==0); return keypad[0][0]; }
  if (col2 == 0) { msdelay(10); while(col2==0); return keypad[0][1]; }
  if (col3 == 0) { msdelay(10); while(col3==0); return keypad[0][2]; }
  if (col4 == 0) { msdelay(10); while(col4==0); return keypad[0][3]; }

  row1 = 1; row2 = 0; row3 = 1; row4 = 1;  // Activate row 2
  if (col1 == 0) { msdelay(10); while(col1==0); return keypad[1][0]; }
  if (col2 == 0) { msdelay(10); while(col2==0); return keypad[1][1]; }
  if (col3 == 0) { msdelay(10); while(col3==0); return keypad[1][2]; }
  if (col4 == 0) { msdelay(10); while(col4==0); return keypad[1][3]; }

  row1 = 1; row2 = 1; row3 = 0; row4 = 1;  // Activate row 3
  if (col1 == 0) { msdelay(10); while(col1==0); return keypad[2][0]; }
  if (col2 == 0) { msdelay(10); while(col2==0); return keypad[2][1]; }
  if (col3 == 0) { msdelay(10); while(col3==0); return keypad[2][2]; }
  if (col4 == 0) { msdelay(10); while(col4==0); return keypad[2][3]; }

  row1 = 1; row2 = 1; row3 = 1; row4 = 0;  // Activate row 4
  if (col1 == 0) { msdelay(10); while(col1==0); return keypad[3][0]; }
  if (col2 == 0) { msdelay(10); while(col2==0); return keypad[3][1]; }
  if (col3 == 0) { msdelay(10); while(col3==0); return keypad[3][2]; }
  if (col4 == 0) { msdelay(10); while(col4==0); return keypad[3][3]; }

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
	unsigned char a[15] = "WELCOME TO EVM";
	unsigned char b[31] = "PRESS # TO SHOW VOTING RESULTS";
	unsigned char c[12] = "VOTING MODE";
	unsigned char d[12] = "RESULT MODE";
	send_msg(a);
	msdelay(2000);
	send_msg(b);
	while(1){
		unsigned char temp = keypad_scan();
		if(temp != 0){
			if(temp == keypad[3][2]){
				send_msg(c);
				msdelay(2000);
				result_mode();
				break;
			}
			else{
				send_msg(d);
				msdelay(2000);
				voting_mode();
				break;
			}
		}
	}
}
void voting_mode(){
	//take valid user vote and increment party counter.
	unsigned char x[33] = "PARTY1   PARTY2  PARTY3   PARTY4";
	unsigned char y[] = "CONFIRM VOTE ?     (A - YES)";
	unsigned char invalid[] = "INVALID INPUT     PLS TRY AGAIN"	;
	unsigned char z[] = "THANK YOU FOR        VOTING";
	unsigned char input;
	while(1){
		send_msg(x);
		msdelay(5000);
		input = keypad_scan();		
		if(keypad_scan!=0){
			if(input == keypad[0][0] || input == keypad[0][1] || input == keypad[0][2] || input == keypad[0][3] || keypad[1][0]){
				send_msg(y);
				msdelay(5000);
				if(keypad_scan() != keypad[0][3]){
					continue;
				}
				else{
					send_msg(z);
					msdelay(10000);
					break;
				}
			}
		}
	}
}

void main(){
	send_msg("HELLO");
}
