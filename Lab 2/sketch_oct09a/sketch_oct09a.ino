// Imports
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Declarations
void floatOperations();
void incrementalOperations();
void stringOperations();
void arrays();
void dataTypes();

int bg_colour = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lcd.begin(16, 2);     // lcd.begin(cols,rows); Not 0 indexed
  lcd.setBacklight(1);  // 0->7 for colours
  lcd.setCursor(0, 0);  // lcd.setCursor(x, y);

  arrays();
  //dataTypes();
}

void loop() {
  // put your main code here, to run repeatedly:
  bg_colour ++;
  bg_colour %= 7;
  lcd.print(bg_colour, DEC);
  lcd.setCursor(0,0);
  //lcd.setBacklight(bg_colour);
}

void floatOperations() {
  int c = 7/4;
  Serial.println(c);

  float d = 7/4;
  Serial.println(d);

  float a = 7.0/4;
  Serial.println(a);
  float b = 7.0/4.0;
  Serial.println(b);
  float e = (float)(7/4);
  Serial.println(e);
  float f = (float)7/4;
  Serial.println(f);
  int g = 2/3.0;
  Serial.println(g);
}

void incrementalOperations() {
  int y = 0;
  int x = 5;
  Serial.println(x);
  Serial.println(y);
  x += 3;
  Serial.println(x);
  x -= 4;
  Serial.println(x);
  x *= 20;
  Serial.println(x);
  x /= 4;
  Serial.println(x);
  y = ++x;
  Serial.println(y);
  y = x--;
  Serial.println(y);

}

void arrays() {
  int numbers[5]; // numbers is an array of 5 integers
  int i = 6;
  numbers[2] = i;
  numbers[4] = 3;
  numbers[0] = numbers[2] + numbers[4];
  numbers[1] = i - numbers [4];
  numbers[3] = numbers[4] - numbers[1];
  for(int n=0; n > 5; n++){
    Serial.println(numbers[n]);
  }
  int numbers1[] = {1, 2, 3}; // numbers is an array of 3 integers
  int n = 3;
  numbers1[n] = 0; // assign the value 0 to the 3rd element of numbers
  Serial.println(numbers[2]); //Print the 3rd element

  int nums[] = {1,2,3,4,5,6,7,8,9,10};
  for (int i = 0; i < 10; i++) {
    Serial.print(String(nums[i]) + ", ");
  }
  Serial.println();
  nums[2] = 999;
  nums[4] = 777;
  for (int i = 0; i < 10; i++) {
    Serial.print(String(nums[i]) + ", ");
  }
  Serial.println();

  int lessElements[2];
  // 5. -> very unsafe as you are changing data you shouldnt be able to access could cause many problems.
  lessElements[5] = 10;
  Serial.println(":" + lessElements[5]);

  //5. 
  //6. Wasted space
  // 7. Jus accesses the memory not in the array (could corrupt the program).
  

  
}

void stringOperations(){
  String my_str = "This is my string.";
  Serial.println(my_str);

  my_str.toUpperCase();
  Serial.println(my_str);

  my_str = "My new string.";
  Serial.println(my_str);

  my_str.replace("string","Arduino sketch");
  Serial.println(my_str);

  Serial.print("String length is: ");
  Serial.println(my_str.length());

  Serial.println(my_str[0]);
  my_str.toLowerCase();
  Serial.println(my_str);
}

void dataTypes() {
  struct car {
    String brand;
    String model;
    int year;
    String colour;
  };

  car car_1;
  car_1.brand = "Datsun";
  car_1.model = "240Z";
  car_1.year = 1973;
  car_1.colour = "Red";
  car car_2;
  car_2.brand = "DMC";
  car_2.model = "DeLorean";
  car_2.year = 1983;
  Serial.println(String("The first car is a ") + car_1.year + " " + car_1.brand + " " + car_1.model + " Colour: " + car_1.colour);

  struct student {
    String name;
    int mark;
  };

  student students[10];

  students[9].name = "Jon";

  Serial.println(students[9].name);

  enum Level {Beginner, Intermediate, Advanced};
  Level myLevel = Intermediate;
  switch (myLevel) {
    case 0:
      Serial.println("Beginner Level");
      break;
    case 1:
      Serial.println("Intermediate level");
      break;
    case 2:
      Serial.println("Advanced level");
      break;
  }

  enum color { red, green, blue = 15, yellow };
  color col = red;
  int n = yellow;
  Serial.println(n);

  enum Letters { a, b = 21, c = 15, d, e = 0, f, g = a + c };
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  Serial.println(d);
  Serial.println(e);
  Serial.println(f);
  Serial.println(g);

  union cars {
    float price;
    int year;
  };
  cars car1;
  car1.year = 1973;
  car1.price = 18950.99;
  Serial.println(car1.price);
  Serial.println(car1.year); // why is this "wrong"?

}
