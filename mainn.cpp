#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // 0x27 คือที่อยู่ของจอ LCD และ 16x2 คือขนาดของจอ LCD

const int startStopSwitchPin = D5;  // สวิตช์เริ่มหยุดการนับเวลา (D5)
const int resetSwitchPin = D6;       // สวิตช์รีเซ็ตเวลา (D6)

unsigned long previousMillis = 00;    // เก็บค่าเวลาของการตั้งเวลาล่าสุด
unsigned long currentMillis = 00;     // เวลาปัจจุบันที่ใช้ในการคำนวณ
unsigned long elapsedTime = 00;       // เวลาที่ผ่านไปตั้งแต่เริ่มนับเวลา
boolean isTiming = false;            // สถานะการนับเวลา (true = กำลังนับ, false = หยุด)

// ประกาศฟังก์ชันแสดงผลเวลาไว้ก่อนที่จะใช้งาน
void displayTime(unsigned long minutes, unsigned long seconds, unsigned long milliseconds);

void setup() {
  pinMode(startStopSwitchPin, INPUT_PULLUP);  // กำหนดให้สวิตช์เริ่มหยุดการนับเวลาเป็น INPUT และใช้ Pull-up resistor
  pinMode(resetSwitchPin, INPUT_PULLUP);      // กำหนดให้สวิตช์รีเซ็ตเวลาเป็น INPUT และใช้ Pull-up resistor
  lcd.init();                                  // กำหนดการเริ่มต้นของจอ LCD
  lcd.backlight();                             // เปิดไฟหลังจอ LCD
  displayTime(00, 00, 00);                        // แสดงผลเวลาเริ่มต้นที่ 0:0:0
}

void loop() {
  int startStopSwitchState = digitalRead(startStopSwitchPin);  // อ่านสถานะของสวิตช์เริ่มหยุดการนับเวลา
  int resetSwitchState = digitalRead(resetSwitchPin);          // อ่านสถานะของสวิตช์รีเซ็ตเวลา

  // ตรวจสอบการกดสวิตช์เริ่มหยุดการนับเวลา
  if (startStopSwitchState == LOW) {
    isTiming = !isTiming;  // เปลี่ยนสถานะการนับเวลา (เริ่มหรือหยุด)
    delay(200);  // รอเพื่อป้องกันการกระพริบของสวิตช์
    previousMillis = millis();  // บันทึกเวลาปัจจุบันเพื่อใช้ในการคำนวณ
  }

  // ตรวจสอบการกดสวิตช์รีเซ็ตเวลา
  if (resetSwitchState == LOW) {
    elapsedTime = 0;  // รีเซ็ตเวลากลับไปที่ 0
    displayTime(00, 00, 00);  // แสดงผลเวลาที่รีเซ็ต
    delay(200);  // รอเพื่อป้องกันการกระพริบของสวิตช์
  }

  // หากกำลังนับเวลา
  if (isTiming) {
    currentMillis = millis();  // นับเวลาปัจจุบัน
    elapsedTime += currentMillis - previousMillis;  // บวกเวลาที่ผ่านไปตั้งแต่ครั้งที่เริ่มนับเวลาล่าสุด
    previousMillis = currentMillis;  // บันทึกเวลาปัจจุบันเพื่อใช้ในการคำนวณต่อไป

    // แปลงเวลาที่ผ่านไปเป็น นาที:วินาที:มิลลิเซค
    unsigned long minutes = (elapsedTime / 60000) % 60;  // นาที
    unsigned long seconds = (elapsedTime / 1000) % 60;    // วินาที
    unsigned long milliseconds = elapsedTime % 1000;     // มิลลิเซค

    // แสดงผลเวลาบนจอ LCD
    displayTime(minutes, seconds, milliseconds);
  }
}

// ฟังก์ชั่นแสดงผลเวลาบนจอ LCD
void displayTime(unsigned long minutes, unsigned long seconds, unsigned long milliseconds) {
  //lcd.clear();  // ล้างจอ LCD
  lcd.setCursor(0, 0);  // ตำแหน่งแถวและคอลัมน์ของการแสดงผล
  lcd.print("Time: ");
  lcd.setCursor(6, 0);  // ตำแหน่งแถวและคอลัมน์ของการแสดงผล
  lcd.print("m");
  lcd.setCursor(9, 0);  // ตำแหน่งแถวและคอลัมน์ของการแสดงผล
  lcd.print("s");
  lcd.setCursor(13, 0);  // ตำแหน่งแถวและคอลัมน์ของการแสดงผล
  lcd.print("ms");
  lcd.setCursor(6, 1);
  lcd.print(minutes);
  lcd.print("  ");
  lcd.print(seconds);
  lcd.print("  ");
  lcd.print(milliseconds);
  lcd.print("                  ");
}
