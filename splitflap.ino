#include<Stepper.h>

#define STEPANGLE 5.4
#define STEPS 360/STEPANGLE
#define ROTATION 60/STEPANGLE

const int button_pin = 2;

int previous_position = 0;
int button_state; 

Stepper stepper(STEPS, 8,9,10,11);  // Those are the pin numbers the motor will operate on

void setup() {
  stepper.setSpeed(30);
  pinMode(button_pin, INPUT);
}


void loop() {
  button_state = digitalRead(button_pin);

  if(button_state == HIGH) {
    stepper.step(ROTATION);
  }  

  // Make sure the button is unpressed before we continue
  while ((button_state == HIGH)) {
    button_state = digitalRead(button_pin);
  }
}
