// stcpy_test.ino
// Tests the strcpy function developed in a seperate env to ensure it works as empected.
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield screen = Adafruit_RGBLCDShield();
struct employee {
  int  job_grade;
  char pension_status[5];
  char salary[9];
  char id[8];
  char job_title[18];
};
void stringcpy(char destination[], char str[], int charLimit);
void displayEmployee(employee emp);
uint8_t buttons;


void setup() {
    // Board Communication Setup
  Serial.begin(9600);

  // LCD Screen Initialisation
  screen.begin(16,2);
  screen.setCursor(0,0);
  // put your setup code here, to run once:
  employee test1;
  test1.job_grade = 7;
  stringcpy(test1.pension_status, "NPEN", 4);
  stringcpy(test1.salary, "99999.99", 8);
  stringcpy(test1.id, "9999999", 7);
  stringcpy(test1.job_title, "Software Engineer", 17);

  displayEmployee(test1);

  free(&test1);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttons = screen.readButtons();
  if (buttons & BUTTON_SELECT) {
    employee test2;
    test2.job_grade = 7;
    stringcpy(test2.pension_status, "PEN\0", 4);
    stringcpy(test2.salary, "12122.12", 8);
    stringcpy(test2.id, "1234567", 7);
    stringcpy(test2.job_title, "SW", 17);

    displayEmployee(test2);
  }

  if (buttons & BUTTON_UP) {
    displayEmployee(test1);
  }else if (buttons & BUTTON_DOWN) {
    displayEmployee(test2);
  }
}

void stringcpy(char destination[], char str[], int charLimit) {
  // Assumes size of destination is +1 then str.
  int i = 0;
  for (i = 0; str[i] != '\0' && i < charLimit; i++) {
    destination[i] = str[i];

  }
  destination[charLimit + 1] = '\0';
}

void displayEmployee(employee emp) {
  
  // Displays employee's data.
  // Assumes arrows have been drawn & Data is correct. 
  
  // Clears rest of screen
  for (int i = 1; i < 16; i++) {
    screen.setCursor(i,0);
    screen.print("");
    screen.setCursor(i,1);
    screen.print("");
  }
  
  screen.setCursor(1,0);
  screen.print((String)emp.job_grade); // Implement without a string?

  screen.setCursor(3,0);
  screen.print(emp.pension_status);

  screen.setCursor(8,0);
  screen.print(emp.salary);

  screen.setCursor(1,1);
  screen.print(emp.id);

  screen.setCursor(9,1);
  screen.print(emp.job_title);
}
