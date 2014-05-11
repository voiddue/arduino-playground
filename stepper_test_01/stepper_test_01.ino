
#include <Stepper.h>

#define STEPS 100


Stepper stepper(STEPS, 13, 12, 11, 10);

void setup()
{
  stepper.setSpeed(30);
}


void loop() 
{
  //stepper.step(10);
}
