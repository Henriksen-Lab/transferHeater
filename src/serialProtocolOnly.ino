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
double Setpoint = 0;
double unconfirmedSetpoint = 0;
serialState state = idle;
int i = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

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
        if (Serial.read() == 83) {
          state = tempCom;
        } else {
          state = idle;
        }
        break;
      case tempCom:
        if (Serial.read() == 32) {
          state = d1;
        } else {
          state = idle;
        }
        break;
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
}
