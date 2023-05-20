#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int AirValue = 500;    //you need to replace this value with Value_1
const int WaterValue = 365;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue) / 3;
int soilMoistureValue = 0;
int relay = 7;
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  Serial.begin(9600);  // open serial port, set the baud rate to 9600 bps
  pinMode(relay, OUTPUT);
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(5000);
  lcd.clear();
  
}
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Soil Status:");
  lcd.setCursor(0, 1);

  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals)) {
    Serial.println("Very Wet");
    lcd.print("Very Wet");
  } else if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals)) {
    Serial.println("Wet");
    lcd.print("Wet");
  } else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals)) {
    Serial.println("Dry");
    lcd.print("Dry");
    //delay(2000);
    lcd.clear();
    pump();
  }
  delay(2000);
  lcd.clear();
  
}

void pump() {
  lcd.setCursor(0, 0);
  lcd.print("Pumping...");
  Serial.println("Pumping...");
  lcd.setCursor(0, 1);
  bool status = false;
  
  while (status == false) {
    digitalWrite(relay, HIGH);
    soilMoistureValue = analogRead(A0);
    if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals)) {
      status = true;
      digitalWrite(relay, LOW);
      Serial.println("Stop");
      lcd.print("Stop");
      delay(2000);
    }
    Serial.println("Looping");
  }
}
