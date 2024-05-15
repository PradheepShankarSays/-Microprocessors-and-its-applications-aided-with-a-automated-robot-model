#include "LCD.h"
#include "dht11.h"
#include <TimerOne.h>

DHT dht;
char c;
#define D_temp 4

int HBSensor = 7;
int HBCount = 0;
int HBCheck = 0;
int TimeinSec = 0;
int HBperMin = 0;
int HBStart = 6;
int HBStartCheck = 0;

void timerIsr()
{
  if(HBStartCheck == 1)
  {
      TimeinSec = TimeinSec + 1;
      lcd.setCursor(14,0);
      lcd.print(TimeinSec);
      lcd.print(" ");
  }
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  delay(250);
  lcd.print("WHEELCHAIR");
  delay(1000);
  lcd.clear();
  pinMode(HBSensor, INPUT);
  pinMode(HBStart, INPUT_PULLUP);
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  lcd.clear();
  lcd.setCursor(9,0);
  lcd.print("Time:");
  lcd.setCursor(0,1);
  lcd.print("HBperMin: 0");
}

void loop() {
  dht.dht_read(D_temp);
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(dht.temperature);

  if(digitalRead(HBStart) == LOW){HBStartCheck = 1;}
  if(HBStartCheck == 1)
  {
      if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
      {
        HBCount = HBCount + 1;
        HBCheck = 1;
        lcd.setCursor(13,1);
        //lcd.print(HBCount);
        //lcd.print(" ");
      }
      if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
      {
        HBCheck = 0;   
      }
      if(TimeinSec == 10)
      {
          HBperMin = HBCount * 24;
          HBStartCheck = 0;
          lcd.setCursor(10,1);
          lcd.print(HBperMin);
          lcd.print(" ");
          HBCount = 0;
          TimeinSec = 0;      
      }
  }
}
