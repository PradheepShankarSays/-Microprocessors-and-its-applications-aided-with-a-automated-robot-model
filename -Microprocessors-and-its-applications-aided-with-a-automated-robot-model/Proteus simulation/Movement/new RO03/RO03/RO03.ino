#include "dht11.h"
#include "LCD.h"
#include <TimerOne.h>
DHT dht;
char c;
#define D_temp 4
#define motor1_pin1 22
#define motor1_pin2 23
#define motor2_pin1 24
#define motor2_pin2 25
#define trigpin 2
#define echopin 3

int pos = 0;  // variable to store the servo position
int HBSensor = 7; //   HB i/p
int HBCount = 0; //    HB start button
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

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
  pinMode(2, OUTPUT);//ULTRASONIC op
  pinMode(3, INPUT); //ULTRASONIC ip
  pinMode(27, INPUT_PULLUP); //F
  pinMode(28, INPUT_PULLUP); //B
  pinMode(29, INPUT_PULLUP); //S
  pinMode(30, INPUT_PULLUP); //L
  pinMode(26, INPUT_PULLUP); //R
  pinMode(motor1_pin1, OUTPUT);   /* Motor1 control pin 1 */
  pinMode(motor1_pin2, OUTPUT);   /* Motor1 control pin 2 */
  pinMode(motor2_pin1, OUTPUT);   /* Motor2 control pin 1 */
  pinMode(motor2_pin2, OUTPUT);   /* Motor2 control pin 2 */
  pinMode(HBSensor, INPUT);       /* HB i/p               */
  pinMode(HBStart, INPUT_PULLUP); /*HB start Button       */
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  lcd.clear();
  lcd.setCursor(0, 0);
  delay(250);
  lcd.print("WHEELCHAIR");
  delay(1000);
  lcd.clear();
  lcd.setCursor(9,0);
  lcd.print("Time:");
  lcd.setCursor(0,1);
  lcd.print("HBperMin: 0");
  
}
//Function To Recieve Data From Bluetooth Device and move
void loop()
{ //DHT11 reading
  dht.dht_read(D_temp);
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(dht.temperature);
  //HB Sensor Reading
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
          HBperMin = HBCount * 6;
          HBStartCheck = 0;
          lcd.setCursor(10,1);
          lcd.print(HBperMin);
          lcd.print(" ");
          HBCount = 0;
          TimeinSec = 0;      
      }
  }
//Ultrasonic detection
  long duration, distance;
  digitalWrite(2, LOW); //
  delayMicroseconds(2);
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);  
  duration = pulseIn(3, HIGH);
  distance = duration/58.2;
  if(distance < 30) {
    lcd.setCursor(0, 1);
    lcd.print("Obstacle ahead ");
    Serial.println("Obstacle ahead ");
    // Stop the motors
    digitalWrite(motor1_pin1, LOW);
    digitalWrite(motor1_pin2, LOW);
    digitalWrite(motor2_pin1, LOW);
    digitalWrite(motor2_pin2, LOW);
    delay(400);
    lcd.clear();
  } 
  else
  {
    while (Serial.available()) //Function To Recieve Data From Bluetooth Device and move
    {
      char c = (char)Serial.read();
      if (c == 'f')
      {
        Serial.print("FORWARD");
        lcd.setCursor(0, 1);
        lcd.print("FORWARD     ");
        digitalWrite(motor1_pin1, HIGH);
        digitalWrite(motor1_pin2, LOW);
        digitalWrite(motor2_pin1, HIGH);
        digitalWrite(motor2_pin2, LOW);

      }
      if (c == 'b')
      {
        Serial.print("REVERSE");
        lcd.setCursor(0, 1);
        lcd.print("REVERSE    ");
        digitalWrite(motor1_pin1, LOW);
        digitalWrite(motor1_pin2, HIGH);
        digitalWrite(motor2_pin1, LOW);
        digitalWrite(motor2_pin2, HIGH);
      }
      if (c == 's')
      {
        Serial.print("STOP");
        lcd.setCursor(0, 1);
        lcd.print("STOP     ");
        digitalWrite(motor1_pin1, LOW);
        digitalWrite(motor1_pin2, LOW);
        digitalWrite(motor2_pin1, LOW);
        digitalWrite(motor2_pin2, LOW);
      }
      if (c== 'l')
       {
        Serial.print("LEFT");
        lcd.setCursor(0, 1);
        lcd.print("LEFT     ");
        digitalWrite(motor1_pin1, HIGH);
        digitalWrite(motor1_pin2, LOW);
        digitalWrite(motor2_pin1, LOW);
        digitalWrite(motor2_pin2, HIGH);
      }
      if (c== 'r')
       {
        Serial.print("RIGHT");
        lcd.setCursor(0, 1);
        lcd.print("RIGHT     ");
        digitalWrite(motor1_pin1, LOW);
        digitalWrite(motor1_pin2, HIGH);
        digitalWrite(motor2_pin1, HIGH);
        digitalWrite(motor2_pin2, LOW);
      }
      
      delay(1000);
    }
    if (digitalRead(27) == 0)
    {
      Serial.print("FORWARD");
      lcd.setCursor(0, 1);
      lcd.print("FORWARD    ");
      digitalWrite(motor1_pin1, HIGH);
      digitalWrite(motor1_pin2, LOW);
      digitalWrite(motor2_pin1, HIGH);
      digitalWrite(motor2_pin2, LOW);

    }
    if (digitalRead(28) == 0)
    {
      Serial.print("REVERSE");
      lcd.setCursor(0, 1);
      lcd.print("REVERSE    ");
      digitalWrite(motor1_pin1, LOW);
      digitalWrite(motor1_pin2, HIGH);
      digitalWrite(motor2_pin1, LOW);
      digitalWrite(motor2_pin2, HIGH);
    }
    if (digitalRead(29) == 0)
    {
      Serial.print("STOP");
      lcd.setCursor(0, 1);
      lcd.print("STOP       ");
      digitalWrite(motor1_pin1, LOW);
      digitalWrite(motor1_pin2, LOW);
      digitalWrite(motor2_pin1, LOW);
      digitalWrite(motor2_pin2, LOW);
    }
    if (digitalRead(30) == 0)
    {
      Serial.print("LEFT");
      lcd.setCursor(0, 1);
      lcd.print("LEFT      ");
      digitalWrite(motor1_pin1, HIGH);
      digitalWrite(motor1_pin2, LOW);
      digitalWrite(motor2_pin1, LOW);
      digitalWrite(motor2_pin2, HIGH);
    }
    if (digitalRead(26) == 0)
    {
      Serial.print("RIGHT");
      lcd.setCursor(0, 1);
      lcd.print("RIGHT      ");
      digitalWrite(motor1_pin1, LOW);
      digitalWrite(motor1_pin2, HIGH);
      digitalWrite(motor2_pin1, HIGH);
      digitalWrite(motor2_pin2, LOW);
    }
  }
  delay(1000);
}
