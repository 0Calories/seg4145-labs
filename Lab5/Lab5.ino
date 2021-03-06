/*
WebClient.pde - Web Client Arduino processing sketch

Copyright (C) 2011 DIYSandbox LLC

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <SoftwareSerial.h>
#include <stdio.h>
#include <Wire.h>
#include <Servo.h>
#include <Wirefree.h>
#include <WifiClient.h>


// Macros
#define LEFT_FORWARD()    analogWrite(45, 191.5);
#define LEFT_BACKWARD()   analogWrite(45, 10);
#define LEFT_STOP()       analogWrite(45, 0);
#define RIGHT_FORWARD()   analogWrite(8, 10);
#define RIGHT_BACKWARD()  analogWrite(8, 191.5);
#define RIGHT_STOP()      analogWrite(8, 0);
#define TEMPSENSOR        0x68
#define LEFT_MOTOR      45
#define RIGHT_MOTOR     8

// Time constants
#define TILE_TIME       1250

WIFI_PROFILE wireless_prof = {
                        /* SSID */ "stingray",
         /* WPA/WPA2 passphrase */ "",
                  /* IP address */ "192.168.1.101",
                 /* subnet mask */ "255.255.255.0",
                  /* Gateway IP */ "192.168.1.1", };

String server = "192.168.1.159"; // Google
// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WifiClient client(server, "6969", PROTO_TCP);

// Global variables
int flashing, checkSonar;
int turning = 0;
long pingDuration;
long distance;
byte temperatureData;
SoftwareSerial LCD = SoftwareSerial(0, 18); // Initialize the LCD screen
int reg = 0x01; // For ambient temperature reader
int command = 0; // Store user command 1-7
int commandInput[3] = {-1, -1, -1};
int count; // Stores current byte
boolean turnedRight = true;
int currentLineIsBlank;

int LEFT = 1;
int RIGHT = 2;
Servo LEFTSERVO;
Servo RIGHTSERVO;
Servo RotationServoLeft;
Servo RotationServoRight;
Servo RotationServoLeft2;
Servo RotationServoRight2;

// the setup function runs once when you press reset or power the board
void setup() {

    // Assign all required pins
    pinMode(13, OUTPUT); // LED
    pinMode(45, OUTPUT); // Left Motor
    pinMode(8, OUTPUT); // Right Motor
    pinMode(49, INPUT); // Left wheel sensor
    pinMode(48, INPUT); // Right wheel sensor

    // Asign LCD Port
    LCD.begin(9600);

    // Clear LCD
    clearLCD();

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Set cursor to approximately the middle of the LCD first row
    LCD.write(5 + 0*64 + 128);
    delay(10);

    // Print first student number
    LCD.print("8394837");
    delay(10);

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Set cursor to approximately the middle of the LCD second row
    LCD.write(5 + 1*64 + 128);

    // Print the second student number
    LCD.print("8154187");
    delay(10);
    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Set cursor to approximately the middle of the LCD second row
    LCD.write(5 + 1*64 + 128);

    // Print the second student number
    LCD.print("8248005");
    delay(10);
    // Flash light for ~5s
    flashLight(5);

    // Clear LCD
    clearLCD();

    // Join I2C bus for ambient temp
    Wire.begin();

    // connect to AP
    Wireless.begin(&wireless_prof);

    // Try and connect to client
    if (client.connect()) {
        // prints to serial if connected
        Serial.println("Connection successful.");

        // Send message over UDP socket to peer device
        client.println("Robot now connected.");
    } else {
        // prints to serial if not connected
        Serial.println("Connection failed.");
    }
}

// the loop function runs over and over again forever
void loop() {
    // if there are incoming bytes available from the peer
    // device, read them and print them:
    while (client.available()) {

        int a;
  char c;

        while ((c = client.read()) == -1); // While nothing returned

    // Assign int to char
    //a = c; // may need to add -->  + '0'

    // If new line is reached and current line is blank
    if (c == '\n' && currentLineIsBlank == 1) {

      // Reset count
      count = 0;

      // Create empty array of size 2
      int action[2] = {};

      // Store command in first spot
      action[0] = command;
                        Serial.println(action[0]);

      // Get input and store in second spot
      int i, k = 0;
      for (i = 0; i < 3; i++) {
        if (commandInput[i] != -1) {
          k = 10 * k + commandInput[i];
        }
      }
      action[1] = k;
                        Serial.println(action[1]);

      // Process user action
      processUserAction(action);

      // Reset command array
      commandInput[0] = -1;
                        commandInput[1] = -1;
                        commandInput[2] = -1;

      // Send response to server
      client.println("Robot has responded, waiting for next instruction.");

      // Break current iteration of while loop
      continue;
    }

    // If new line
    if (c == '\n') {
      currentLineIsBlank = 1;
      count = 0;
    } else if (c != '\r') { // If byte received
      currentLineIsBlank = 0;

      // Get the command
      if (count == 0) {

        if (c == '07') {
          Serial.println("Robot quitting.");
          break;
        }

        // Store as int
        command = c - '0';
      } else {
                // Add to array
                commandInput[count-1] = c - '0';
            }

      // Increment count
      count++;

    }
    }

    // Delay for 1 millisecond
    delay(1);
}

/**
* Clears all characters from the LCD screen.
* @name clearLCD
* @return (void)
*/
void clearLCD() {
    LCD.write(0xFE);  // Put the LCD screen in command mode.
    LCD.write(0x01);  // Clear the LCD screen.
    delay(10);
}

/**
* Prints a message to the LCD screen.
* @name printMessage
* @param word1 word on first line, word2 word of second line
* @return (void)
*/
void printMessage(char* word1, char* word2) {
    // Clear the LCD
    clearLCD();

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Get word1 length
    int word1Len = strlen(word1);

    // Set cursor to first row, first column
    LCD.write(((16-word1Len)/2) + 0*64 + 128);

    // Print the message
    LCD.print(word1);

    // If there is another word
    if (word2 != 0) {
        // Put the LCD screen in command mode.
        LCD.write(0xFE);

        // Get word1 length
        int word2Len = strlen(word2);

        // Set cursor to first row, first column
        LCD.write(((16-word2Len)/2) + 1*64 + 128);

        // Print the message
        LCD.print(word2);
    }

    // Delay for 10 milliseconds
    delay(10);
}

/**
* Prints temperature to LCD Screen
* @name printTemperature
* @param word1 word on first line, word2 word of second line
* @return (void)
*/
void printTemperature(char* word1, byte word2) {
    // Clear the LCD
    clearLCD();

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Get word1 length
    int word1Len = strlen(word1);

    // Set cursor to first row, first column
    LCD.write(((16-word1Len)/2) + 0*64 + 128);

    // Print the message
    LCD.print(word1);

    // If there is another word
    if (word2 != 0) {
        // Put the LCD screen in command mode.
        LCD.write(0xFE);

        // Set cursor to first row, first column
        LCD.write(0 + 1*64 + 128);

        // Print the message
        LCD.print(word2);
    }

    // Delay for 10 milliseconds
    delay(10);
}

/**
* Prints distance of closest object to LCD Screen
* @name printDistance
* @param word1 word on first line, word2 word of second line
* @return (void)
*/
void printDistance(char* word1, long word2) {
    // Clear the LCD
    clearLCD();

    // Put the LCD screen in command mode.
    LCD.write(0xFE);

    // Get word1 length
    int word1Len = strlen(word1);

    // Set cursor to first row, first column
    LCD.write(0 + 0*64 + 128);

    // Print the message
    LCD.print(word1);

    // If there is another word
    if (word2 != 0) {
        // Put the LCD screen in command mode.
        LCD.write(0xFE);

        // Set cursor to first row, first column
        LCD.write(0 + 1*64 + 128);

        // Print the message
        LCD.print(word2);
    }

    // Delay for 10 milliseconds
    delay(10);
}

/**
* Flashes the light every second for num amount of time.
* @name flashLight
* @param num number of seconds that the light will be flashing
* @return (void)
*/
void flashLight(int num) {
    for (flashing = 0; flashing < num; flashing++) {
        // Wait for 1s
        delay(1000);

        // turn the LED on (HIGH is the voltage level)
        digitalWrite(13, HIGH);

        // Wait for 0.01s
        delay(10);

        // turn the LED on (HIGH is the voltage level)
        digitalWrite(13, LOW);
    }
}

/**
* Adjust the speed of the wheels
* @name adjustSpeed
* @param motor number of motor
* @return (void)
*/
void adjustSpeed(int motor) {

    if (motor == 45) {
      RIGHT_FORWARD();
      analogWrite(motor, 150);
    } else {
      LEFT_FORWARD();
      analogWrite(motor, 100);
    }

}

/**
* Performs an action for a predefined number of "ticks"
* @name actionLength
* @param ticks number of wheel ticks that must be processed
* @return (void)
*/
void actionLength(int ticks) {
    int timeLeft = 0;
    int timeRight = 0;
    int oldPulseLeft = NULL;
    int newPulseLeft = NULL;
    int oldPulseRight = NULL;
    int newPulseRight = NULL;
    int valid = 0;

    while (valid == 0) {

        // Read left motor  sensor
        oldPulseLeft = newPulseLeft;
        newPulseLeft = digitalRead(48);

        // Read right motor sensor
        oldPulseRight = newPulseRight;
        newPulseRight = digitalRead(49);

        // If new tick, increment left time
        if (newPulseLeft != oldPulseLeft) {
            timeLeft++;
        }

        // If new tick, increment right time
        if (newPulseRight != oldPulseRight) {
            timeRight++;
        }

        // If the robot is not currently turning
        if (turning != 1) {

          // Check if sensors are equal or not
          if (timeLeft > timeRight) {
              adjustSpeed(45);
          } else if (timeRight > timeLeft) {
              adjustSpeed(8);
          }
        }
        
        // Set valid flag to true if both are done    
        if (timeLeft >= ticks && timeRight >= ticks) {
          valid = 1;
        }
    }
}

/**
* Retrieves user action and processes it accordingly
* @name processUserAction
* @params
* @return (void)
*/
void processUserAction(int input[]) {
    // Store selection
    int selection = input[0];
    
    //Serial.println(input[0]);

    // Switch case
    switch(selection) {
        case 1:
            moveForward(input[1]);
            stopMotor(10);
            break;
        case 2:
            moveBackward(input[1]);
            stopMotor(10);
            break;
        case 3:
            turnRight(input[1]);
            stopMotor(10);
            break;
        case 4:
            turnLeft(input[1]);
            stopMotor(10);
            break;
        case 5:
            readSonar();
            printDistance("Distance", distance);
            break;
        case 6:
            readTemp(input[1]);
            printTemperature("Temperature", temperatureData);
            break;
    }
}

/**
* Takes cm distance and converts to ticks
* @name calculateDistance
* @params distance distance in cm
* @return int number of ticks
*/
int calculateDistance(int distance) {
    return (distance * 4);
}

/**
* Takes degrees and converts to ticks
* @name calculateDegrees
* @params degrees degrees in
* @return int number of ticks
*/
int calculateDegrees(long degrees) {
    return (degrees * 0.5);
}

/**
* Read the sonar results and calculates the resulting distance
* @name readSonar
* @return (void)
*/
void readSonar(){

    // Output Sonar sensor
    pinMode(22, OUTPUT);

    // Write low value to pin
    digitalWrite(22, LOW);

    //Wait for 2 microsecs
    delay(0.002);

    // Write high value to pin
    digitalWrite(22, HIGH);

    //Wait for 5 microsecs
    delay(0.005);

    //Write low value to pin
    digitalWrite(22, LOW);

    // Input Sonar sensor
    pinMode(22, INPUT);

    // Read duration of the pulse
    pingDuration = pulseIn(22, HIGH);

    // Calculate distance
    distance = pingDuration / (29*2);

    // Delay 10 milliseconds
    delay(10);
}

/**
* Reads the ambient temperature
* @name readTemp
* @return (void)
*/
void readTemp(int type) {
    Wire.beginTransmission(TEMPSENSOR);
    Wire.write(reg); // Indicate temperature value to read
    Wire.endTransmission();
    Wire.requestFrom(TEMPSENSOR, 1); // Request data
    while(Wire.available() < 1); // Wait for data
    temperatureData = Wire.read(); // Store temperature value
  // Change if fahrenheit
  if (type == 2) {
    temperatureData = temperatureData * 1.8 + 32;
  }
    delay(50); // Delay 50 ms if values are read in a loop
}

/**
* Stops the robot. A delay is provided to determine how long
* the robot will remain in this state.
* @name stopMotor
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void stopMotor(int duration) {
    LEFT_STOP();
    RIGHT_STOP();
    turning = 0;
    printMessage("Stopped", 0);
    delay(duration);
}


void moveBackward(int numTiles){
  LEFTSERVO.attach(LEFT_MOTOR);
  RIGHTSERVO.attach(RIGHT_MOTOR);
  delay(100);

  printMessage("Moving", "Backward");
  LEFTSERVO.write(0);
  RIGHTSERVO.write(180);
  delay(TILE_TIME * numTiles / 5);

  LEFTSERVO.detach();
  RIGHTSERVO.detach();
  delay(100);
}

//moveForward
//Moves the wheels forward using the servo library
void moveForward(int numTiles) {
  LEFTSERVO.attach(LEFT_MOTOR);
  RIGHTSERVO.attach(RIGHT_MOTOR);
  delay(100);
  
  printMessage("Moving", "Forward");
  LEFTSERVO.write(180);
  RIGHTSERVO.write(62);
  
  delay(numTiles*TILE_TIME / 5);

  LEFTSERVO.detach();
  RIGHTSERVO.detach();
  delay(100);
}

/**
* Turns the robot left. A delay is provided to determine how long
* the robot will remain in this state.
* @name turnLeft
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnLeft(int duration) {
  RotationServoLeft.attach(LEFT_MOTOR);
  RotationServoRight.attach(RIGHT_MOTOR);
  delay(100);
  turnedRight = false;
  
  printMessage("Rotating", "Left");
  RotationServoLeft.write(45);
  RotationServoRight.write(0);
  
  delay(900 / duration);

  RotationServoLeft.detach();
  RotationServoRight.detach();
  delay(100);
}

/**
* Turns the robot right. A delay is provided to determine how long
* the robot will remain in this state.
* @name turnRight
* @param duration number of milliseconds that the robot will remain in this state
* @return (void)
*/
void turnRight(int duration) {
  RotationServoLeft2.attach(LEFT_MOTOR);
  RotationServoRight2.attach(RIGHT_MOTOR);
  delay(100);
  turnedRight = true;
  
  printMessage("Rotating", "Right");
  RotationServoLeft2.write(90);
  RotationServoRight2.write(180);
  
  delay(900 / duration);

  RotationServoLeft2.detach();
  RotationServoRight2.detach();
  delay(100);
}
