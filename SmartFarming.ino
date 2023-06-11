#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int AirValue = 500;    //you need to replace this value with Value_1
const int WaterValue = 380;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue) / 3;
double soilMoistureValue = 0;
const int relay = 7;
double totalPercentage = 0;
double averagePercentage = 0;
double percentage = 0;
int count = 0;
double arrayPercentage[10];
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  Serial.begin(9600);  // open serial port, set the baud rate to 9600 bps
  pinMode(relay, OUTPUT);
  lcd.begin();

  // Turn on the backlight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
  
}
void loop() {

  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  Serial.println(soilMoistureValue);
  percentage = ((AirValue - soilMoistureValue) / (AirValue - WaterValue)) * 100;

  if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals)) {
    Serial.println("Very Wet");
  } else if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals)) {
    Serial.println("Wet");
  } else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals)) {
    Serial.println("Dry");
    delay(2000);
    lcd.clear();
    pump();
  }

  totalPercentage = 0;  // Resets totalPercentage to 0
  Serial.println(percentage);
  
  if (count > 9) {  // Resets pointer to 0 once counter exceeds 9
    count = 0;
  } 

  arrayPercentage[count] = percentage;  // Adds percentage into arrayPercentage array

  for (int i = 0; i < 10; i ++) {   // Calculates totalPercentage from array

    totalPercentage += arrayPercentage[i];
  }

  int validValues = 0;  // Resets number of non-0 values in array to 0

  for (int j = 0; j < 10; j ++) {

    if (arrayPercentage[j] != 0) {
      
      validValues += 1;
    }
  }

  averagePercentage = totalPercentage / validValues;
  String averageString ="";
  averageString = String(averagePercentage, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Average Moisture");
  lcd.setCursor(0, 1);
  lcd.print("     " + averageString + "%");
  count += 1;
  delay(5000);
}

void pump() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pumping...");
  Serial.println("Pumping...");
  delay(2000);
  lcd.setCursor(0, 1);
  bool status = false;  // 
  
  while (true) {
    digitalWrite(relay, HIGH);
    soilMoistureValue = analogRead(A0);
    Serial.println(soilMoistureValue);
    delay(5000);
    digitalWrite(relay, LOW);
    delay(5000);
    if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals)) {
      digitalWrite(relay, LOW);
      Serial.println("Stop");
      delay(2000);
      break;
    }
  }
}
