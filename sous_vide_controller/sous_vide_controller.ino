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

#define STATEstartup 0
#define STATEunreferenced 1
#define STATEstandby 2
#define STATEactiveCool 4
#define STATEactiveHeat 5
#define STATEactiveStirCool 6
#define STATEactiveStirHeat 7
#define STATEfault 8
#define STATEtargetting 9

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//========================================

const int maxTemp = 250;
const int minTemp = 50;


//FLAGS
bool FLAGtargetted = 0;
bool FLAGreferenced = 0;
bool FLAGactiveSwitch = 0;
bool FLAGtargettingSwitch = 0;
bool FLAGreferencingSwitch = 0;

const int buttonPinReference = 2;
long lastDebounceTimeReference = 0;
int buttonStateReference;  
int lastButtonStateReference = LOW; 

const int vPinRef = A0;
const int vPin1Temp = A1;

int currentState = stateUnreferenced;
int nextState = stateUnreferenced;
int refVoltage = -1;
int refTemp = -1;

long debounceDelay = 50;

void setup()
{
  pinMode(buttonPinReference, INPUT);
  lcd.begin (20,4,LCD_5x8DOTS);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  Serial.begin(9600);
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
}
void loop()
{
  lcd.setBacklight(HIGH);
  lcd.home();  
  
  boolean refButtonClicked = getButtonPush(buttonPinReference,&lastDebounceTimeReference,&buttonStateReference,&lastButtonStateReference);

  if(refButtonClicked)
  {
    nextState = (currentState!=stateReferencing)?stateReferencing:stateUnreferenced;
  }
  
  if(nextState != currentState)
  {
    lcd.clear();
    currentState = nextState; 
  } 

  activateState(currentState);


}
