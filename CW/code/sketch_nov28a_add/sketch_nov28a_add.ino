#define error(x) Serial.print(F("ERROR: ")); Serial.println(x);
void handleCmd(String inp) {
  if (inp != "") {
    if (inp.length() >= 3) {
      String cmd = inp;
      cmd = cmd.substring(0,3);

      // Remove command from input string.
      inp = inp.substring(3);
      int l = inp.length();

      if (cmd == "ADD") {
        /* Verify Data */
        // Ensure all data and formats are present
        if (inp[0] != '-' || inp[8] != '-' || inp[10] != '-') {
          error(F("Invalid Format for Data"));
          return;
        }
        // Ensure job_title is between 3-17 (fixed length of other fields)
        if (l < 14 || l > 28){
          error(F("Invalid Job Title (Length Between 3-17)."));
          return;
        }
        // Check employee_id only contains numbers
        for (int i = 1; i < 8; i++) {
          if (!isDigit(inp[i])) {
            error(F("Employee ID should only contain numbers."));
            return;
          }
        }
        // Check job_grade is a number
        if (!isDigit(inp[9])) {
          error(F("Job Grade is not a number."));
          return;
        }

        for (int i = 11; i < l; i++) {
          char x = inp[i];
          // Check character is in valid range as defined in spec.
          if (!((x >= 'A' && x <= 'Z') ||
                (x >= 'a' && x <= 'z') ||
                isDigit(x)             ||
                x == '_'               ||
                x == '.'                 )) {

                  error(F("Job Title Contains invalid character."));
                  return;
                }
        }
        
        /* Create & Add Node */
        Serial.println("DONE!");
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  error("hi");



  String inp[] = {"ADD-1234567-1-123","ADD-3356872-4-Guard","ADD-1425466-8-HR_Manager","ADD-23577172-5-Care_Taker","ADD-2357712-Care_Taker","ADD-2357712-5-CT","ADD-2357712-5-","ADD9872334-7-Swim_Instructor","ADD-9872334-7-123456789012345678","ADD-2357712-5-Care@Taker","ADD-9872334-7- Swim_Instructor"};
  // Should be 3 guccis
  // format
  // format
  // Length
  // length
  //format
  //length
  // rest invalid job titles
  for (int i = 0; i < 11; i++) {
    handleCmd(inp[i]);
  }

}

bool test = false;
void loop() {
  if (test == false) {
    int tes = 1;
    test = true;
  } else {
    Serial.print(tes);
  }
  }