// Taewan Kang (8394837)
// Ashwin Anand (8154187)
// Quang-Tri Do (8248005)
// SEG4145
// Lab 2, GROUP 1
// Lab2.ino
// Contains all code to perform tasks for Lab 2

#include <SoftwareSerial.h>
#include <Servo.h>
 

// Define Constants
#define LEFT_MOTOR      45
#define RIGHT_MOTOR     8
#define LCD_DISPLAY     18
#define LED             13

// Direction constants
#define LEFT_BACKWARD   10
#define LEFT_FORWARD    191.5
#define RIGHT_BACKWARD  191.5
#define RIGHT_FORWARD   100

// Time constants
#define TILE_TIME       1400

int STRAIGHT = 0;
int LEFT = 1;
int RIGHT = 2;

SoftwareSerial LCD(0, LCD_DISPLAY);

boolean startMoving = true;


//setup
//Standard setup for arduino
void setup() {
 // Initialize all pins.
 pinMode(LEFT_MOTOR, OUTPUT);
 pinMode(RIGHT_MOTOR, OUTPUT);
 pinMode(LED, OUTPUT);

 LCD.begin(9600);
 delay(10);
 delay(3000);
 

}

void loop() {
 if (startMoving)
  testPath();
}

//backward
//Moves both wheels backwards
void backward() {
 analogWrite (LEFT_MOTOR, LEFT_BACKWARD);
}

// Takes 2.79s for full rotation
//rotate
//direction: which direction to rotate (LEFT or RIGHT)
//degree: how many degrees (45 or 90)
//Rotates the wheels depending on direction and degree
void rotate(int direction, int degree) {
  if (direction == LEFT) {
    printRotateLeft();
    if (degree == 45) {
      analogWrite (LEFT_MOTOR, LEFT_BACKWARD);
      analogWrite (RIGHT_MOTOR, RIGHT_FORWARD);
      delay(450);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    } else if (degree == 90) {
      analogWrite (LEFT_MOTOR, LEFT_BACKWARD);
      analogWrite (RIGHT_MOTOR, RIGHT_FORWARD);
      delay(925);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    } else if (degree == 135) {
      analogWrite (LEFT_MOTOR, LEFT_BACKWARD);
      analogWrite (RIGHT_MOTOR, RIGHT_FORWARD);
      delay(1150);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    }
  } else if (direction == RIGHT) {
    printRotateRight();
    if (degree == 45) {
      analogWrite(LEFT_MOTOR, LEFT_FORWARD);
      analogWrite(RIGHT_MOTOR, RIGHT_BACKWARD);
      delay(400);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    } else if (degree == 90) {
      analogWrite(LEFT_MOTOR, LEFT_FORWARD);
      analogWrite(RIGHT_MOTOR, RIGHT_BACKWARD);
      delay(725);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    }
  }
}

void rotateServo(int direction, int degree) {
  
}

//moveForward
//Moves the wheels forward
void moveForward(int numTiles) {
  printForward();
  analogWrite(LEFT_MOTOR, LEFT_FORWARD);
  analogWrite(RIGHT_MOTOR, RIGHT_FORWARD);
  delay(TILE_TIME * numTiles);
}

//moveForward
//Moves the wheels forward using the servo library
void moveForwardServo(int numTiles) {
  Servo right;
  Servo left;
  right.attach(RIGHT_MOTOR);
  left.attach(LEFT_MOTOR);

  right.write(0);
  left.write(180);
  delay(TILE_TIME * numTiles);
}

//stopMotors
//stops the wheels
void stopMotors() {
  analogWrite(LEFT_MOTOR, 0);
  analogWrite(RIGHT_MOTOR, 0);
  startMoving = false;
}

void path() {
  moveForward(2);
  rotate(RIGHT, 90);
  moveForward(2);
  rotate(RIGHT, 90);
  moveForward(3);
  rotate(RIGHT, 90);
  moveForward(3);
  rotate(RIGHT, 90);
  moveForward(2);
  rotate(LEFT, 45);
  moveForward(1);
  rotate(LEFT, 135);
  moveForward(4);
  rotate(LEFT, 90);
  moveForward(2);
  rotate(LEFT, 90);
  moveForward(2);
  stopMotors();
}

void testPath() {
  moveForwardServo(3);
}

//displayStudentNumbers
//displays student numbers on LCD and flashes LED for 5 seconds
void displayStudentNumbers() {
 LCD.write(0xFE);
 LCD.write(0x01);
 delay(10);
 LCD.write(0xFE);
 LCD.write(128);
 LCD.print("8154187 8248005 8394837");
 delay(100);
 for (int i = 0; i < 5; i++) {
   flashLED();
   delay(1000);
 }
  startMoving = true;
}

//flashLED
//Flashes LED once
void flashLED() {
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
}

//printForward
//displays "moving forward" on LCD
void printForward() {
  LCD.write(0xFE);
  LCD.write(0x01);
  delay(10);
  LCD.write(0xFE);
  LCD.write(5 + 128);
  LCD.print("moving");
  delay(10);
  LCD.write(0xFE);
  LCD.write(4 + 64 + 128);
  LCD.print("forward");
  delay(10);
}

//printRotateLeft
//displays "rotating left" on LCD
void printRotateLeft() {
  LCD.write(0xFE);
  LCD.write(0x01);
  delay(10);
  LCD.write(0xFE);
  LCD.write(3 + 128);
  LCD.print("rotating");
  LCD.write(0xFE);
  LCD.write(5 + 64 + 128);
  LCD.print("left");
  delay(10);
}

//printRotateRight
//displays "rotating right" on LCD
void printRotateRight() {
  LCD.write(0xFE);
  LCD.write(0x01);
  delay(10);
  LCD.write(0xFE);
  LCD.write(3 + 128);
  LCD.print("rotating");
  LCD.write(0xFE);
  LCD.write(5 + 64 + 128);
  LCD.print("right");
  delay(10);
}
