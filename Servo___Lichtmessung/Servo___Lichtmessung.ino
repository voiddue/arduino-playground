//Sketch setzt sich aus "Sweep"(aus Beispiele) und "Lichtempfindlicher Widerstand"(s.Arduino Kochbuch K. 6.2)

#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
int pos = 0;    // variable to store the servo position 
 

const int ledPin = 13;
const int sensorPin = 0;

void setup()
{

myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
pinMode(ledPin, OUTPUT);
Serial.begin(9600);
}

void loop()
{
int rate = analogRead(sensorPin);
digitalWrite(ledPin,HIGH);
delay(500);
Serial.print("Lichtstärke:");
Serial.println(rate);
digitalWrite(ledPin,LOW);
delay(500);
if(rate<200)
{
   for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    Serial.println(pos);
    delay(5);                       // waits 15ms for the servo to reach the position 
  } 
Serial.println("...in der Nacht!");

}
}
