#include <stdio.h>
#include <string.h>

void one_one();
void one_two();
void one_three();
void one_four();
void one_five();
void one_six();
void one_seven();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");
  //one_one();
  //one_two();
  //one_three();
  //one_four();
  //one_five();
  one_six();
  //one_seven();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void one_one() {
  struct sensorInfo {
    char date[11];
    int indoorTemp;
    int outdoorTemp;
  } morningTemps, *ptr;
  strcpy(morningTemps.date, "07/03/2023");
  morningTemps.indoorTemp = 22;
  morningTemps.outdoorTemp = 8;
  ptr = &morningTemps;
  
  Serial.print("On ");
  Serial.println(ptr->date);
  Serial.print("The indoor temp was: ");
  Serial.println(ptr->indoorTemp);
  Serial.print("The outdoor temp was: ");
  Serial.println(ptr->outdoorTemp);
  strcpy(ptr->date, "10/03/2023");
  ptr->indoorTemp = 25;
  ptr->outdoorTemp = 12;
  Serial.print("On ");
  Serial.println(ptr->date);
  Serial.print("The indoor temp was: ");
  Serial.println(ptr->indoorTemp);
  Serial.print("The outdoor temp was: ");
  Serial.println(ptr->outdoorTemp);

}

void testFunction(int &x, int &y) {
  int value1= x + 5;
  int value2 = x * y;
  x = value1;
  y = value2;
}

void test2Function(int *x,int *y) {
  int value1 = *y;
  int value2 = *x;
  *x = value1;
  *y = value2;
}

void one_two() {
  int a = 5, b = 3;
  testFunction(a, b);
  Serial.print("a = ");
  Serial.println(a);
  Serial.print("b = ");
  Serial.println(b);
}

void one_three() {
  int a = 1, b = 7;
  test2Function(&a,&b);
  Serial.print("a=");
  Serial.println(a);
  Serial.print("b=");
  Serial.println(b);
}

void one_four() {
  int numbers[10];
  int *ptr;
  ptr = &numbers[0];
  *ptr = 1;
  *ptr++;
  *ptr = 2;
  *ptr++;
  *ptr = 3;
  *ptr++;
  *ptr = 4;
  Serial.print(numbers[0]);
  Serial.print(numbers[1]);
  Serial.print(numbers[2]);
  Serial.print(numbers[3]);
  Serial.println();

}

void one_five() {
  int* ptr;
  if (ptr == NULL) {
    Serial.println("Non Valid");
  } else {
    Serial.println("Valid");
  }
}

void one_six() {
  void* unknowndatatypePtr = NULL;
  int *intptr;
  char *cptr;
  int x = 5;
  char c = 'h';
  unknowndatatypePtr = &x;
  intptr = (int*)unknowndatatypePtr;
  *intptr = 10;
  unknowndatatypePtr = &c;
  cptr = (char*) unknowndatatypePtr;
  *cptr = 'c';
  
  Serial.println((unsigned long) unknowndatatypePtr, HEX);
  Serial.println(x); 
  Serial.println(c); 
}

void one_seven() {
  
}
