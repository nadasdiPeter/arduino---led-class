#ifndef LED_H
   #define LED_H

#include <Arduino.h>

#define DEFAULT_ON_CYCLE_TIME_MS                      100
#define DEFAULT_OFF_CYCLE_TIME_MS                     900
#define DEFAULT_LED_MODE                              continuous
#define DEFAULT_LED_STATE                             ON
#define DEFAULT_BRIGHTNESS                            255
#define DEFAULT_FADE_STEPS                            2
#define DEFAULT_FADE_DELAY                            5
#define DEFAULT_FADE_PAUSE                            500
#define DEFAULT_FADE_STATE                            fade_raising

enum ledMode         { continuous, blinking, fade };
enum ledState        { OFF, ON };
enum ledFadeState    { fade_raising, fade_falling, fade_paused };

class Led
{
   private:
      byte m_Pin;
      ledState m_State;
      ledMode m_Mode;
      byte m_Brightness;
      ledFadeState m_FadeState;
      byte m_FadeStep;
      unsigned long int m_FadeDelay;
      unsigned long int m_On_Interval_ms;
      unsigned long int m_Off_Interval_ms;
      unsigned long int m_Last_Time_State_Changed_ms;

   public:

      /**
       * Constructor of the LED class Should not use the default constructore.
       */
      Led();
      
      /**
       * Constructor of the LED class.
       * Will set the LED to continuous mode with defaolt ON state.
       *
       * @param pin The pin of the microcontroller where the LED connected
       */
      Led(byte pin);

      /**
       * Constructor of the LED class.
       *
       * @param pin The pin of the microcontroller where the LED connected
       * @param mode continuous or blinking
       */
      Led(byte pin, ledMode mode);

      /**
       * Constructor of the LED class.
       *
       * @param pin The pin of the microcontroller where the LED connected
       * @param state Strating state of the led: ON / OFF
       * @param mode continuous or blinking
       * @param brightness The brigthness of the led: 0: off 255: max brigthness
       * @param on_interval_ms Interval of the on cycle in case blinking mode.
       * @param off_interval_ms Interval of the off cycle in case blinking mode.
       */
      Led(byte pin, ledState state, ledMode mode, 
          byte brightness = DEFAULT_BRIGHTNESS,
          unsigned long int on_interval_ms = DEFAULT_ON_CYCLE_TIME_MS, 
          unsigned long int off_interval_ms = DEFAULT_OFF_CYCLE_TIME_MS);

      /**
       * Constructor of the LED class.
       *
       * @param pin The pin of the microcontroller where the LED connected
       * @param state Strating state of the led: ON / OFF
       * @param mode continuous or blinking
       * @param brightness The brigthness of the led: 0: off 255: max brigthness
       * @param fadeState
       * @param fadeStep
       * @param fadeDelay
       * @param on_interval_ms Interval of the on cycle in case blinking mode.
       * @param off_interval_ms Interval of the off cycle in case blinking mode.
       */
      Led( byte pin, ledState state, ledMode mode, byte brightness, ledFadeState fadeState, byte fadeStep, unsigned long int fadeDelay,
           unsigned long int on_interval_ms, unsigned long int off_interval_ms);

      /**
       * Defines the pin mode and sets the init state of the LED.
       */
      void init();

      /**
       * Sets the LED brigthness to the requested value. 0 .. 255.
       * 0   -> LED switched off
       * 255 -> Max brightness
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       */
      void set_Brightness(byte requested_brightness);

      /**
       * Turns the LED ON.
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       */
      void turn_On();

      /**
       * Turns the LED OFF.
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       */
      void turn_Off();

      /**
       * Turns the LED ON if it was OFF and turns it OFF if it was ON previously.
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       */
      void toggle();
      
      /**
       * Turns ON or OFF the LED according to the requested_state.
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       * 
       * @param requested_state ON or OFF
       */
      void switch_Led(ledState requested_state);

      /**
       * Sets the LED to continuous mode with the defined state.
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       * 
       * @param state ON or OFF
       */
      void set_ContinuousMode(ledState state);

      /**
       * Sets the LED to synchronous blinking mode (on and off cycle will be the same) with the defined parameters.
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       * 
       * @param starting_state ON or OFF
       * @param interval The interval of the blinking
       */
      void set_SynchronousBlinkingMode(ledState starting_state, unsigned long int interval);

      /**
       * Sets the LED to asynchronous blinking mode with the defined parameters.
       * Also updating the "m_Last_Time_State_Changed_ms" class member.
       * 
       * @param starting_state ON or OFF
       * @param on_interval The interval of the on cycle
       * @param off_interval The interval of the off cycle
       */
      void set_AsynchronousBlinkingMode(ledState starting_state, unsigned long int on_interval, unsigned long int off_interval);

      /**
       * Sets the LED to fade mode with the defined parameters.
       * 
       * @param fade_step How smooth the transition from Off state into the full brigthness. Smaller value means smooter the transition. Recommanded range: 1 .. 10
       * @param fade_delay How long a transition state maintained. (ms)
       * @param off_interval The interval of the off cycle (ms)
       */
      void set_FadeMode(byte fade_step, unsigned long int fade_delay, unsigned long int off_interval);

      /**
       * Sets the LED to fade mode with the defined parameters.
       * 
       * @param fade_step How smooth the transition from Off state into the full brigthness. Smaller value means smooter the transition. Recommanded range: 1 .. 10
       * @param fade_delay How long a transition state maintained. (ms)
       * @param off_interval The interval of the off cycle (ms)
       */
      void set_FadeMode(unsigned long int off_interval = DEFAULT_FADE_PAUSE);

      /**
       * Sets the LED to slow fadeing mode with the defined parameters.
       * 
       * @param off_interval The interval of the off cycle (ms)
       */
      void set_FadeMode_slow(unsigned long int off_interval = DEFAULT_FADE_PAUSE);

      /**
       * Sets the LED to fast fadeing mode with the defined parameters.
       * 
       * @param off_interval The interval of the off cycle (ms)
       */
      void set_FadeMode_fast(unsigned long int off_interval = DEFAULT_FADE_PAUSE);

      /**
       * Drives the LED.
       */
      void update();
};

#endif /* LED_H */