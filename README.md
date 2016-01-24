# SSWS2812-Lib
Super Simple WS2812 Lib for STM32F1xx microcontroller

PWM + DMA based driver for WS2812 LEDs

* Top level API in wsLED.h
* Designed for STM32F1xx microcontroller with the appropiate Std.Periph Lib.
* Output port can be adjusted via wsConfig.h
* NO IRQs are used in this driver, the DMA uses the cicular mode and the synch gap is included in the led bit buffer
* The led count can be adjusted in wsConfig.h or detected automatically

```C
#include "wsLED.h

void main(void)
{
  uint8_t i = 0;
  WS_LED_T LED;
  HARDWARE_INIT();
  wsLED_Init();

  for(;;)
  {
    LED.Red = i;
    LED.Green = i;
    LED.Blue = i;
    wsLED_SetAllLed(&LED);
    wsLED_UpdateStripe();  /* Move data into DMA buffer */
    DELAY_SOME_MS();
    i++;
  }
  /* Here be dragons */
} 
```
