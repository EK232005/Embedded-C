/*GPIOTE (Interrupt Handling)
*nrf_drv_gpiote_is_init()
 Checks if GPIOTE driver is already initialized.
*nrf_drv_gpiote_init()
  Initializes the GPIO interrupt system.
*nrf_drv_gpiote_in_init(pin, &config, handler)
  Configures a pin as interrupt input and attaches a handler function.
*nrf_drv_gpiote_in_event_enable(pin, true)
  Enables interrupt events for the pin.

nrf_gpio_cfg_output(pin)
  Configures pin as output.
nrf_gpio_pin_set(pin)
  Sets pin HIGH (LED OFF on nRF52 DK).
nrf_gpio_pin_toggle(pin)
  toggles pin state (LED ON/OFF).

Input Configuration
GPIOTE_CONFIG_IN_SENSE_HITOLO(true)
  Configures interrupt on HIGH → LOW transition (button press).
in_config.pull = NRF_GPIO_PIN_PULLUP
  Enables internal pull-up resistor for stable input.

ret_code_t
  Stores return status of functions.
APP_ERROR_CHECK(err_code)
  Stops execution if an error occurs.

  __WFE()
  Wait For Event → puts CPU to sleep until interrupt occurs.
 
*/
#include <stdbool.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"

#define LED 17
#define Btn 13

void input_pin_handle(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    nrf_gpio_pin_toggle(LED);
}

void gpio_init()
{
    ret_code_t err_code;

    if (!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }

    nrf_gpio_cfg_output(LED);
    nrf_gpio_pin_set(LED);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(Btn, &in_config, input_pin_handle);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(Btn, true);
}

int main(void)
{
    gpio_init();

    while (true)
    {
        __WFE(); // sleep until event (better than empty loop)
    }
}
