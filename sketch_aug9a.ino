#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

DateTime displayTime;
unsigned long previousMillis = 0;
const long normalInterval = 1000;
unsigned long lastChaos = 0;
const long chaosInterval = 10000;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(_DATE), F(TIME_)));
  }

  displayTime = rtc.now();
}

void loop() {
  unsigned long currentMillis = millis();

  // Update clock every second
  if (currentMillis - previousMillis >= normalInterval) {
    previousMillis = currentMillis;
    displayTime = displayTime + TimeSpan(1); // Add 1 second
    displayClock(displayTime);
  }

  // Every 10 seconds: create chaos
  if (currentMillis - lastChaos >= chaosInterval) {
    lastChaos = currentMillis;
    confuseTime();
  }
}

void displayClock(DateTime time) {
  lcd.setCursor(0, 0);
  lcd.print("Useless Clock :) ");

  lcd.setCursor(0, 1);
  if (time.hour() < 10) lcd.print("0");
  lcd.print(time.hour());
  lcd.print(":");
  if (time.minute() < 10) lcd.print("0");
  lcd.print(time.minute());
  lcd.print(":");
  if (time.second() < 10) lcd.print("0");
  lcd.print(time.second());
  lcd.print("       ");
}

void confuseTime() {
  int chaos = random(0, 3);
  int offset = random(1, 30); // Minutes to shift

  if (chaos == 0) {
    displayTime = displayTime + TimeSpan(0, 0, offset, 0);
  } else if (chaos == 1) {
    displayTime = displayTime - TimeSpan(0, 0, offset, 0);
  } else {
    int h = random(0, 24);
    int m = random(0, 60);
    int s = random(0, 60);
    displayTime = DateTime(displayTime.year(), displayTime.month(), displayTime.day(), h, m, s);
  }

  lcd.setCursor(0, 0);
  lcd.print("!! Time Warped !!");
  delay(1000);
}