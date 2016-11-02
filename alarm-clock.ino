#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();

int hPotPin = 1;
int mPotPin = 0;
int alarmTime = 0;
int[] alarmDigits = {0,0,0,0};
int isAlarmOn = false;

void setup(void) {
  Serial.begin(9600);

  setSyncProvider(RTC.get);
  if(timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");

  #ifndef __AVR_ATtiny85__
    Serial.println("Starting lcd display");
  #endif
    matrix.begin(0x70);
  matrix.setBrightness(2);
  matrix.blinkRate(3);
}

void loop(void) {
  //    matrix.writeDigitNum(1, 0);
  //    matrix.print(alarmTime);
  //    matrix.writeDisplay();
  int hPotVal = analogRead(hPotPin);
  int mPotVal = analogRead(mPotPin);
  alarmTime = potTimeParser(hPotVal, mPotVal);
  alarmDigits = digitsParser(alarmTime);
}

int[] potTimeParser(int hPot, int mPot) {
  int h = hPot / 41 - 1;
  int min = mPot / 85 * 5;
  if (hour >= 23) hour = 23;
  if (min <= 0) min = 0;
  if (min >= 55) min = 55;
  return hour * 100 + min;
}

int[] digitsParser(int time) {
  int h = time / 100;
  int m = time % 100;
  if (h < 0) {
    return [-1,-1,-1,-1];
  }
  else if (h == 0) {
    return [0,0,m/10,m%10];
  }
  else {
    return [h/10,h%10,m/10,m%10];
  }
}
