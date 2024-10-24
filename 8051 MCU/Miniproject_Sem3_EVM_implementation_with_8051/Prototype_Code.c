#include <reg51.h>

//function prototypes
void lcd_cmd(unsigned char);
void lcd_init();
void lcd_data(unsigned char);
void msdelay(unsigned int);
char keypad_scan();
void boot();
void result_mode();
void voting_mode();

//LCD FUNCTIONS and CONSTANTS- Send Data and Commands along with Initialization

#define display_port P2 //Data pins connected to port 2 on microcontroller
sbit rs = P3^3;
sbit rw = P3^4;
sbit e =  P3^5;

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

sbit col1 = P1^7;
sbit col2 = P1^6;
sbit col3 = P1^5;
sbit col4 = P1^4;

unsigned char code keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
char keypad_scan() {
	row1 = 0; row2 = 1; row3 = 1; row4 = 1;  // Activate row 1
  if (col1 == 0) { ; while(col1==0); return keypad[0][0]; }
  if (col2 == 0) { msdelay(50); while(col2==0); return keypad[0][1]; }
  if (col3 == 0) { msdelay(50); while(col3==0); return keypad[0][2]; }
  if (col4 == 0) { msdelay(50); while(col4==0); return keypad[0][3]; }

  row1 = 1; row2 = 0; row3 = 1; row4 = 1;  // Activate row 2
  if (col1 == 0) { msdelay(50); while(col1==0); return keypad[1][0]; }
  if (col2 == 0) { msdelay(50); while(col2==0); return keypad[1][1]; }
  if (col3 == 0) { msdelay(50); while(col3==0); return keypad[1][2]; }
  if (col4 == 0) { msdelay(50); while(col4==0); return keypad[1][3]; }

  row1 = 1; row2 = 1; row3 = 0; row4 = 1;  // Activate row 3
  if (col1 == 0) { msdelay(50); while(col1==0); return keypad[2][0]; }
  if (col2 == 0) { msdelay(50); while(col2==0); return keypad[2][1]; }
  if (col3 == 0) { msdelay(50); while(col3==0); return keypad[2][2]; }
  if (col4 == 0) { msdelay(50); while(col4==0); return keypad[2][3]; }

  row1 = 1; row2 = 1; row3 = 1; row4 = 0;  // Activate row 4
  if (col1 == 0) { msdelay(50); while(col1==0); return keypad[3][0]; }
  if (col2 == 0) { msdelay(50); while(col2==0); return keypad[3][1]; }
  if (col3 == 0) { msdelay(50); while(col3==0); return keypad[3][2]; }
  if (col4 == 0) { msdelay(50); while(col4==0); return keypad[3][3]; }

  return 0;  // No key pressed
}
//LOGIC MODES - FUNCTIONS and CONSTANTS

unsigned int candidate_votes[4]; //stores individual candidate votes in the array (upto 4 candidates)
unsigned char code password[4] = {'1','2','3','4'};

void disp_int_to_str(unsigned int num) {
    unsigned char str[6];  // Allocate space for the string (enough for up to 5 digits and null terminator)
    unsigned char *ptr = str;  // Use a pointer to traverse the string
    int i = 0;

    if (num == 0) {
        *ptr++ = '0';
    } else {
        while (num > 0) {
            *ptr++ = (num % 10) + '0';  // Store digits as characters
            num /= 10;
        }
    }
    *ptr = '\0';  // Null-terminate the string

    // Traverse the string in reverse order to display it correctly
    for (--ptr; ptr >= str; ptr--) {  
        lcd_data(*ptr);  // Display the digits on the LCD
    }
}

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
	unsigned char code a[15] = "WELCOME TO EVM";
	unsigned char code b[31] = "PRESS # TO SHOW VOTING RESULTS";
	unsigned char code c[12] = "VOTING MODE";
	unsigned char code d[12] = "RESULT MODE";
	send_msg(a);
	msdelay(2000);
	send_msg(b);
	while(1){
		unsigned char temp = keypad_scan();
		if(temp != 0){
			if(temp == keypad[3][2]){
				send_msg(d);
				msdelay(2000);
				result_mode();
				break;
			}
			else{
				send_msg(c);
				msdelay(2000);
				voting_mode();
				break;
			}
		}
	}
}
void voting_mode() {
	unsigned char code x[] = "PARTY1   PARTY2  PARTY3   PARTY4";
  unsigned char code y[] = "CONFIRM VOTE? (A - YES, B - NO)";
  unsigned char code invalid[] = "INVALID INPUT, TRY AGAIN";
  unsigned char code not_confirmed[] = "VOTE NOT CONFIRMED, SELECT AGAIN";
  unsigned char code z[] = "THANK YOU FOR VOTING";
  unsigned char input;
	unsigned char confirm;
  unsigned char party_index = 0xFF;  // Invalid party index initially
	
	send_msg(x);		// Show available parties
  while (1) {
		input = keypad_scan();               
		if(input != 0){
				if (input == keypad[0][0]) {  //checking for valid user input
					party_index = 0;               
			} else if (input == keypad[0][1]) { 
					party_index = 1;               
			} else if (input == keypad[0][2]) { 
					party_index = 2;               
			} else if (input == keypad[1][0]) { 
					party_index = 3;               
			} else {
					send_msg(invalid);            //uf invalid input
					msdelay(2000);
					send_msg(x);
					continue;                     // Go back to the loop to take input again
			}
			send_msg(y);
			msdelay(2000);
					//capture user input for confirmation
			while(1){
				confirm = keypad_scan();
				if(confirm != 0){
					if (confirm == keypad[0][3]) {      
							candidate_votes[party_index]++; //increment after confirming
							send_msg(z);                    
							msdelay(2000);
							break; //break the loop after confirming vote                         
					}else { 
							send_msg(not_confirmed);     //not confirmed go back to mode choose
							msdelay(2000);
							break;                       // Go back to the start of the voting process
					}
				}
				
			}
			break;
		}
  }
}
void result_mode(){
	unsigned char i= 0,keypress;
	unsigned char input[4] = {0};
	unsigned char code p[] = "INPUT 4 LETTER PASSWORD";
	unsigned char code q[] = "WRONG PASSWORD";
	send_msg(p);
	msdelay(2000);
	lcd_cmd(0x01); //clear screen
	lcd_cmd(0x81); //bring cursor back
	while(i<4){ //check for 4 keypresses
		keypress = keypad_scan();
		if(keypress!=0){
			input[i] = keypress;
			i++;
			lcd_data('*');
			msdelay(100); //delay for debouncing
		}
	}
	if(input[0] == password[0] && input[1] == password[1] && input[2] == password[2] && input[3] == password[3]){
		lcd_cmd(0x01); //clear screen
		lcd_cmd(0x81); //bring cursor back 
		for(i=0;i<4;i++){
			disp_int_to_str(candidate_votes[i]);
			lcd_data(',');
			msdelay(10);
		}
		msdelay(10000);
	}
	else{
		send_msg(q);
		msdelay(5000);
	}
}
void main(){
	lcd_init();
	while(1){
		modechoose();
	}
}
