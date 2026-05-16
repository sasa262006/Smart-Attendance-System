#ifndef BIOMETRIC_STORAGE_H
#define BIOMETRIC_STORAGE_H

#include <Arduino.h>
#include "Hardware_Config.h"

extern File attendanceFile;
extern File usersFile;

int getFingerprintID() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK) return -1;
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) return -1;
    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK) return -1;
    return finger.fingerID;
}

bool fingerEnroll(int id) {
    int p;
    Serial.println("Place your finger...");
    while ((p = finger.getImage()) != FINGERPRINT_OK);
    p = finger.image2Tz(1);
    if (p != FINGERPRINT_OK) return false;
    Serial.println("Remove your finger");
    delay(2000);
    Serial.println("Place your finger again...");
    while ((p = finger.getImage()) != FINGERPRINT_OK);
    p = finger.image2Tz(2);
    if (p != FINGERPRINT_OK) return false;
    p = finger.createModel();
    if (p != FINGERPRINT_OK) return false;
    p = finger.storeModel(id);
    if (p != FINGERPRINT_OK) return false;
    return true;
}

void registerFingerprint() {
    int id;
    Serial.println("Enter new fingerprint ID: ");
    while (!Serial.available());
    id = Serial.parseInt();
    
    Serial.print("Registering ID "); Serial.println(id);
    if (fingerEnroll(id)) {
        Serial.println("Enter user name: ");
        while (!Serial.available());
        String name = Serial.readStringUntil('\n');
        name.trim();
        
        usersFile = SD.open("users.txt", FILE_WRITE);
        if (usersFile) {
            usersFile.print(id);
            usersFile.print(",");
            usersFile.println(name);
            usersFile.close();
        }
        Serial.println("Fingerprint registered successfully!");
    } else {
        Serial.println("Failed to register fingerprint");
    }
}

String getUserName(int id) {
    usersFile = SD.open("users.txt");
    if (usersFile) {
        while (usersFile.available()) {
            String line = usersFile.readStringUntil('\n');
            int storedID = line.substring(0, line.indexOf(',')).toInt();
            if (storedID == id) {
                String name = line.substring(line.indexOf(',') + 1);
                usersFile.close();
                return name;
            }
        }
        usersFile.close();
    }
    return "Unknown";
}

void logAttendance(String name) {
    attendanceFile = SD.open("attendance.csv", FILE_WRITE);
    if (attendanceFile) {
        DateTime now = rtc.now();
        attendanceFile.print(name);
        attendanceFile.print(",");
        attendanceFile.print(now.year());
        attendanceFile.print("/");
        attendanceFile.print(now.month());
        attendanceFile.print("/");
        attendanceFile.print(now.day());
        attendanceFile.print(",");
        attendanceFile.print(now.hour());
        attendanceFile.print(":");
        attendanceFile.print(now.minute());
        attendanceFile.println();
        attendanceFile.close();
    }
}

#endif
