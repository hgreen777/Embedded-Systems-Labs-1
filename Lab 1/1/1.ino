// Lab1.ino

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include<utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield(); // Create lcd object which has methods to control the display
// lcd.write() prints the variable one character at a time
// lcd.print(var); prints the variable var to the display

void basicInput();
void slightlyBetterInput();
void writingToLCD();
void printing();

char incomingChar = 0;
String incomingString = "";

void setup() {
  Serial.begin(9600); // Define the baud rate for serial communication.

  lcd.begin(16,2); // lcd.begin(cols,rows); Not 0 indexed
  lcd.setBacklight(1); // 0->7 for colours
  lcd.setCursor(0,0); // lcd.setCursor(x, y);

  lcd.println("Hello World"); // Adds characters to the end (probs escape character for newline)
  lcd.setCursor(0,1);
  lcd.print("Hello "); lcd.write("World"); // ??
  
  Serial.println("Hello, World!");

  String my_str = "Hello World";
  char letter = my_str[0]; // alternatively my_str.charAt(n)
  Serial.println(letter);
  //String lower_my_str = my_str.toLowerCase(); <-- Does not return anything,
  mutates the original string
  my_str.toLowerCase();
  Serial.println(my_str);
}

void loop() {
  //basicInput();
  slightlyBetterInput();
}

void basicInput() {
  // Serial.available gets the num of bytes (characters) available for reading from the serial port.
  // [data that has already arrived and stored in the serial recieve buffer (holds 64bytes)]
  if (Serial.available() > 0) { // Only true when data is stored in the serial recieve buffer.
    // Having both uncommented means the first line removes one letter from the buffer and
    // the second line takes the rest of the buffer as a string to print it.
    incomingChar = Serial.read(); // Takes each character from the buffer
    incomingString = Serial.readString(); // Takes whole string from buffer

    Serial.print("String recieved: ");
    Serial.println(incomingString);
    Serial.print("I recieved: ");
    Serial.println(incomingChar);
  }
}

void slightlyBetterInput() {
  Serial.println("Enter data:");
  // Paused execution until data available wheres if statement allows further execution
  while (Serial.available() == 0) {} // Wait for available data

  String inputString = Serial.readString(); // read until timeout [ie end of buffer]
  // Serial.readStringUntil() -> reads till terminating character (char). What is the benefit??
  inputString.trim(); // remove any \r \n whitespace at the end of the string

  if (inputString == "COA202") {
    Serial.println("This is Embedded Systems Programming");
  } else {
    Serial.println("This is something else");
  }
}

void writingToLCD() {

}

void printing() {
  // Numbers get casted to string automatically
  // Conversions for nums can be done natively in print aswell (also using
  String() same thing could be done)
  Serial.println(202);
  Serial.println(3.14159);
  Serial.println('M'); // '' For char, "" for string
  Serial.println("Hello COA202!");
  Serial.println(202, BIN);
  Serial.println(202, OCT);
  Serial.println(202, DEC);
  Serial.println(202, HEX);
  Serial.println(3.14159, 0); // to x decimal places
  Serial.println(3.14159, 2);
  Serial.println(3.14159, 4);
  // String literals take up alot of space in SRAM, therefore wrap string literal in F wrapper to ensure saved in Flash Memory
  Serial.println(F("Hello COA202!")); // Passes Flash-Memory based strings
  Serial.print("Hello ");
  Serial.print("\t");
  Serial.print("COA202");
  Serial.println(" !");
  Serial.print("\n");
  Serial.print("Hello part B");
  Serial.print("\r");
  Serial.println("Hello Part A");
  for (int i=0; i < 10; i++) {
    Serial.println(i);
    delay(10); // in ms [100ms => 1s]
  }
}
