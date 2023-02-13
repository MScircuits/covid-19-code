#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 4);

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);
 
unsigned char UcHeartBeat = 0;
unsigned char UcSpo2 = 0;


#define REPORTING_PERIOD_MS     1000

PulseOximeter pox;

unsigned int tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
 
}

void setup()
{
  Serial.begin(9600);
  sensors.begin();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Covid-19 Health");
  lcd.setCursor(0, 1);
  lcd.print("Mont Using SPo2");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(3000);
  lcd.clear();
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin())
  {
    Serial.println("FAILED");
    for (;;);
  } else
  {
    Serial.println("SUCCESS");

  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);

}

void loop()
{
   pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    lcd.clear();
   
    lcd.setCursor(0, 0);
    lcd.print("BPM:");
    UcHeartBeat = int(pox.getHeartRate());
    lcd.print(UcHeartBeat);
    Serial.println(UcHeartBeat);
    lcd.print(" ");
    lcd.print("SpO2:");
    UcSpo2 = pox.getSpO2();
    lcd.print(UcSpo2);
    Serial.println(UcSpo2);
    lcd.print("%");
    //Wait for a bit to keep serial data from saturating
    delay(1);
    tsLastReport = millis();
   
  }
}   
