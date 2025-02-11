/*
roughly 45 BYTES FOR A NODE AND DATA.
*/
//cw_1.ino -> F422436.ino
// 80 Character guideline -                                                    ||
// _ -> Variables, camelcase for functions

/* Package/Module Import & Declarations */
#include <TimerOne.h>
#include <MemoryFree.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

/* Global Variable Declaration (Initialisation) */
unsigned long  my_time = 0;
int state;
int substate;                                   // use dependent on extensions implemented.
bool timer_started;
int current_employee_count = 0;
bool scroll = false;                            // is scroll enabled.
int current_scroll_offset = 0;
const unsigned long scroll_interval = 1000000;  // 0.5s per character therefore, 2 char per second.
char cjt_buffer[8];                             // Buffer for the job title to be displayed on screen.

struct employee {         // 38 bytes
  int  job_grade;         // 2 bytes
  bool on_pension;        // 1 byte
  char salary[9];         // 9 bytes
  char id[8];             // 8 bytes
  char job_title[18];     // 18 bytes
};

// Double Linked list
struct employee_node {              // 6 bytes
  employee* employee_data;          // 2 bytes        
  employee_node* next_employee;     // 2 bytes
  employee_node* previous_employee; // 2 bytes
};
employee_node* root = nullptr;        // Always points to root of LL.
employee_node* current_node = nullptr;// Current node for LL operations.
employee_node* active_node = nullptr; // Currently viewed node on screen.


// UDCHARS -> Declaration
byte custom_up_arrow[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};
byte custom_down_arrow[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};

//enum {main_substate, scroll_substate}

/* Macro & Debug Declarations */
#define sync_msg "R"
#define MAIN_PHASE 0
#define STUDENT_ID 1

#define YELLOW 3
#define WHITE 7
#define PURPLE 5
#define RED 1
#define GREEN 2

#define ADD "ADD"
#define PST "PST"
#define GRD "GRD"
#define SAL "SAL"
#define CJT "CJT"
#define DEL "DEL"

#define PEN "PEN"
#define NPEN "NPEN"

#define MAX_EMPLOYEES 20  // Safe num would require loosely around 1000 bytes of memory free.

//#define DEBUG
//#define TIME_DEBUG
//#define DEBUG_LVL 2
// For constant messages to ensure right calls are firing.
#define const_debug(x) Serial.print(F("DEBUG: ")); Serial.println(x);

//#ifdef DEBUG_LVL >= 1
#ifdef DEBUG
#define debug_msg(x) Serial.print(F("DEBUG: ")); Serial.print(x);
#define debug_msg_ln(x) Serial.print(F("DEBUG: ")); Serial.println(x);
#else
#define debug_msg(x) do {} while(0)
#define debug_msg_ln(x) do {} while(0)
#endif
//#ifdef DEBUG_LVL >= 2
#ifdef TIME_DEBUG
#define time_debug_msg(x) debug_msg(x); // on same line due to spam
#else
#define time_debug_msg(x) do {} while(0)
#endif

// Testing Program Response Messages.
#define DONE Serial.println(F("DONE!"));
#define error(x) Serial.print(F("ERROR: ")); Serial.println(x);
#define emp_id_error error(F("Invalid employee ID"))
#define length_error error(F("Invalid data (length)"))
#define invalid_data error(F("Invalid data format"))

//...

/* Function Declaration */
bool checkTimeChange(int interval); // Checks if interval (ms) time has passed.
String gainInput();
void stringcpy(char destination[], char str[], int charLimit); // unused
void displayEmployee(employee* emp);
void clearScreen(int startX);
void handleCommand();
employee_node* createNode(employee* emp);
void addNode(employee_node* node);
employee_node* findEmployee(char employee_id[]);
bool deleteNode(char employee_id[]);
bool updateNode(char old_employee_id[], employee* new_emp); //unused
bool dataValidation(String* inp);   //partially used
void scrollHandler(void);
void printJobTitle(void);



/* MAIN PROGRAM */
void setup() {
  // Board Serial Communication Setup
  Serial.begin(9600);

  // Timer Setup
  Timer1.initialize(scroll_interval);
  Timer1.attachInterrupt(scrollHandler);

  // LCD Screen Initialisation
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  // UDCHARS Initialisation
  lcd.createChar(0, custom_up_arrow);
  lcd.createChar(1, custom_down_arrow);

  /* Synchronisation Phase - Print R until Receive BEGIN string. */
  lcd.setBacklight(YELLOW); // 3 = Yellow   
  // Blocking due to synchronisation in setup.
  while (true) {
    if (checkTimeChange(2000)) {
      
      String return_str = gainInput();
      debug_msg(return_str);
      // Check for newline or carriage return.
      if (return_str.endsWith("\n") || return_str.endsWith("\r")) {
        error(F("ERROR: Message should not contain newline or carriage return character."));
      } else if (return_str == "BEGIN"){
        // Set state and setup for main loop.
        Serial.print(F("BASIC\n"));
        state = MAIN_PHASE;
        lcd.setBacklight(WHITE);
        break;
      }
      
      Serial.print(sync_msg);
    }
    //debug_msg(gainInput());
  }

  debug_msg_ln(sizeof(employee)); //38 bytes 
  debug_msg_ln(sizeof(employee_node)); //6bytes
}

void loop() {
  uint8_t buttons = lcd.readButtons();
  debug_msg(buttons);

  switch (state) {

    case MAIN_PHASE:
    {
      // Display main menu (test arrows -> dependent on if active_node pointers)
      // up_arrow
      if (active_node != NULL && active_node->previous_employee != NULL) {
        lcd.setCursor(0,0);
        lcd.write((uint8_t)0);
      } else {
        lcd.setCursor(0,0);
        lcd.print(" ");
      } 
      // down_arrow
      if (active_node != NULL && active_node->next_employee != NULL) {
        lcd.setCursor(0,1);
        lcd.write((uint8_t)1);
      } else {
        lcd.setCursor(0,1);
        lcd.print(" ");
      }
      
      debug_msg_ln(timer_started);
      time_debug_msg(my_time);
      time_debug_msg(active_node->employee_data->id);

      // if select is held for 2 seconds, enter student_id phase.
      if (buttons & BUTTON_SELECT) {
        if (!timer_started) {
          timer_started = true;
          my_time = millis();
        }else {
          if (checkTimeChange(2000)) {
            lcd.clear();
            state = STUDENT_ID;
            scroll = false;
            break; // Prevent unnecessary further processing.
          }
        }
      
      /* Haneedles scrolling through main menu & scrolling between nodes.*/
      } else if (buttons & BUTTON_UP) {
        if (active_node != NULL && active_node->previous_employee != NULL) {
          active_node = active_node->previous_employee;
          displayEmployee(active_node->employee_data);
        }
      } else if (buttons & BUTTON_DOWN) {
        if (active_node != NULL && active_node->next_employee != NULL) {
          active_node = active_node->next_employee;
          displayEmployee(active_node->employee_data);
        }
      }

      // Handles resetting timer if button select is released too early.
      if (!(buttons & BUTTON_SELECT)) {
        timer_started = false;
      }

      // Displays the current job title buffer if it is scrolling.
      if (scroll) {
        printJobTitle();
      }

      break;
    }
    case STUDENT_ID:
    {
      // Set backlight purple and show student ID.
      lcd.setBacklight(PURPLE);
      lcd.setCursor(0, 0);
      lcd.print(F("F422436"));
      // Show free memory
      lcd.setCursor(0, 1);
      lcd.print(freeMemory());
      debug_msg_ln(freeMemory());

      // Handle exiting student_id.
      if (!(buttons && BUTTON_SELECT)) {
        state = MAIN_PHASE;
        lcd.setBacklight(WHITE);
        timer_started = false;
        lcd.clear();
        if (active_node != NULL) {
          displayEmployee(active_node->employee_data);
        }
      }
      break;
    }
  }

  handleCommand();
}



/* Functions [Declared in function declaration ^^] */
bool checkTimeChange(int interval) {
  /* Checks if a given interval has passed since last call. */
  time_debug_msg(millis());
  unsigned long current_time = millis();
  if ((current_time - my_time) >= interval) {
    my_time = current_time;
    return true;
  }
  return false;
}

String gainInput() {
  /* Check if there is anything available in the serial input buffer and returns as string. */
  if (Serial.available() > 0) {
    String str = Serial.readString();
    return str;
  }
  return "";
}

// UNUSED
void stringcpy(char destination[], char str[], int charLimit) {
  // Copies the character from the input string to the destination string character at a time.
  // Until end of string or the max characters has been reached. 
  // Assumes char limit is desired characters to copy not size of destination.
  for (int i = 0; str[i] != '\0' && i < charLimit; i++) {
    destination[i] = str[i];
  }

  destination[charLimit + 1] = '\0'; // Adds terminating character.
}

void displayEmployee(employee* emp) {
  /* Displays an employees details */
  // Clears screen starting at x = 1.
  clearScreen(1);

  // if nothing got passed but function still called..
  if (emp == NULL) {
    lcd.setBacklight(WHITE);
    scroll = false;
    const_debug(F("Fatal Error?"))
    return;
  }
  
  // Displays employee's data.
  // Assumes arrows have been drawn & (argument) data is correct. 
  
  // Display employee data.
  lcd.setCursor(1,0);
  lcd.print(emp->job_grade);

  lcd.setCursor(3,0);

  if (emp->on_pension == true) {
    lcd.setBacklight(GREEN);
    lcd.print(PEN);
  } else {
    lcd.setBacklight(RED);
    lcd.print(NPEN);
  }

  lcd.setCursor(8,0);
  lcd.print(emp->salary);

  lcd.setCursor(1,1);
  lcd.print(emp->id);

  lcd.setCursor(9,1);
  lcd.print(emp->job_title);
  // Checks if scrolling needs to be turned on.
  if (strlen(emp->job_title) > 7) {
    scroll = true;
    current_scroll_offset = 0;
  } else {
    scroll = false;
  }
  
  debug_msg_ln(emp->id);
  debug_msg_ln(emp->job_grade);
  debug_msg_ln(emp->on_pension);
  debug_msg_ln(emp->salary);
  debug_msg_ln(emp->id);
  debug_msg_ln(emp->job_title);
}

void clearScreen(int startX) {
  /* Clears screen starting from a given X coordinate. (Clears both top & bottom). */
  for (int i = startX; i < 16; i++) {
    lcd.setCursor(i,0);
    lcd.print(" ");
    lcd.setCursor(i,1);
    lcd.print(" ");
  }
}

// horrific function.... but it works... (i think)
void handleCommand() {
  /* Checks for input & then checks for a valid command before firing the required function. */
  
  String inp = gainInput();

  if (inp != "") {
    if (inp.length() >= 3) {
      String cmd = inp;
      cmd = cmd.substring(0,3);
      debug_msg_ln(inp);
      debug_msg_ln(cmd);

      // Remove command from input string.
      inp = inp.substring(3);
      int l = inp.length();
      debug_msg_ln(l);
      debug_msg_ln(inp);

      /* Common data validation, would have been in dataValidation, abstracted here during static review */
      // Some commands have it some don't. During static review, aim to remove redundant calls of this.
      // one call had charAt maybe bug with indexing consistency .charat 
      // Not XOR
      if (!(!(inp.charAt(l-1) != '\n') != !(inp.charAt(l-1) != '\r'))) {
        error(F("Invalid command - no newline character. terminated by \\n and \\r is not allowed"))
        return;
      }

      // To remove inconsistent use of dataValidation during static review 
      // Checks valid id format.
      // exception to this rule is delete
      if (!(cmd == DEL)) {
        if (!(l > 8 && inp[0] == '-' && inp[8] == '-')) {
          invalid_data;
          return;
        }
      }

      // Check ID Characters.
      for (int i = 1; i < 8; i++) {
        if (!isDigit(inp[i])) {
          emp_id_error;
          return;
        }
      }
        

      if (cmd == ADD) {

        // Safety 
        debug_msg_ln(current_employee_count);
        if (current_employee_count >= MAX_EMPLOYEES) {
          error(F("Max Employee Count Reached (20). -> Try Deleting user's first."));
          return;
        }

        // Node creation only requires 44 bytes of memory. However keep a buffer of 100 bytes due to fragmentation or local variables & stack calls.
        debug_msg_ln(freeMemory());
        if (freeMemory() < 100) {
          error(F("Try Deleting user's first."));
          return;
        }

        /* Verify Data */
        // Ensure job_title is between 3-17 (fixed length of other fields)
        if (l < 15 || l > 29) // include null character/newlinecharacter
        {
          error(F("Invalid Job Title (Length Between 3-17)."));
          return;
        }

        //Serial.print("DEBUG:");Serial.println(inp[l]);

        // Ensure all data and formats are present
        if (inp[10] != '-') {
          error(F("Invalid Format for Data"));
          return;
        }
        // Check employee_id only contains numbers
        // moved to top.

        // Check job_grade is a number
        if (!isDigit(inp[9])) {
          error(F("Job Grade is not a number."));
          return;
        }

        char x;
        for (int i = 11; i < l -1; i++) {
          x = inp[i];
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
        // Check its unique node in add
        if (findEmployee(inp.substring(1,8).c_str()) != NULL) {
          error(F("Employee ID already exists."));
          return;
        }

        // Allocate memory for a new empoyee.
        employee* new_emp = (employee*)malloc(sizeof(employee));
        if (new_emp == NULL) {
          error(F("Memory Allocation Failed or Stopped -> Try Deleting user's first."));
          return;
        }

        // Add all the data.
        strcpy(new_emp->id, inp.substring(1,8).c_str());
        new_emp->job_grade = inp[9] - '0';
        strcpy(new_emp->job_title, inp.substring(11,l-1).c_str());
        strcpy(new_emp->salary, "00000.00");
        new_emp->on_pension = true;

        // add node to LL, update emp count and return done.
        // include null check
        employee_node* new_node = createNode(new_emp);
        if (new_node != NULL){
          addNode(new_node);
          current_employee_count++;
          DONE;
        }
      } else if (cmd == PST) {   
        /* Verify Data */
        // Check length. Length can only be 13 or 14.
        if (!(l == 13 || l == 14)) {
          length_error;
          return;
        }
        //Check Format. -1234567-PEN\n
        // moved to before command fires

        // Check ID Characters.
        // moved to top.
        
        // Check Valid Status
        String new_pension = inp.substring(9,l-1);
        debug_msg_ln(new_pension);
        bool on_pen;
        if (new_pension == PEN) {
          on_pen = true;
        } else if (new_pension == NPEN) {
          on_pen = false;
        } else {
          error(F("Invalid Pension Status."))
          return;
        }


        /* Check if Employee Exists/pull data */
        employee_node* emp = findEmployee(inp.substring(1,8).c_str());
        if (emp == NULL) {
          error(F("Invalid employee ID."))
          return;
        }

        // Check the salary status.
        if (strcmp(emp->employee_data->salary, "00000.00\0") == 0) {
          error(F("Employee's salary is still £00000.00. Change it to change pension status."))
          return;
        }

        // Check current status is not the same as new status.
        if (emp->employee_data->on_pension == on_pen) {
          error(F("Pension Status is already that"));
          return;
        }

        // Create new employee. -> sets it due to being a pointer
        emp->employee_data->on_pension = on_pen;

        // If the currently shown node has been updated, display the new data.
        if (active_node == emp) {
          displayEmployee(emp->employee_data);
        }

        DONE;
      } else if (cmd == GRD) {
        // length check.
        if (l != 11) {
          length_error;
          return;
        }
        // moved format for newline 
        // no need for dataValidation, moved to before therefore, redundant call.

        // check job_grade is number
        if ((!isDigit(inp[9])) || inp[9] == '0') {
          error(F("Job Grade is not a valid number (1-9)"))
        }
        // pull employee
        employee_node* emp = findEmployee(inp.substring(1,8).c_str());
        if (emp == NULL) {
          error(F("Invalid employee ID."))
          return;
        }

        // throw error if original higher or same as new. 
        if (emp->employee_data->job_grade >= (inp[9] - '0')) {
          error(F("Job Grade is not higher then the one before"))
          return;
        }

        // update 
        emp->employee_data->job_grade = inp[9] - '0';

        DONE;
      } else if (cmd == SAL) {
        // allow for only one extra character for rounding 
        // Check length.
        if (l < 11 || l > 19) {
          const_debug(F("Salary is either too large or to too many decimal points. Try reducing the decimal points [it will get rounded to 2 anyway] or the size of the salary."))
          length_error;
          return;
        }
        // moved newline check to start
        // removed datavalidation due to checks before

        // Check salary is a number but is allowed to contain 1 decimal point.
        // Cannot only contain a decimal point.
        // 0
        // 100
        // 100.
        // 100.011
        // .011
        // 99999.999

        // Edge case ->"."
        if (l == 10 && inp[9] == '.') {
          error(F("Salary cannot only contain a decimal point."))
          return;
        }

        // Check valid float.
        bool decimal_point = false; // 1 byte
        for (int i = 9; i < l -1; i++) {
          if (inp[i] == '.') {
            if (decimal_point) {
              error(F("Salary contains more than one decimal point."))
              return;
            }
            decimal_point = true;
          } else if (!isDigit(inp[i])) {
            error(F("Salary contains invalid character."))
            return;
          }
        }
        debug_msg_ln(decimal_point);

        // Check if valid user (and pull data).
        employee_node* emp = findEmployee(inp.substring(1,8).c_str());
        if (emp == NULL) {
          error(F("Invalid employee ID."))
          return;
        }

        String sal = inp.substring(9, l - 1); // Extract salary string
        float new_sal = sal.toFloat(); // Convert to float
        debug_msg_ln(sal);
        debug_msg_ln(new_sal);

        // Check valid range of salary.
        if (new_sal < 0.00 || new_sal > 99999.99) {
          error(F("Salary is out of range."));
          return;
        }

        // Convert to 2 decimal places.
        new_sal = round(new_sal * 100) / 100.0; // Round to 2 decimal places
        debug_msg_ln(new_sal);

        // Convert to specific format needed XXXXX.YY (Padded with 0's)
        char sal_str[9]; // 9 bytes
        dtostrf(new_sal, 8, 2, sal_str);

        // Convert spaces to 0's for padding.
        for (int i = 0; i < 8; i++) {
          if (sal_str[i] == ' ') {
            sal_str[i] = '0';
          }
        }
        sal_str[8] = '\0'; // Ensure null-terminated string
        debug_msg_ln(sal_str);

        // Update salary.
        strcpy(emp->employee_data->salary, sal_str);

        // If the currently shown nde has been updated, display the new data.
        if (active_node == emp) {
          displayEmployee(emp->employee_data);
        }

        DONE;
      } else if (cmd == CJT) {
        // Length Check -1234567-12345678901234567\n
        if (l < 13 || l > 27) {
          length_error;
          return;
        }

        // moved format validation to top
        // Check Characters. -> moved to top

        // Check valid job title.
        char x;
        for (int i = 9; i < l -1; i++) {
          x = inp[i];
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
        
        // Check Valid User.
        employee_node* emp = findEmployee(inp.substring(1,8).c_str());
        if (emp == NULL) {
          error(F("Invalid employee ID."))
          return;
        }

        // Update job title.
        strcpy(emp->employee_data->job_title, inp.substring(9,l-1).c_str());

        // If the currently shown nde has been updated, display the new data.
        if (active_node == emp) {
          displayEmployee(emp->employee_data);
        }

        DONE;
      } else if (cmd == DEL) {
        // Check common data validation for input [and given scenario].
        // Check length.
        if (l != 9) {
          length_error;
          return;
        }

        // Check Format.
        if (inp[0] != '-') {
          invalid_data;
          return;
        }
        // Check emp ID. -> moved to top
          // Only employee ID is needed so extract froom input.
          //inp = inp.substring(1,8).c_str();

          // No need to check if valid user as handled in delete user -> reflection overdevloped DS handlers functionality before BASIC commands therefore unnecessary oversight in places (ie unnecessary extra work -> left in because no need to remove as it works)
          // Inconsistent DS handler functions ie here for delete has built in error checking and bool returns ie diff to others.
          // Delete user
        if (deleteNode(inp.substring(1,8).c_str())) {
          current_employee_count--;
          DONE;
        }
      } else {
        // First 3 letters are non-valid.
        error(F("Not Recognized Command"));
      } 
    } else {
      // Length does not fit a command.
      error(F("Non-Valid Command"));
    }
  }
}


employee_node* createNode(employee* emp) {
  /* Creates a new node using employee data (initialises with empty pointers) and 
     returns the pointer to the new node*/
  
  employee_node* node = (employee_node*)malloc(sizeof(employee_node));
  if (node == NULL) {
    error(F("Memory Allocation Failed or stopped -> Try Deleting user's first."));
    free(emp);
    return NULL;
  }

  
  // Set relevant fields for the node 
  node->employee_data = emp;
  node->next_employee = NULL;
  node->previous_employee = NULL;


  return node;
}

void addNode(employee_node* node) {
  /*  Receives a pointer to a node
      If the list is empty, "creates new list"
      Else will find where in the list to place node.
      else append node to list. */

  // Check empty list.
  if (root == NULL) {
    // Create new root for list.
    root = node;
    current_node = root;
    active_node = root;

    // Display data (to show data has been added).
    if (state == MAIN_PHASE) {
      displayEmployee(active_node->employee_data);
    }
    return;
  }

  current_node = root;

  // Check if the new node should be inserted before the current root.
  if (strcmp(node->employee_data->id, root->employee_data->id) < 0) {
    node->next_employee = root;
    root->previous_employee = node;
    root = node;
    
    return;
  }

  // Traverse the list to find the correct insertion point or end of list.
  while (current_node->next_employee != NULL && strcmp(current_node->next_employee->employee_data->id, node->employee_data->id) < 0) {
    current_node = current_node->next_employee;
  }

  // Insert the node in the correct position.
  node->next_employee = current_node->next_employee;
  node->previous_employee = current_node;

  // If it is not the end of the list, sort the pointers.
  if (current_node->next_employee != NULL) {
    current_node->next_employee->previous_employee = node;
  }

  // Final pointer for inserting node into list.
  current_node->next_employee = node;
}

employee_node* findEmployee(char employee_id[]) {
  /* Finds an employee node given an employee id.*/
  current_node = root; 

  // Check list is not empty
  if (current_node == NULL) {
    const_debug(F("List is empty"));
    return NULL;
  }

  // Traverses LL to find the specific node.
  while (current_node != NULL && strcmp(current_node->employee_data->id, employee_id) != 0) {
    current_node = current_node->next_employee;
  }

  // Checks that node has been found not end of list.
  if (current_node != NULL && strcmp(current_node->employee_data->id, employee_id) == 0) {
    return current_node;
  // if end of list or other, node cannot be found.
  } else {
    const_debug(F("Node cannot be found.")); // As debug as could be positive or negative.
    return NULL;
  }
}

bool deleteNode(char employee_id[]) {
  /* Handles pointer assignment and freeing of nodes in doubly linked list */
  // https://www.geeksforgeeks.org/delete-a-node-in-a-doubly-linked-list/ Last Accessed: 4/12/24 02:47.
  current_node = root;

  // Check non-empty list.
  if (current_node == NULL) {
    error(F("No node to delete."));
    return false;
  }

  // Check first node.
  if (strcmp(current_node->employee_data->id, employee_id) == 0) {
    employee_node* temp = root;
    root = current_node->next_employee;

    // If the next node is nothing then list is empty.
    if (root != NULL) {
      root->previous_employee = NULL;
    }

    // Check if active node == temp, display root
    if (active_node == temp) {
      if (state == MAIN_PHASE) {
        if (root != NULL) {
          displayEmployee(root->employee_data);
        } else {
          displayEmployee(NULL);
        }
      }
      active_node = root;
    }
    

    free(temp->employee_data);
    free(temp);
    return true;
  }

  // Check rest of linked ll -> find node to be deleted
  while (current_node != NULL && strcmp(current_node->employee_data->id, employee_id) != 0) {
    current_node = current_node->next_employee;
  }

  // end of list found.
  if (current_node == NULL) {
    error(F("Node not found"));
    return false;

  // if wanted node is found
  } else if (strcmp(current_node->employee_data->id, employee_id) == 0) {
    // Pointer assignments.
    employee_node* temp = current_node;
    current_node->previous_employee->next_employee = temp->next_employee;
    if (temp->next_employee != NULL) {
      temp->next_employee->previous_employee = temp->previous_employee; 
    }

    // if the active node  is deleted, change active node and display the previous node so system doesn't break next time button pressed.
    if (active_node == temp) {
      if (state == MAIN_PHASE) {
        displayEmployee(temp->previous_employee->employee_data);
      }
      active_node = temp->previous_employee;
    }
    

    // free memory.
    free(temp->employee_data);
    free(temp);
    return true;
  }
  

  error(F("Unknown Error"))
  return false;
}

// May not be needed
bool updateNode(char old_employee_id[], employee* new_emp) {
  // Find old node.
  employee_node* old_node = findEmployee(old_employee_id);

  if (old_node != NULL) {
    delete old_node->employee_data;
    old_node->employee_data = new_emp;
    return true;
  } else {
    return false;
  }
}

// will be unused
bool dataValidation(String* inp) {
  // Inclusive of size ( bounds)

  /* Verify Data */
  // Check Format.
  if (inp->charAt(0) != '-' || inp->charAt(8) != '-') {
    invalid_data;
    return false;
  }

  // Check Characters.
  for (int i = 1; i < 8; i++) {
    if (!isDigit(inp->charAt(i))) {
      emp_id_error;
      return false;
    }
  }

  return true;
}

//Unnecessary Stack Call? -> just leave redundant call in for now.
/*
bool validJobTitle(String* inp, int title_start, int title_end, ) {
  for (int i = 11; i < l -1; i++) {
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

}
*/

void scrollHandler(void) {
  /* Handles the scrolling of the job title (setting buffer). */
  debug_msg_ln(scroll);
  if (scroll) {
    // Check if the current node has a job title. In safe dereferencing way.
    if (active_node != NULL && active_node->employee_data != NULL && active_node->employee_data->job_title != NULL) { 
      int l = strlen(active_node->employee_data->job_title); // 2 bytes -> freed (popped) from stack after function finishes.
      debug_msg_ln(l);
      debug_msg_ln(current_scroll_offset);
      // Check if the job title is long enough for scrolling.
      if (l > 7) {
        // Check if the current scroll offset is at the end of the job title.
        if (current_scroll_offset >= l - 7) {
          // Reset the scroll offset.
          current_scroll_offset = 0;
        } else {
          // Increment the scroll offset.
          current_scroll_offset += 2;
        }
        
        // Set the cjt_buffer 
        for (int i = 0; i < 7; i++) {
          // As switching 2 chars at a time if the edge case of the last character never being displayed, this adds a space to ensure the last character is seen before the reset.
          // Also ensures that if an error occurs a space is displayed.
          if (i + current_scroll_offset >= l) {
            cjt_buffer[i] = ' ';
          } else {
            cjt_buffer[i] = active_node->employee_data->job_title[i + current_scroll_offset];
          }
        }
        cjt_buffer[7] = '\0';
        debug_msg_ln(cjt_buffer);
      }

    }
  }
}

void printJobTitle(void) {
  /* Prints the job title to the screen (if scroll is active).*/
  lcd.setCursor(9,1);
  lcd.print(cjt_buffer);
}