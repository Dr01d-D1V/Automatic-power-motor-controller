#include <Arduino.h>

#include <PID_v1.h>

const int motorPin = 9;  // The pin the motor is attached to
const int freqPin = 10;  // The analog input pin for the frequency reading
unsigned long duration;
float frequency;

const int freqThreshold = 50;  // The frequency threshold for reducing the motor speed
const int maxSpeed = 255;  // The maximum speed for the motor

// PID tuning parameters
double Kp = 2;
double Ki = 5;
double Kd = 0;

double Setpoint = 50;

unsigned long elapsedTime = millis();
unsigned long lastExecutedMillis = 0;

double Input, Output;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  
  // Set up the PID controller
  myPID.SetMode(AUTOMATIC);

  // Set the initial setpoint to the current motor speed
}

void loop() {
  duration = pulseIn(freqPin, HIGH);
  frequency = 1 / (duration * 0.000001);

  // Read the frequency from the analog input pin
  Input = frequency;
  
  // Update the PID controller
  myPID.Compute();
  
  // Set the motor speed based on the PID output
  // int delayTime = map(Output, -10, 10, 1, 17);

  double delayTime = 1 + (17 - 1) * (1 - ((-15) - 15) / (15 - (-15))); //equivalent of map function in for inverse relationship between input and output

  digitalWrite(motorPin, LOW);

  if (elapsedTime >= 0.5) {
    digitalWrite(motorPin, HIGH);
    lastExecutedMillis = elapsedTime; 
  }
  if ((elapsedTime - lastExecutedMillis) >= delayTime) {
    digitalWrite(motorPin, LOW);
    lastExecutedMillis = elapsedTime;
  }
  if ((elapsedTime - lastExecutedMillis) >= (20 - (0.5 + delayTime))) {
    Serial.println("Done");
    lastExecutedMillis = elapsedTime;
  }

  // // Adjust the setpoint and PID tuning parameters based on the frequency reading
  // if (Input > freqThreshold) {
  //   Setpoint = Input * 0.9;  // Reduce the setpoint to slow down the motor
  //   Kp = 2;  // Increase Kp to make the control more aggressive
  //   Ki = 7;  // Increase Ki to reduce steady-state error
  //   Kd = 0;  // Set Kd to 0 to avoid overshooting
  //   myPID.SetTunings(Kp, Ki, Kd);
  // } else if (Input < freqThreshold - 1) {
  //   Setpoint = Input * 1.1;  // Increase the setpoint to speed up the motor
  //   Kp = 2;  // Set Kp back to the original value
  //   Ki = 5;  // Set Ki back to the original value
  //   Kd = 0;  // Set Kd to 0 to avoid overshooting
  //   myPID.SetTunings(Kp, Ki, Kd);
  // }
  
  // // Wait for a short time before taking the next reading
  // delay(50);
}
