#include "Led.h"

Led::Led()
{
   // Do not use.
}

Led::Led(byte pin)
{
   m_Pin = pin;
   m_State = ON;
   m_Mode = continuous;
   m_Last_Time_State_Changed_ms = millis();
   m_Brightness = 255;
}

Led::Led(byte pin, ledMode mode)
{
   m_Pin             = pin;
   m_State           = ON;
   m_Mode            = mode;
   m_On_Interval_ms  = DEFAULT_ON_CYCLE_TIME_MS;
   m_Off_Interval_ms = DEFAULT_OFF_CYCLE_TIME_MS;
   m_Brightness      = DEFAULT_BRIGHTNESS;
   m_FadeDelay       = DEFAULT_FADE_DELAY;
   m_FadeStep        = DEFAULT_FADE_STEPS;
   m_FadeState       = DEFAULT_FADE_STATE;
   m_Last_Time_State_Changed_ms = millis();
}

Led::Led(byte pin, ledState state, ledMode mode, byte brightness, unsigned long int on_interval_ms, unsigned long int off_interval_ms)
{
   m_Pin = pin;
   m_State = state;
   m_Mode = mode;
   m_On_Interval_ms = on_interval_ms;
   m_Off_Interval_ms = off_interval_ms;
   m_Last_Time_State_Changed_ms = millis();
   m_Brightness = brightness;
   m_FadeDelay = DEFAULT_FADE_DELAY;
   m_FadeStep = DEFAULT_FADE_STEPS;
   m_FadeState = fade_raising;
}

Led::Led( byte pin, ledState state, ledMode mode, byte brightness, ledFadeState fadeState, byte fadeStep, unsigned long int fadeDelay, unsigned long int on_interval_ms, unsigned long int off_interval_ms)
{
   m_Pin = pin;
   m_State = state;
   m_Mode = mode;
   m_On_Interval_ms = on_interval_ms;
   m_Off_Interval_ms = off_interval_ms;
   m_Brightness = brightness;
   m_FadeDelay = fadeDelay;
   m_FadeStep = fadeStep;
   m_FadeState = fadeState;
   m_Last_Time_State_Changed_ms = millis();
}

void Led::init()
{
   pinMode(m_Pin, OUTPUT);
   switch_Led(m_State);
}

void Led::set_Brightness(byte requested_brightness)
{
   m_State = (requested_brightness > 0) ? ON : OFF;
   m_Brightness = requested_brightness;
   analogWrite(m_Pin, m_Brightness);
   m_Last_Time_State_Changed_ms = millis();
}

void Led::turn_On()
{
   m_State = ON;
   m_Brightness = 255;
   digitalWrite(m_Pin, m_State);
   m_Last_Time_State_Changed_ms = millis();
}

void Led::turn_Off()
{
   m_State = OFF;
   m_Brightness = 0;
   digitalWrite(m_Pin, m_State);
   m_Last_Time_State_Changed_ms = millis();
}

void Led::toggle()
{
   m_State = (m_State == OFF) ? ON : OFF;
   switch_Led(m_State);
}

void Led::switch_Led(ledState requested_state)
{
   if(requested_state == ON) 
      turn_On();
   else
      turn_Off();
}

void Led::set_ContinuousMode(ledState state)
{
   m_Mode = continuous;
   init();
   if(state == ON)
      turn_On();
   else
      turn_Off();
}

void Led::set_SynchronousBlinkingMode(ledState starting_state, unsigned long int interval)
{
   set_AsynchronousBlinkingMode(starting_state, interval, interval);
}

void Led::set_AsynchronousBlinkingMode(ledState starting_state, unsigned long int on_interval, unsigned long int off_interval)
{
   init();
   m_Mode = blinking;
   switch_Led(starting_state);
   m_On_Interval_ms = on_interval;
   m_Off_Interval_ms = off_interval;
}

void Led::set_FadeMode(byte fade_step, unsigned long int fade_delay, unsigned long int off_interval)
{
   m_Mode = fade;
   m_FadeDelay = fade_delay;
   m_FadeStep = fade_step;
   m_Off_Interval_ms = off_interval;
   m_FadeState = fade_raising;
   m_Brightness = 0;
}

void Led::set_FadeMode(unsigned long int off_interval)
{
   m_Mode = fade;
   m_FadeDelay = DEFAULT_FADE_DELAY;
   m_FadeStep = DEFAULT_FADE_STEPS;
   m_Off_Interval_ms = off_interval;
   m_FadeState = fade_raising;
   m_Brightness = 0;
}

void Led::set_FadeMode_slow(unsigned long int off_interval)
{
   m_Mode = fade;
   m_FadeDelay = 30;
   m_FadeStep = 5;
   m_Off_Interval_ms = off_interval;
   m_FadeState = fade_raising;
   m_Brightness = 0;
}

void Led::set_FadeMode_fast(unsigned long int off_interval)
{
   m_Mode = fade;
   m_FadeDelay = 5;
   m_FadeStep = 5;
   m_Off_Interval_ms = off_interval;
   m_FadeState = fade_raising;
   m_Brightness = 0;
}

void Led::update()
{
   unsigned long int timeNow = millis();
   if(m_Mode == blinking)
   {
      if( m_State )
      {
         // On interval active
         if(timeNow - m_Last_Time_State_Changed_ms >= m_On_Interval_ms)
         {
            toggle();
         }
      }
      else
      {
         // Off interval active
         if(timeNow - m_Last_Time_State_Changed_ms >= m_Off_Interval_ms)
         {
            toggle();
         }
      }
   }

   if(m_Mode == fade)
   {
      if( m_FadeState == fade_raising )
      {
         if( timeNow - m_Last_Time_State_Changed_ms >= m_FadeDelay )
         {
            if( m_Brightness + m_FadeStep <= 255)
            {
               set_Brightness(m_Brightness + m_FadeStep);
            }
            else
            {
               m_FadeState = fade_falling;
            }
         }
      }
      else if( m_FadeState == fade_falling )
      {
         if( timeNow - m_Last_Time_State_Changed_ms >= m_FadeDelay )
         {
            if( m_Brightness - m_FadeStep >= 0)
            {
               set_Brightness(m_Brightness - m_FadeStep);
            }
            else
            {
               m_FadeState = fade_paused;
            }
         }
      }
      else
      {
         if( timeNow - m_Last_Time_State_Changed_ms >= m_Off_Interval_ms )
         {
            set_Brightness(0);
            m_FadeState = fade_raising;
         }
      }
   }

   if( m_Mode == continuous )
   {
      switch_Led(m_State);
   }
}