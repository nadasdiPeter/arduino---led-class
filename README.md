# Button class
LED class for arduino projects.
Creating and using LEDs was never so easy...
```
#include <Arduino.h>
#include <Led.h>

#define LED_PIN            GPIO_NUM_4
Led led(LED_PIN, ON, fade);

void setup()
{
   led.init();
}

void loop()
{
   led.update();
}
```


## continuous mode:

## blinking mode:

## fade mode:
