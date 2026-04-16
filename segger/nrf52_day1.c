#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define led1 17
#define led2 18
#define button1 13
#define button2 14

/**
 * @brief Function for application main entry.
 */\


 //user def led blink func
 void ledblink(uint8_t ledpin, uint32_t delayms){
  nrf_gpio_pin_toggle(ledpin);
  nrf_delay_ms(delayms);
  nrf_gpio_pin_toggle(ledpin);
  nrf_delay_ms(delayms);
 }

bool b1p,b2p = false;

int main(void)
{
  //configure input and output pins
  nrf_gpio_cfg_output(led1);
  nrf_gpio_cfg_output(led2);
  nrf_gpio_cfg_input(button1,NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(button2,NRF_GPIO_PIN_PULLUP);

  while(1){
    if(nrf_gpio_pin_read(button1) == 0){
      b1p = true;
    }
    else{b1p = false;}
    //  
    if(nrf_gpio_pin_read(button2) == 0){
      b2p = true;
    }
    else{b2p = false;}
    //
    if(b1p == true){
      nrf_gpio_pin_clear(led1);
    }
    else{nrf_gpio_pin_set(led1);}
    //
    if(b2p == true){
      nrf_gpio_pin_clear(led2);
    }
    else{nrf_gpio_pin_set(led2);}
  }
}
/**
 *@}
 **/
