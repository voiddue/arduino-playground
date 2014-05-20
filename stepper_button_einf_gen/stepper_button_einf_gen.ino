#include <Stepper.h>

#define STEPS 100

int schalter = 10; //setzt pin 10 als schalter fest
int zehn;
Stepper stepper(STEPS, 13, 12, 9, 8);

void setup()
{
  stepper.setSpeed(30);
  pinMode(schalter, INPUT);
  digitalWrite (schalter, HIGH);
}


void loop() 
{
  int val = digitalRead(schalter);
  if (val == HIGH) {
  zehn = 10;
  }
  else {
  zehn = 0;
  }  
  stepper.step(zehn);
  
}
