#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

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

const int buttonPin = 2;
const int refPin = A0;
const int tempPin1 = A1

bool displayState = true;
bool referencing = false;
int refVoltage = -1;
int refTemp = -1;

long lastDebounceTime = 0;
long debounceDelay = 50;
int buttonState;  
int lastButtonState = HIGH;   

void setup()
{
  pinMode(buttonPin, INPUT);
  lcd.begin (20,4,LCD_5x8DOTS);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
}

void loop()
{
 
  int buttonReading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      if(reading==HIGH)
      {
        referencing=false
      }
      else
      {
        referencing=true
      }
      buttonState = reading;

      if (displayState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  
  lastButtonState = reading;
  
  
  
  //Turn the display backlight on or off
  //========================================
  if(displayState==true)
  {
    lcd.setBacklight(HIGH);
  }
  else
  {
    lcd.setBacklight(LOW);
  }    
  //========================================
  lcd.home ();
  
  if(refTemp==-1)
  {
    lcd.print("Please set the reference temperature");
  }
  else
  {
    lcd.print(refTemp);
  }

  int buttonVal = analogRead(buttonPin);
  if()
  int sensorValue = analogRead(refPin);
  float voltage = sensorValue * (5.0 / 1023.0);

}
