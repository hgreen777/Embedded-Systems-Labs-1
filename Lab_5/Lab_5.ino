// Lab 5 
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();


unsigned long my_time;
void timeSeconds();

byte upArrowMatrix[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};

byte downArrowMatrix[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setBacklight(6);
  lcd.createChar(0, upArrowMatrix);
  lcd.createChar(1, downArrowMatrix);

  my_time = 0;
  
  lcd.write((uint8_t)0);
  lcd.setCursor(0,1);
  lcd.write((uint8_t)1);
}

void loop() {
  timeSeconds();
}

void timeSeconds() {
  // Without blocking delay
  if (millis() >= (my_time + 1000)) {
    Serial.print("Time: ");
    my_time = millis();
  
    Serial.println(my_time);
  }
}
