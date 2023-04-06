#include <Arduino.h>

#include <PID_v1.h>

const int motorPin = 9;  // The pin the motor is attached to
const int freqPin = A0;  // The analog input pin for the frequency reading
const int freqThreshold = 50;  // The frequency threshold for reducing the motor speed
const int maxSpeed = 255;  // The maximum speed for the motor

// PID tuning parameters
double Kp = 2;
double Ki = 5;
double Kd = 0;

double Setpoint, Input, Output;
PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  
  // Set up the PID controller
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(50);
  pid.SetOutputLimits(0, maxSpeed);
  
  // Set the initial setpoint to the current motor speed
  Setpoint = analogRead(freqPin);
}

void loop() {
  // Read the frequency from the analog input pin
  Input = analogRead(freqPin);
  
  // Update the PID controller
  pid.Compute();
  
  // Set the motor speed based on the PID output
  analogWrite(motorPin, Output);
  
  // Print the frequency and motor speed for debugging
  Serial.print("Frequency: ");
  Serial.print(Input);
  Serial.print(", Speed: ");
  Serial.println(Output);
  
  // Adjust the setpoint and PID tuning parameters based on the frequency reading
  if (Input > freqThreshold) {
    Setpoint = Input * 0.9;  // Reduce the setpoint to slow down the motor
    Kp = 2;  // Increase Kp to make the control more aggressive
    Ki = 7;  // Increase Ki to reduce steady-state error
    Kd = 0;  // Set Kd to 0 to avoid overshooting
    pid.SetTunings(Kp, Ki, Kd);
  } else if (Input < freqThreshold - 1) {
    Setpoint = Input * 1.1;  // Increase the setpoint to speed up the motor
    Kp = 2;  // Set Kp back to the original value
    Ki = 5;  // Set Ki back to the original value
    Kd = 0;  // Set Kd to 0 to avoid overshooting
    pid.SetTunings(Kp, Ki, Kd);
  }
  
  // Wait for a short time before taking the next reading
  delay(50);
}
