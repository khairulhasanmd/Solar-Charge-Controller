#include "ACS712.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal.h>

const int voltageSensor = A1;
ACS712  ACS(A0, 5.0, 1023, 100);
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;
const int numReadings = 50;
float readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;  
#define TRUE 1
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
char str[30];
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);


long lastMsg = 0;
float temp = 0;
int inPin = 5;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
//  lcd.print("Hilu");
  pinMode(inPin, INPUT);
  sensors.begin();
  Serial.println(" Measure > 25V  ");
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  delay(1);
}

void loop()
{

    long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

  if (Serial.available() > 0) {
      Serial.readStringUntil('\n') = str;
      Serial.println(Serial.readStringUntil('\n'));
//      lcd.print(Serial.readStringUntil('\n'));
      delay(1);
      
  }
  else{
      
  value = analogRead(voltageSensor);
  vOUT = (value * 5) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));
  float mA = ACS.mA_DC();
  float current = (mA/1000)*1.764705;
  total = total - readings[readIndex];
  readings[readIndex] = current;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;
    long now = millis();
    lastMsg = now;
    sensors.setResolution(12);
    sensors.requestTemperatures(); // Send the command to get temperatures
    temp = sensors.getTempCByIndex(0);
    
  
//  Serial.print("{\"current\":");
//  Serial.print(average);
//  Serial.print(", \"voltage\":");
//  Serial.print(vIN);
//  float power = average * vIN;    
//  Serial.print(", \"power\":");
//  Serial.print(power);
//  Serial.print(", \"temp\":");
//  Serial.print(temp);
//  Serial.println("}");

//  lcd.print("Hilu");
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(average, DEC);
  Serial.print(average);
//  lcd.print("A, ");
//  lcd.setCursor(0, 10);
//  lcd.print(vIN);
//  lcd.print("V");
//  lcd.setCursor(0, 1);
//  lcd.println(power);
//  lcd.print("W, ");
//  lcd.setCursor(10, 0);
//  lcd.print(temp);
//  lcd.print("C");
  
  delay(0);
    
    
    
    
    
    }

  
  }

  
  
}
