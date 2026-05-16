#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

#include <Adafruit_Fingerprint.h>
#include <SD.h>
#include <RTClib.h>

// تعيين المنافذ والبروتوكولات
extern SoftwareSerial mySerial(2, 3);   // لمستشعر البصمة
extern SoftwareSerial espSerial(8, 9); // لوحدة ESP8266

extern Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
extern LiquidCrystal_I2C lcd(0x27, 16, 2);
extern RTC_DS3231 rtc;

const int buzzer = 7;
const int greenLED = 6;
const int chipSelect = 10;

#endif
