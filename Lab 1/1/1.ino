// Lab1.ino

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();  // Create lcd object which has methods to control the display
// lcd.write() prints the variable one character at a time
// lcd.print(var); prints the variable var to the display

void basicInput();
void slightlyBetterInput();
void writingToLCD();
void lcdButtons();
void stringFunction();
void printing();

void clearSpaces(int x);

char incomingChar = 0;
String incomingString = "";
uint8_t buttons;

void setup() {
  Serial.begin(9600);  // Define the baud rate for serial communication.

  lcd.begin(16, 2);     // lcd.begin(cols,rows); Not 0 indexed
  lcd.setBacklight(1);  // 0->7 for colours
  lcd.setCursor(0, 0);  // lcd.setCursor(x, y);

  Serial.println("Hello, World!");
  lcd.println(F("Hello World"));  // Adds characters to the end (escape character for newline and carriage return)
  // ^^ fix could be a constant for hello world and trim it 
  // Flash memory to save on memory -> see printing 
  lcd.setCursor(0, 1); 
  

}

void loop() {
  
  lcdButtons();
}

// Functions

void basicInput() {
  // Serial.available gets the num of bytes (characters) available for reading from the serial port.
  // [data that has already arrived and stored in the serial recieve buffer (holds 64bytes)]
  if (Serial.available() > 0) {  // Only true when data is stored in the serial recieve buffer.
    // Having both uncommented means the first line removes one letter from the buffer and
    // the second line takes the rest of the buffer as a string to print it.
    incomingChar = Serial.read();          // Takes each character from the buffer
    incomingString = Serial.readString();  // Takes whole string from buffer

    Serial.print("String recieved: ");
    Serial.println(incomingString);
    Serial.print("I recieved: ");
    Serial.println(incomingChar);
  }
}

void slightlyBetterInput() {
  Serial.println("Enter data:");
  // Paused execution until data available wheres if statement allows further execution
  while (Serial.available() == 0) {}  // Wait for available data

  String inputString = Serial.readString();  // read until timeout [ie end of buffer]
  // Serial.readStringUntil() -> reads till terminating character (char). What is the benefit??
  inputString.trim();  // remove any \r \n whitespace at the end of the string

  if (inputString == "COA202") {
    Serial.println("This is Embedded Systems Programming");
  } else {
    Serial.println("This is something else");
  }
}

void writingToLCD() {
  /*  write accepts a byte as an argument, 
      as a string is passed it is not a single character and therefore not a byte.
      The pointer to the string (char arr) is converted to a number and that ASCII 
      character is shown.
      */
  lcd.write("W");

  lcd.setCursor(0, 1); 
  lcd.print("Hello ");


  lcd.write('W');
  lcd.write('o');
  lcd.print("rld");

}

void lcdButtons() {
  buttons = lcd.readButtons();
  // Bitstring (binary integer) each bit corresponds to a button
  // the last bit, least significant BIT is for select (1), then right (10 = 2) etc etc

  //Serial.println(uint8_t(buttons), BIN);
  // Buttons usually returns 0, however if a button is pressed, then a number is displayed (the number correlating with the byte)
  // ie 0 = nothing, 1 = select, 2 = right, 4 = down, 8 = up, left = 16 etc
  // Clear the screen with new messages ""
  // When button release, should be hidden 
  // Write a function to clear x spaces from starting position x 
  lcd.setCursor(0,1);
  if (buttons & BUTTON_RIGHT) {
    lcd.print("RIGHT");
  } else if (buttons & BUTTON_DOWN) {
    lcd.print("DOWN");
  } else if (buttons & BUTTON_LEFT) {
    lcd.print("LEFT");
  } else if (buttons & BUTTON_UP) {
    lcd.print("UP");
  } else if (buttons & BUTTON_SELECT) {
    lcd.print("SELECT");
  } else if (!buttons) {
    clearSpaces(6);
  }

}

void clearSpaces(int x) {
  lcd.setCursor(0,1);

  for (int i = 0; i < x + 1; i++) {
    lcd.write(' ');
  }

  lcd.setCursor(0, 1);
}

void stringFunction() {
  String my_str = "Hello World";
  char letter = my_str[0];  // alternatively my_str.charAt(n)
  Serial.println(letter);
  //String lower_my_str = my_str.toLowerCase(); <-- Does not return anything,mutates the original string
  my_str.toLowerCase();
  Serial.println(my_str);
}

void printing() {
  // Numbers get casted to string automatically
  // Conversions for nums can be done natively in print aswell (also using String() same thing could be done)
  Serial.println(202);
  Serial.println(3.14159);
  Serial.println('M');  // '' For char, "" for string
  Serial.println("Hello COA202!");
  Serial.println(202, BIN);
  Serial.println(202, OCT);
  Serial.println(202, DEC);
  Serial.println(202, HEX);
  Serial.println(3.14159, 0);  // to x decimal places
  Serial.println(3.14159, 2);
  Serial.println(3.14159, 4);
  // String literals take up alot of space in SRAM, therefore wrap string literal in F wrapper to ensure saved in Flash Memory
  Serial.println(F("Hello COA202!"));  // Passes Flash-Memory based strings
  Serial.print("Hello ");
  Serial.print("\t");
  Serial.print("COA202");
  Serial.println(" !");
  Serial.print("\n");
  Serial.print("Hello part B");
  Serial.print("\r");
  Serial.println("Hello Part A");
  for (int i = 0; i < 10; i++) {
    Serial.println(i);
    delay(10);  // in ms [100ms => 1s]
  }
}
