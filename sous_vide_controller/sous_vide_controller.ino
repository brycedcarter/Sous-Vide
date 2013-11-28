#include <String.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <math.h>

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
const long maxTemp = 250;
const long minTemp = 50;
//========================================


// Define pins
//=======================================

//Input Pins
const int vPinTarget = A0;
const int vPinTemp1 = A1;
const int vPinTemp2 = A2;
const int vPinTemp3 = A3;
const int vPinTemp4 = A4;
const int inpSwitchActive = 2;

const int numInputPins = 6;

const int inputPins[] = {vPinTarget,vPinTemp1,vPinTemp2,vPinTemp3,vPinTemp4,inpSwitchActive};


//Output pins
const int outPinMotor = 4;
const int outPinHeater = 3;

const int numOutputPins = 2;
const int numActiveLowPins = 2;

const int outputPins[] = {outPinMotor,outPinHeater};
const int activeLowPins[] = {outPinMotor,outPinHeater};

//=======================================


//FLAGS
//=======================================
int INITIALIZED = false; 
//=======================================

int currentState = STATEstartup;
int targetVoltage = -1;
int targetTemp = -1;
int thermistorConstant = 65;
int upperThresh = 0;
int lowerThresh = 0;
String currentDisplay = " ";
String nextDisplay = "";

void setup()
{
	//Setup pins
	for (int i=0;i<numInputPins;i++)
	{
		int pin = inputPins[i];
		pinMode(pin, INPUT);
	}
	for (int i=0;i<numOutputPins;i++)
	{
		int pin = outputPins[i];
		pinMode(pin, OUTPUT);
		for (int j=0;j<numActiveLowPins;j++)
		{
			if (activeLowPins[j]==pin)
			{
				digitalWrite(pin,HIGH);
			}
		}
	}
  
	//Setup LCD
	lcd.begin (20,4,LCD_5x8DOTS);
	lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);

	//Setup Serial (should not be used in final... just for debugging)
	Serial.begin(9600);
}

//class displayOutput 
//{
//public:
//	displayOutput ();
//	void setLines(int,bool);
//private:
//	int lines;
//};
//
//void displayOutput::setLines(int lines[], bool)
//{
//	this->lines = lines;
//}



double Thermister(int RawADC) {
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
 return Temp;
}

void activeStateHandler()
{
	int sensorValueTarget = analogRead(vPinTarget);
	targetTemp = sensorValueTarget * ((maxTemp-minTemp) / 1023.0)+minTemp;

	int sensorValueTemp = analogRead(vPinTemp1);
	int currentTemp = Thermister(sensorValueTemp);
	String status;
	if (currentTemp < targetTemp-lowerThresh)
	{
		status = "HEATING";
		digitalWrite(outPinHeater,LOW);
	}
	else if (currentTemp > targetTemp+upperThresh)
	{
		status = "COOLING";
		digitalWrite(outPinHeater,HIGH);
	}
	nextDisplay = String("  ::ACTIVE MODE::    ")+currentTemp;
}

void standbyStateHandler()
{
	digitalWrite(outPinHeater,HIGH);
	int sensorValue = analogRead(vPinTarget);
	targetTemp = sensorValue * ((maxTemp-minTemp) / 1023.0)+minTemp;
	nextDisplay = String("  ::STANDBY MODE::   Target Temp = ")+targetTemp;
}

void startupStateHandler()
{	
	nextDisplay = "Initializing...";
	INITIALIZED = true;
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
			int activeSwitchState = digitalRead(inpSwitchActive);
			Serial.println(activeSwitchState);
			if (activeSwitchState == HIGH)
			{
				return STATEactive;
			}
			else
			{
				return STATEstandby;
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
	int currentState = determineState();
	switch (currentState)
	{
		case STATEstartup: 
			startupStateHandler();
			break;
		case STATEstandby:
			standbyStateHandler();
			break;
		case STATEactive:
			activeStateHandler();
			break;
		case STATEfault:
			faultStateHandler();
			break;
	}

	if (currentDisplay!=nextDisplay)
	{
		currentDisplay = nextDisplay;
		lcd.clear();
		lcd.print(currentDisplay);
	}
}

