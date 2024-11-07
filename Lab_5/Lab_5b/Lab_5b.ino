#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(7);  
  Serial.println("hello");
}

void loop () {
  static int i = 0;
  // Wait until you receive input from the serial interface
  while(Serial.available() == 0) {};
  int colour = Serial.parseInt();
  if ((colour >= 0) && (colour <= 7)){
    lcd.setBacklight(colour);
  }
}
