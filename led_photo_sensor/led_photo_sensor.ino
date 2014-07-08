int led = 13;
int basis = 0;
int sens = 0;
int x = 0;
int y = 0;
int totaal = 0;
int totaal1 = 0;

int loopCount = 0;

void setup() {
  // Used for Debugging
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  // here we do 50 readings to set the sensors
 for(x = 0; x < 50; x++) {
   sens = analogRead(0);
   totaal = totaal1 + sens;
   totaal1 = totaal;
 }
 sens = totaal/x;                          // divide the 50 readings by 50 again
 totaal = 0;
 totaal1 = 0;
 basis = sens-20;           // setting sensitivity - now it will react if the LED is 20 lower than the setting above
 debugConsole(basis, 0);
  for(y = 0; y < 100; y++){            // after every 1000 tests the program will reset the led to cope with changing light
     for(x = 0; x < 50; x++) {      // 50 readings to see if the sensor is in the dark
       sens = analogRead(0);
       totaal = totaal1 + sens;
       totaal1 = totaal;
       //delay(10);
     }
     sens = totaal/x;
     if (sens < basis) {                // testing is the led was in the dark
       digitalWrite(led, HIGH);  // turning the led in port 13 or on the board on if the sensor-led was 20 darker than now than in the setting
       debugConsole(sens, 1);
     }
       else {
       digitalWrite(led, LOW); // turning it of if not
       debugConsole(sens, 1);
       }
 totaal = 0;  
 totaal1 = 0;
 }
 }

// Sets the Base Value of the Sensor
int setBaseValue () {
  
}
 
void debugConsole(int data, int sensorArt) {
  switch (sensorArt) {
    case 0:
      Serial.print("Basiswert: ");
      Serial.println(data);
    break;
    case 1:
      Serial.print("Sensorwert: ");
      Serial.println(data);
    break;
  }
}
 

