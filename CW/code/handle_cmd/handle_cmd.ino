#define ADD "ADD"
String gainInput() {
  // Check if there is anything available in the serial input buffer and returns as string.
  if (Serial.available() > 0) {
    String str = Serial.readString();
    Serial.println(str);
    return str;
  }
  Serial.println("Nothing");
  return "";
}

void handleCommand() {
  // Checks for input & then checks for a valid command before firing the required function.
  
  String inp = gainInput();

  if (inp != "") {
    if (inp.length() > 3) {
      String cmd = inp;
      cmd.substring(0,2);
      Serial.println(inp);
      Serial.println(cmd);

      /*
      switch (cmd) {
        case ADD:
        {
          Serial.println("ADD");
          break;
        }
      }
      */
    }
  }
}

void setup() {
  Serial.println("Hello World.");
}

void loop() {
  Serial.println("Hellow");
  handleCommand();
}