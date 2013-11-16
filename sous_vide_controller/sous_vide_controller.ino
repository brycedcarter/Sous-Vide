#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include<stdlib.h>


// SETUP LCD
//========================================
#define I2C_ADDR 0x27
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//========================================


// Define states
//========================================
#define STATEstartup 0
#define STATEstandby 1
#define STATEactive 2
#define STATEfault 3
//========================================

// Setup Constants
//========================================
const int maxTemp = 250;
const int minTemp = 50;
//========================================


// Define pins
//=======================================

//Input Pins
const int vPinTarget = A0;
const int vPinTemp1 = A1;
const int vPinTemp2 = A2;
const int vPinTemp3 = A3;
const int vPinTemp4 = A4;
const int inpSwitchActive = 53;

const array inputPins = [vPinTarget,vPinTemp1,vPinTemp2,vPinTemp3,vPinTemp4,
						 inpSwitchActive];


//Output pins
const int outPinMotor = 51;
const int outPinHeater = 52;
const array outputPins = [outPinMotor,outPinHeater];

//=======================================


//FLAGS
//=======================================
int INITIALIZED = false; 
//=======================================

int currentState = stateUnreferenced;
int targetVoltage = -1;
int targetTemp = -1;

void setup()
{
	//Setup pins
	for (i=0;i++;i<inputPins.length)
	{
		int pin = inputPins[i];
		pinMode(pin, INPUT);
	}
	for (i=0;i++;i<outputPins.length)
	{
		int pin = inputPins[i];
		pinMode(pin, OUTPUT);
	}
  
	//Setup LCD
	lcd.begin (20,4,LCD_5x8DOTS);
	lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);

	//Seup Serial (should not be used in final... just for debugging)
	Serial.begin(9600);
}

void activeStateHandler()
{

}

void standbyStateHandler()
{

}

void startupStateHandler()
{	
	lcd.clear();
	currentState = STATEstandby;
}

void faultStateHandler()
{

}

int determineState()
{
	if (currentState!=STATEfault)
	{
		if (INITIALIZED)
		{
			activeSwitchState = digitalRead(inpSwitchActive);
			if (activeSwitchState == HIGH)
			{
				return STATEactive;
			}
			else
			{
				return STATEstandby
			}
		}
		else
		{
			return STATEstartup;
		}
	}
	else
	{
		return STATEfault;
	}
}

void loop()
{
	lcd.setBacklight(HIGH);

	switch (determineState())
	{
		case STATEstartup: 
			startupStateHandler();
		case STATEstandby:
			standbyStateHandler();
		case STATEactive:
			activeStateHandler();
		case STATEfault:
			faultStateHandler();
	}
}

void activateState(int state)
{
  String blankEnding = "     ";
  switch(state)
  {
    case stateReferencing:
    {
      String baseText = "Referencing: ";
      
      int sensorValue = analogRead(vPinRef);
      int refTemp = sensorValue * ((maxTemp-minTemp) / 1023.0)+minTemp;
      lcd.print(baseText+refTemp+blankEnding );
      break;
    }
    case stateUnreferenced:
      lcd.print("please set reference tem");
      break;
  }
}

boolean getButtonPush(int buttonPin,long* lastDebounceTime,int* buttonState,int* lastButtonState)
{  
  int buttonReading = !digitalRead(buttonPin);
  int output = false;

  if (buttonReading != *lastButtonState) {
    *lastDebounceTime = millis();
  } 
  
  if ((millis() - *lastDebounceTime) > debounceDelay) 
  {
    if (buttonReading != *buttonState) 
    {
      *buttonState = buttonReading;
      
      if (*buttonState == HIGH) 
      {
        output = true;
      }
    }
  }
  *lastButtonState = buttonReading;
  
  return output;

