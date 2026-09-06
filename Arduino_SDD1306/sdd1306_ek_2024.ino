#include <Wire.h>

//setup commands 

#define Mux_ratio 0xA8 //send 0x3F after
#define set_display 0xD3 //command to set display - after this send 0 if no offset
#define startline 0x40 //starts from page 0
#define seg_remap 0xA1 //or 0xA0
#define com_scan_dir 0xC0 //0xC8 if right to left
#define pin_config 0xDA // send 0x02 after this (works for my display)
#define contrast_ctrl 0x81 // send 0x7F after ( ideal contrast setting )
#define display_ram 0xA4
#define display_ALL 0xA5 
#define normal_pixels 0xA6
#define inverse_pixels 0xA7
#define oscillator_freq 0xD5 //send 0x80 after
#define enable_pumpcharge 0x8D //send 0x14 after
#define display_ON 0xAF
#define display_OFF 0xAE

//writing commands

//memory addressing modes
#define memory_add 0x20
#define page_addressing 0x02

// page_start addresses - 0xB0 to 0xB7 

#define hort_addressing 0x00 //then set page start and end address
#define vert_addressing 0x01

//only for horizontal and vertical addressing modes
#define column_add 0x21 //give start and end column address after (0 -127) to (0-127)
#define page_add 0x22


const byte address = 0x3C;

void send_cmd(uint8_t data) {
  Wire.beginTransmission(address);
  Wire.write((uint8_t)0x00);
  Wire.write(data);
  Wire.endTransmission();
}

void send_data(uint8_t data) {
  Wire.beginTransmission(address);
  Wire.write((uint8_t)0x40);
  Wire.write(data);
  Wire.endTransmission();
}

void initialise() {
  send_cmd(display_OFF);
  send_cmd(Mux_ratio); send_cmd(0x3F);
  send_cmd(set_display); send_cmd(0x00);
  send_cmd(startline);
  send_cmd(seg_remap);
  send_cmd(com_scan_dir); send_cmd(0xC8);
  send_cmd(pin_config); send_cmd(0x02);
  send_cmd(contrast_ctrl); send_cmd(0x7F);
  send_cmd(display_ram);
  send_cmd(normal_pixels);
  send_cmd(oscillator_freq); send_cmd(0x10);
  send_cmd(enable_pumpcharge); send_cmd(0x14);
  send_cmd(display_ON);
}

void clear_display() {
  send_cmd(display_OFF);
  send_cmd(memory_add); send_cmd(hort_addressing);
  send_cmd(column_add); send_cmd(0); send_cmd(127);
  send_cmd(page_add); send_cmd(0); send_cmd(7);

  for (int i = 0; i<1024; i++){
    send_data(B00000000);
  }
  send_cmd(display_ON);
}

void write_display_page(){
  send_cmd(memory_add);
  send_cmd(column_add); send_cmd(0); send_cmd(127);

  for (int i = 0; i<8; i++){
    send_cmd(page_addressing);
    for (int j = 0; j<128; j++){
      send_cmd(0xB0 + i);
      send_data(B11111111);
      delay(10);
    }
  }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  initialise();
  clear_display();

  delay(5000);
  write_display_page();
}

void loop() {
}
