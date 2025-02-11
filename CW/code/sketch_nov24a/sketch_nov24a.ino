#define ADD "ADD"
String gainInput() {
  // Check if there is anything available in the serial input buffer and returns as string.
  if (Serial.available() > 0) {
    String str = Serial.readString();
    return str;
  }
  return "";
}

void handleCommand() {
  // Checks for input & then checks for a valid command before firing the required function.
  
  String inp = gainInput();
  if (inp.length() != 0) {
    Serial.println(inp.length());
  }


  if (inp != "") {
    if (inp.length() >= 3) {
      String cmd = inp;
      cmd = cmd.substring(0,3);
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
  Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println("Hello World.");
}

void loop() {
  // put your main code here, to run repeatedly:
  handleCommand();
}
