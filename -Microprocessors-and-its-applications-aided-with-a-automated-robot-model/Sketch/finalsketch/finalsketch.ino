#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 6
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
LiquidCrystal_I2C lcd(0x27,16,2);
#define REPORTING_PERIOD_MS 1000
const int trigPin = 2;
const int echoPin = 3;
MAX30100 sensor;
int a;
// Create a PulseOximeter object
PulseOximeter pox;
long duration;
int distance;
// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// Callback routine is executed when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);
    pinMode(A0,OUTPUT);
    pinMode(A1,OUTPUT);
    pinMode(A2,OUTPUT);
    pinMode(A3,OUTPUT);
    Serial.begin(9600);
    Serial1.begin(9600);
    dht.begin();
    sensor_t sensor1;
    dht.temperature().getSensor(&sensor1);
    dht.humidity().getSensor(&sensor1);
    delayMS = sensor1.min_delay / 1000;
    lcd.init();
    lcd.init();
    lcd.backlight();
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      Serial.print("Heart rate:");
      Serial.print(pox.getHeartRate());
      Serial.print("bpm / SpO2:");
      Serial.print(pox.getSpO2());
      Serial.println("%");
      sensors_event_t event;
      dht.temperature().getEvent(&event);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("T: ");
      lcd.setCursor(2,0);
      lcd.print(event.temperature);
      
      lcd.setCursor(9,0);
      lcd.print("HB: ");
      lcd.setCursor(12,0);
      lcd.print(pox.getHeartRate());
      while(Serial1.available())
      {
        char t=Serial1.read();
        //Serial.println(t);
        if(t=='F')
        {
          Serial.println("FORWARD");
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          duration = pulseIn(echoPin, HIGH);
          distance= duration*0.034/2;
          if(distance>15)
          {
            digitalWrite(A0,LOW);
            digitalWrite(A1,HIGH);
            digitalWrite(A2,LOW);
            digitalWrite(A3,HIGH);
            lcd.setCursor(0,1);
            lcd.print("FORWARD");
            pox.begin();
            pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
            pox.setOnBeatDetectedCallback(onBeatDetected);
          }
          if(distance<15)
          {
            digitalWrite(A0,LOW);
            digitalWrite(A1,LOW);
            digitalWrite(A2,LOW);
            digitalWrite(A3,LOW);
          }
        }
        if(t=='B')
        {
          digitalWrite(A0,HIGH);
          digitalWrite(A1,LOW);
          digitalWrite(A2,HIGH);
          digitalWrite(A3,LOW);
          lcd.setCursor(0,1);
          lcd.print("REVERSE");
          pox.begin();
          pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
          pox.setOnBeatDetectedCallback(onBeatDetected);
        }
        if(t=='R')
        {
          digitalWrite(A0,LOW);
          digitalWrite(A1,HIGH);
          digitalWrite(A2,HIGH);
          digitalWrite(A3,LOW);
          lcd.setCursor(0,1);
          lcd.print("RIGHT");
          pox.begin();
          pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
          pox.setOnBeatDetectedCallback(onBeatDetected);
        }
        if(t=='L')
        {
          digitalWrite(A0,HIGH);
          digitalWrite(A1,LOW);
          digitalWrite(A2,LOW);
          digitalWrite(A3,HIGH);
          lcd.setCursor(0,1);
          lcd.print("LEFT");
          pox.begin();
          pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
          pox.setOnBeatDetectedCallback(onBeatDetected);
        }
        if(t=='S')
        {
          digitalWrite(A0,LOW);
          digitalWrite(A1,LOW);
          digitalWrite(A2,LOW);
          digitalWrite(A3,LOW);
          // lcd.setCursor(0,1);
          // lcd.print("STOP");
        }
      }
      tsLastReport = millis();
  }
  
}

void configureMax30100() {
  sensor.setMode(MAX30100_MODE_SPO2_HR);
  sensor.setLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_27_1MA);
  sensor.setLedsPulseWidth(MAX30100_SPC_PW_1600US_16BITS);
  sensor.setSamplingRate(MAX30100_SAMPRATE_100HZ);
  sensor.setHighresModeEnabled(true);
}