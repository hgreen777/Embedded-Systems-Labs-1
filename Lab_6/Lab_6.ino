// Lab_6.ino -> Finite State Machine
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
int my_time = 0; // Necessary for checTimeChange -> DO NOT REMOVE
int specific_delay = 0;
bool checkTimeChange(int interval);
bool checkDelay(int interval);
char checkInput();



char state = 'A';

uint8_t btnInput; 

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setBacklight(1);
  my_time = 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  btnInput = lcd.readButtons();  

  if (btnInput & BUTTON_UP) {
    //Serial.println("btnclicked");
    if (state == 'A') {
      state = 'B';
    }
  }else if (btnInput & BUTTON_RIGHT) {
    state = 'C';
    specific_delay = millis();
  } else if (btnInput & BUTTON_DOWN) {
    if (state == 'C') {
      state = 'A';
    }
  }

  if (state== 'C') {
    if (checkDelay(10000)) {
      state = 'B';
    }
  }

  char inp = checkInput();
  
  if (inp == 'A' & state == 'B') {
    state = 'A';
    //Serial.println(inp);  
  }
  
  if (checkTimeChange(1000)) {
    lcd.setCursor(0,0);
    lcd.write(state);
  }

}


bool checkTimeChange(int interval) {
  if (millis() >= (my_time + interval)) {
    my_time = millis();

    return true;
  }
  return false;
  
}

bool checkDelay(int interval) {
  if (millis() >= (specific_delay + interval)) {
    specific_delay = 0;
    
    return true;
  }
  return false;
  
}


char checkInput() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    return input;
  }
  return 'X';
}
