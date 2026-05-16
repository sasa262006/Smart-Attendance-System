#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// استدعاء ملفات المشروع المقسمة
#include "Hardware_Config.h"
#include "Biometric_Storage.h"
#include "Network_Report.h"

void setup() {
    Serial.begin(115200);
    mySerial.begin(57600);
    espSerial.begin(115200);
    
    pinMode(buzzer, OUTPUT);
    pinMode(greenLED, OUTPUT);
    
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }
    
    if (!SD.begin(chipSelect)) {
        Serial.println("SD Card failed or not present");
        return;
    }
    
    finger.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Fingerprint sensor connected");
    } else {
        Serial.println("Fingerprint sensor not found");
        while (1);
    }
    lcd.clear();
    lcd.print("Ready for scan");
}

void loop() {
    Serial.println("Press 'r' to register a new fingerprint");
    if (Serial.available()) {
        char command = Serial.read();
        if (command == 'r') {
            registerFingerprint();
        }
    }
    
    int id = getFingerprintID();
    if (id > 0) {
        String name = getUserName(id);
        Serial.print("User ID: "); Serial.println(id);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome: ");
        lcd.setCursor(0, 1);
        lcd.print(name);
        
        digitalWrite(buzzer, HIGH);
        digitalWrite(greenLED, HIGH);
        delay(1000);
        digitalWrite(buzzer, LOW);
        digitalWrite(greenLED, LOW);
        
        logAttendance(name);
        sendReport();
    }
}
