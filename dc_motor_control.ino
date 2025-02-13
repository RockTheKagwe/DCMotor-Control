/* This code is used for DC Motor Control using the L298N Driver.
 *  The pushbutton is used to start or stop the motor.
 *  The potentiometer is used to vary the speed of the motor from 0 to 100%. 
 */
 
// Include the Arduino Framework
#include <Arduino.h>

// Pin Definitions
const int potPin = A0;      // Potentiometer pin
const int buttonPin = 2;    // Pushbutton pin
const int motorEnable = 5;  // PWM Motor Speed Control (ENA)
const int motorIn1 = 6;     // Motor Direction (IN1)
const int motorIn2 = 7;     // Motor Direction (IN2)

// Variables
bool motorState = false;    // Motor ON/OFF state
bool lastButtonState = HIGH;

void setup() {
    pinMode(buttonPin, INPUT);  // External pull-up for button
    pinMode(motorEnable, OUTPUT);
    pinMode(motorIn1, OUTPUT);
    pinMode(motorIn2, OUTPUT);

    // Set forward direction
    digitalWrite(motorIn1, HIGH);  
    digitalWrite(motorIn2, LOW);   
}

void loop() {
    // Read pushbutton state with debounce
    bool buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && lastButtonState == HIGH) {
        motorState = !motorState; // Toggle motor state
    }

    lastButtonState = buttonState;

    // Read potentiometer value for speed
    int potValue = analogRead(potPin);
    int motorSpeed = map(potValue, 0, 1023, 0, 255); // Map to PWM range

    if (motorState) {
        analogWrite(motorEnable, motorSpeed); // Set motor speed
    } 
    else {
        analogWrite(motorEnable, 0); // Stop motor
    }
}
