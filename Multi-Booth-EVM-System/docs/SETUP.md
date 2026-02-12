# Setup Guide

## 1) Arduino IDE Dependencies

Install:

- FirebaseESP32 (or compatible Firebase ESP32 client used by your setup)
- Adafruit Fingerprint Sensor Library
- LiquidCrystal I2C
- Adafruit GFX
- Adafruit SSD1306

## 2) Open Firmware

Open:

`firmware/MultiBoothEVMSystem/MultiBoothEVMSystem.ino`

## 3) Board and Port

- Board: ESP32 Dev Module
- Select proper COM/serial port

## 4) Configure Credentials

Update Wi-Fi and Firebase settings in firmware before upload.
Use `config/env.example` as a reference template.

## 5) Upload and Validate

- Verify sketch
- Upload sketch
- Monitor Serial at 115200
- Check Firebase nodes update during voting
