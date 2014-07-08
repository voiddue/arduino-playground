#include <Stepper.h>

#define STEPS 100

Stepper stepper(STEPS, 9, 8, 7, 6);
volatile int interrupt;

void setup()
{
 Serial.begin(9600);
  stepper.setSpeed(30);
 pinMode(3,INPUT_PULLUP);
 attachInterrupt(1, abbruch, CHANGE);
 Serial.println("Hallo");
 
 interrupt = 0;
}

void abbruch()

{
//Serial.begin(9600);
  //stepper.step(1000);
  interrupt = 1;
}

void loop()
{
  // Serial.println(digitalRead(3)););
  if (interrupt) {
     interrupt = 0;
     Serial.println("So weit ist das schon gekommen!"); 
     
     delay(2000);
  }
}
