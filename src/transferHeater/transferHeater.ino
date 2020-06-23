//Written by Jeff Ahlers 6/21/2020
#include <PID_v1.h>
//#define relayPin B00001000
#define relayPin B00101000 //Pin 11 && 13
#define RTD A3

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
//No idea if the default Kp Ki Kd are correct
PID myPID(&Input, &Output, &Setpoint, .05, .01, 5, DIRECT);

unsigned long now = millis();
unsigned long then = millis();
bool heaterState = false;

unsigned long statusNow = millis();
unsigned long statusThen = millis();


unsigned long computeNow = millis();
unsigned long computeThen = millis();
//hello
enum serialState {
  idle,
  magic,
  tempCom,
  d1,
  d2,
  d3,
  d4,
  d5,
  finalizing
};
double unconfirmedSetpoint = 0;
serialState state = idle;
int i = 0;
void setup() {
  // Set pin 11 to output:
  DDRB = DDRB | relayPin;
  //Set pins 8-13 low
  PORTB = PORTB & B11000000;
  Setpoint = 0;
  Input = readRTD();
  myPID.SetOutputLimits(0, 100);
  //Turn the PID on
  myPID.SetMode(AUTOMATIC);
  Serial.begin(9600);
}

void loop() {
  computeNow = millis();
  if (computeNow >= computeThen + 250) {
    Input = readRTD();
    myPID.Compute();
    computeThen = computeNow;
  }
  slowPWM(Output);
  
  //Valid command format "CS ###.#E". Example: "CS 090.3E" for temperature setpoint of 90.3 C
  if (Serial.available() > 0) {
    switch (state) {
      case idle:
        //Look for magic number C
        if (Serial.read() == 67) {
          state = magic;
        }
        break;
      case magic:
      //Look for command type
        if (Serial.read() == 83) {
          state = tempCom;
        } else {
          state = idle;
        }
        break;
      //Look for space
      case tempCom:
        if (Serial.read() == 32) {
          state = d1;
        } else {
          state = idle;
        }
        break;
      //Look for digits with validation
      case d1:
        i = Serial.read();
        if (i >= 48 && i <= 57) {
          unconfirmedSetpoint += (i - 48) * 100;
          state = d2;
        } else {
          state = idle;
          unconfirmedSetpoint = 0;
        }
        break;
      case d2:
        i = Serial.read();
        if (i >= 48 && i <= 57) {
          unconfirmedSetpoint += (i - 48) * 10;
          state = d3;
        } else {
          state = idle;
          unconfirmedSetpoint = 0;
        }
        break;
      case d3:
        i = Serial.read();
        if (i >= 48 && i <= 57) {
          unconfirmedSetpoint += (i - 48);
          state = d4;
        } else {
          state = idle;
          unconfirmedSetpoint = 0;
        }
        break;
      case d4:
        if (Serial.read() == 46) {
          state = d5;
        } else {
          state = idle;
          unconfirmedSetpoint = 0;
        }
        break;
      case d5:
        i = Serial.read();
        if (i >= 48 && i <= 57) {
          unconfirmedSetpoint += (i - 48) * 0.1;
          state = finalizing;
        } else {
          state = idle;
          unconfirmedSetpoint = 0;
        }
        break;
      //Look for exit code E
      case finalizing:
        if (Serial.read() == 69) {
          Setpoint = unconfirmedSetpoint;
          Serial.print("Setpoint set to: ");
          Serial.println(Setpoint);
          unconfirmedSetpoint = 0;
          state = idle;
        } else {
          unconfirmedSetpoint = 0;
          state = idle;
        }
        break;
    }
  }
  
  //Display status message
  statusNow = millis();
  if (statusNow >= statusThen + 3000) {
    statusThen = statusNow;
    Serial.print("The setpoint is: ");
    Serial.print(Setpoint);
    Serial.print(". The current temperature is: ");
    Serial.println(readRTD());
  }
}
//Low frequency PWM for the solid state relay
void slowPWM(double setPer) {
  //In milliseconds
  double period = 250;
  double onTime = period * setPer / 100;
  now = millis();
  if (now >= then + onTime && heaterState) {
    //Turn heater off
    PORTB = PORTB & ~relayPin;
    heaterState = false;
  } else if (now >= then + period) {
    //Turn heater on
    PORTB = PORTB | relayPin;
    heaterState = true;
    then = now;
  }
}
//Read a temperature value. refResitance and the slope should be calibrated. Nominal: 100 Ohm refResist, 0.385 Ohm/C, 100 Ohm at 0 C
double readRTD() {
  double refResistance = 100.0;
  double vOut = analogRead(RTD) * 5.0 / 1024.0;
  double resistance = refResistance * vOut / (5.0 - vOut);
  double temperature = (resistance - 100.0) / .385;
  return temperature;
}
