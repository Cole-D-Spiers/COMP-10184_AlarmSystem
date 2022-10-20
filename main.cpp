// COMP-10184 – Mohawk College 
// AlarmSystem
// 
// This is an alarm system system, flashes when movement button while flashing within 10 seconds dissables. if too slow light stays on until reset. if pressed it will turn off until u turn it back on to find another movement.
//
// @author    Cole Spiers
// @id        000839039
//
// I, Cole Spiers, student number 000389039, certify that this material is my original work. No other person's work has been used without due acknowledgment and I have not made my work available to anyone else.
//


#include <Arduino.h>


//the pin ports for the button and movement sensor. 
#define PIN_S D5
#define PIN_B D6

//the ten seconds for the alarm to go off
#define ALR_COUNT_TIME 10000
//interval for LED flashing
#define LED_COUNT_TIME 125

//defines alarm states
#define ALR_OFF 1
#define ALR_START 2
#define ALR_COUNT 3
#define ALR_ON 4

//if the led is on or off (deafult of false(off))
bool LedState = false;

//the current status of the whole alarm
int  alarmStateNow;

//the time since the program started (millis)
long alrTimerStart1;
//current time for the alarm active
long alarmCurrentTime;

//the time since the program started (millis)
long alrTimerStart2;
//the current led timer thingy
long timeForLED;

//the previous state of the button, deafult of HIGH
int buttonPrev = HIGH;
//the buttons state right now
int buttonState;

//the state of the motion sensor right now
bool stateOfSensor;
// ***********************************************************

//grabs the digi inputs and what not
void inputYoink()
{
  //reads the current state of the motion sensor
  stateOfSensor = digitalRead(PIN_S); 
  //reads the current state of the button
  buttonState = digitalRead(PIN_B); 
}
// ***********************************************************

//double checks the current state of the button so that it does not keep going
bool buttonCheck()
{
  if(buttonState == LOW && buttonPrev == HIGH)
  {
    //the right now button state == LOW and the prev is HIGH then make the previous the current state and return true
    buttonPrev = buttonState;
    return true;
  }
  else
  {
    //if not then make the prev the current and return false
    buttonPrev = buttonState;
    return false;
  }
}
// ***********************************************************

//checks which current state the alarm is in
void alrStateeLook()
{
  //NOTE TO COLE PLEASE DOUBLE CHECK THIS SWITCH WORKS AT THE END I LEGIT DONT KNOW IF IT REALLY DOES WORK LMAOOOOOOO
  switch(alarmStateNow)
  {
    case ALR_ON:
      //turns the led on (LOW)
      digitalWrite(LED_BUILTIN, LOW); 
      break;
      // ***********************************************************

    case ALR_OFF:
      //if the button is pressed make the alarm go again
      if(buttonCheck()) 
      {
        alarmStateNow = ALR_START;
      }
      break;
    // ***********************************************************

    case ALR_START:
      if(stateOfSensor)
      {
        alrTimerStart2 = millis();
        alrTimerStart1 = millis();
        alarmStateNow = ALR_COUNT;
      }
      //if the button was pressed while the alarm is flashing then turn it off
      if(buttonCheck()) alarmStateNow = ALR_OFF;
      break;
      // ***********************************************************

    case ALR_COUNT:
      //find the amount of milliseonds since the program started
      alarmCurrentTime = millis();
      
      //if the alarmCurrentTime minus alrTimerStart1 is greather then (>=) then the countdown the alarm is active
      if((alarmCurrentTime - alrTimerStart1) >= ALR_COUNT_TIME) 
      {
        alarmStateNow = ALR_ON;
      }

      //find the amount of milliseconds for the led intervul
      timeForLED = millis();
      //if the alarmCurrentTime minus alrTimerStart1 is greather then (>=) then the countdown the alarm is active
      if((timeForLED - alrTimerStart2) >= LED_COUNT_TIME)
      {
        if(LedState)
        {
          digitalWrite(LED_BUILTIN, HIGH);

          LedState = false;
        }
        else if(!LedState)
        {
          digitalWrite(LED_BUILTIN, LOW);

          LedState = true;
        }
        alrTimerStart2 = timeForLED;
      }

      //if sir button was pressed it leaves the countdown state and alrTimerStart1 becomes the alarmCurrentTime meaning the larm is now off and led is off
      if(buttonCheck()) 
      {
        alrTimerStart1 = alarmCurrentTime; 
        alarmStateNow = ALR_OFF; 
        digitalWrite(LED_BUILTIN, HIGH);
      }
      break;
      // ***********************************************************

      default:
      break;
  }
}
// ***********************************************************

void setup() 
{ 
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // make the led an output 
  pinMode(LED_BUILTIN, OUTPUT); 
 
  // configure the PIN_S (D5) to a input
  pinMode(PIN_S, INPUT); 
  // rename (D5) later ^
 
  // configure the PIN_B (D6) to a button input 
  pinMode(PIN_B, INPUT_PULLUP);
  // rename (D6) later ^ 

  // starts the alarm on start up of the program
  alarmStateNow = ALR_START;
} 
// ***********************************************************

void loop() 
{ 
  alrStateeLook();
  inputYoink();
}
// ***********************************************************

