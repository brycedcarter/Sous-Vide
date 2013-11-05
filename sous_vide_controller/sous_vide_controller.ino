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

#define stateReferencing 0
#define stateUnreferenced 1
#define stateNormOperation 2

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//========================================

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
      float voltage = sensorValue * (5.0 / 1023.0);
      lcd.print(baseText+sensorValue+blankEnding );
      break;
    }
    case stateUnreferenced:
      lcd.print("please set reference temp");
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
