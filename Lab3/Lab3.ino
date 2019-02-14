// Taewan Kang (8394837)
// Ashwin Anand (8154187)
// Quang-Tri Do (8248005)
// SEG4145
// Lab 3, GROUP 1
// Lab3.ino
// Contains all code to perform tasks for Lab 3

#include <SoftwareSerial.h>
#include <Servo.h>
 

// Define Constants
#define LEFT_MOTOR      45
#define RIGHT_MOTOR     8
#define LCD_DISPLAY     18
#define LED             13
#define SONAR           22

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
Servo LEFTSERVO;
Servo RIGHTSERVO;

SoftwareSerial LCD(0, LCD_DISPLAY);

boolean startMoving = true;


//setup
//Standard setup for arduino
void setup() {
 // Initialize all pins.
 //pinMode(LEFT_MOTOR, OUTPUT);
 //pinMode(RIGHT_MOTOR, OUTPUT);
 
 pinMode(LED, OUTPUT);

 LCD.begin(9600);
 delay(10);
 delay(3000);
 

}

void loop() {
 if (startMoving)
  testPath();
}

void initiateServo(){
  LEFTSERVO.attach(LEFT_MOTOR);
  RIGHTSERVO.attach(RIGHT_MOTOR);
  delay(100);
}

void detachServo(){
  LEFTSERVO.detach();
  RIGHTSERVO.detach();
  delay(100);
}

//moveForward
//Moves the wheels forward using the servo library
void moveForward(int numTiles) {
  printForward();
  LEFTSERVO.write(180);
  RIGHTSERVO.write(62);
  delay(TILE_TIME * numTiles);
}

void moveBackward(int numTiles){
  printBackward();
  LEFTSERVO.write(0);
  RIGHTSERVO.write(180);
  delay(TILE_TIME * numTiles);
}

void rotate(int direction, int degree){
  if (direction == LEFT) {
    printRotateLeft();
    if (degree == 45) {
      LEFTSERVO.write(45);
      RIGHTSERVO.write(0);
      delay(450);
    } else if (degree == 90) {
      LEFTSERVO.write(45);
      RIGHTSERVO.write(0);
      delay(900);
    }
    
  } else if (direction == RIGHT) {
    printRotateRight();
    if (degree == 45) {
      LEFTSERVO.write(90);
      RIGHTSERVO.write(180);
      delay(450);
    } else if (degree == 90) {
      LEFTSERVO.write(90);
      RIGHTSERVO.write(180);
      delay(750);
    }
  }
}

//stopMotors
//stops the wheels
void stopMotors() {
  analogWrite(LEFT_MOTOR, 0);
  analogWrite(RIGHT_MOTOR, 0);
  startMoving = false;
}

void traverse() {
  moveForward(1);
  delay(1000);
}

void testPath() {
  initiateServo();
  printBackward();
  detachServo();
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

//printBackward
//displays "moving backward" on LCD
void printBackward() {
  LCD.write(0xFE);
  LCD.write(0x01);
  delay(10);
  LCD.write(0xFE);
  LCD.write(5 + 128);
  LCD.print("moving");
  delay(10);
  LCD.write(0xFE);
  LCD.write(4 + 64 + 128);
  LCD.print("backward");
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

long readSonar() {
  pinMode(SONAR, OUTPUT);
  digitalWrite(SONAR, LOW);
  delay(0.002);
  digitalWrite(SONAR, HIGH);
  delay(0.005);
  digitalWrite(SONAR, LOW);
  pinMode(SONAR, INPUT);
  unsigned long duration = pulseIn(SONAR, HIGH);
  long distance = duration / (29 * 2);
  return distance;
}
