#include <SoftwareSerial.h>

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

int STRAIGHT = 0;
int LEFT = 1;
int RIGHT = 2;

SoftwareSerial LCD(0, LCD_DISPLAY);

boolean startMoving = false;


void setup() {
 // Initialize all pins.
 pinMode(LEFT_MOTOR, OUTPUT);
 pinMode(RIGHT_MOTOR, OUTPUT);
 pinMode(LED, OUTPUT);

 LCD.begin(9600);
 displayStudentNumbers();
}

void loop() {
 if (startMoving)
  path2();
  //spin();
}

void backward() {
 analogWrite (LEFT_MOTOR, LEFT_BACKWARD);
}

// Takes 2.79s for full rotation
void rotate(int direction, int degree) {
  if (direction == LEFT) {
    if (degree == 45) {
      analogWrite (LEFT_MOTOR, LEFT_BACKWARD);
      analogWrite (RIGHT_MOTOR, RIGHT_FORWARD);
      delay(475);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    } else if (degree == 90) {
      analogWrite (LEFT_MOTOR, LEFT_BACKWARD);
      analogWrite (RIGHT_MOTOR, RIGHT_FORWARD);
      delay(950);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    }
  } else if (direction == RIGHT) {
    if (degree == 45) {
      analogWrite(LEFT_MOTOR, LEFT_FORWARD);
      analogWrite(RIGHT_MOTOR, RIGHT_BACKWARD);
      delay(325);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    } else if (degree == 90) {
      analogWrite(LEFT_MOTOR, LEFT_FORWARD);
      analogWrite(RIGHT_MOTOR, RIGHT_BACKWARD);
      delay(650);
      analogWrite(LEFT_MOTOR, 0);
      analogWrite(RIGHT_MOTOR, 0);
    }
  }
}

void spin() {
  analogWrite(LEFT_MOTOR, LEFT_FORWARD);
  analogWrite(RIGHT_MOTOR, RIGHT_BACKWARD);
}

void moveForward() {
  analogWrite(LEFT_MOTOR, LEFT_FORWARD);
  analogWrite(RIGHT_MOTOR, RIGHT_FORWARD);
}

void stopMotors() {
  analogWrite(LEFT_MOTOR, 0);
  analogWrite(RIGHT_MOTOR, 0);
}

void path1() {
  moveForward();
  delay(5000);
  rotate(LEFT, 90);
  moveForward();
  delay(5000);
  rotate(RIGHT, 90);
  moveForward();
  delay(5000);
  rotate(RIGHT, 90);
  moveForward();
  delay(5000);
  rotate(LEFT, 90);
  moveForward();
  delay(5000);
  stopMotors();
}

void path2() {
  moveForward();
  delay(5000);
  rotate(RIGHT, 90);
  moveForward();
  delay(5000);
  rotate(LEFT, 90);
  rotate(LEFT, 45);
  moveForward();
  delay(5000);
  rotate(RIGHT, 135);
  moveForward();
  delay(5000);
  rotate(LEFT, 90);
  moveForward();
  delay(5000);
  stopMotors();
  
}


void displayStudentNumbers() {

 LCD.write(0xFE);
 LCD.write(0x01);
 delay(10);

  
  LCD.print("8154187, 8248005, 8394837");
  for (int i = 0; i < 5; i++) {
    flashLED();
    delay(1000);
  }

  LCD.write(0xFE);
  LCD.write(0x01);
  delay(10);
  LCD.print("Test");
  startMoving = true;
}

void flashLED() {
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
}
