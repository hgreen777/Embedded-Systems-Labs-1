//Lab_3.ino
// Research function in 2.1
// Fix in 2.2
// 3.1 Fix adjustments for the actual lab question.

#define pi 3.14159

void two_one();
void two_two();

void three_one();
void three_two();

void four(); //4.3-5

void five(); //5.6 5.7, 5.8

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");

  //two_one();
  //two_two();
  three_one();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void two_one() {
  int int1; int int2;
  bool watch_movie = false;
  int age = 20;

  if (age > 18) {
    watch_movie = true;
  }

  Serial.println(watch_movie);
  Serial.print("Enter 2 Numbers");

  // Ensure no line endings.
  while (Serial.available() == 0) {}
  int1 = Serial.parseInt(); // No arguents?
  while (Serial.available() == 0) {}
  int2 = Serial.parseInt();
  Serial.println();
  if (int1 > int2) {
    Serial.println("First!");
  } else if (int2 > int1) {
    Serial.println("Second!");
  } else {
    Serial.println("Equal!");
  }

  int time = 20;

  if (time < 12) {
    Serial.println("Good Morning.");
  } else if (time < 17) {
    Serial.println("Good Afternoon.");
  } else {
    Serial.println("Good Evening.");
  }

}

void two_two() {
  int day = 6;
  switch (day) {
    case 6:
      Serial.println("Today is Saturday");
      break;
    case 7:
      Serial.println("Today is Sunday");
      break;
    default:
      Serial.println("Looking forward to the Weekend");
  }

  enum months {January = 1, Febuary, March, April, May, June, July, August, September, October, November, December};
  //String month;
  while (Serial.available() == 0) {}
  //int month = Serial.readString();
  int month = January;

  //Fix...
  switch (month) {
    case (3, 4, 5):
      Serial.println("Spring");
      break;
    case (12, 1, 2):
      Serial.println("Winter");
      break;
    case (6, 7, 8):
      Serial.println("Summer");
      break;
    case (9, 10, 11):
      Serial.println("Autumn");
      break;
    default:
      Serial.println("Error");
      break;
  }


}

void three_one () {
  for (int i = 1; i <= 10; i++) {
    Serial.println(i);
  }

  int numbers[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; // Will accept implicit and explicit count.
  for (int i = 0; i < 10; i++) {
    Serial.println(numbers[i]);
  }

  for (int i = 9; i>=0; i--) {
    Serial.println(numbers[i]);
  }

  for (int x = 50; x < 100; x++) {
    Serial.print(x);
    Serial.print(" ");
  }
  Serial.println();
  for(int i = 2; i <80; i *= 2) {
    Serial.println(i);
  }

  for (int i = 1; i <= 10; i++) {
    if (i >= 4 and i <= 7) {
      continue;
    }
    Serial.print(i);
    Serial.print(" ");
  }
  Serial.println();

  /* Make necessary adjustments for the exercises
  int input[5];
  for (int i = 0; i < 5; i++) {
    Serial.println("Please enter a number");
    while (Serial.available() == 0) {}
    input[i] = Serial.parseInt();
    if (input[i] < 0) {
      
    }
    
  }
  for (int i = 0; i < 5; i++) {
    Serial.println(input[i]);
  }
  */

  int x = 6, y = 1;
  bool keepRunning = true;
  while(true){
    if(y < x){
      y++;
      Serial.print(y);
    }else{
      break;
    }
  }
  Serial.println("Exit");


}
