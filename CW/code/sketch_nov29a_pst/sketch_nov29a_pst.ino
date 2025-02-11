void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  String test = "-1234567-7-JOBBIE";
  int l = test.length();
  Serial.println(l); //17
  test = test.substring(11,l);
  Serial.println(test);

}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
length == 11 || 12 characters
Charachter 0 & 8 == '-'
Characters 1->8 == Digits

Find the employee data

if employee_pension == NULL {
  error acc doesn't exist
}

if employee_pension = new_pen {
  error pension not changed
}

if salary = 00000.00 {
  error Salary still - 
}

check valid either pen or npen

create new employee with new pen 


*/
