const int ledPin1 = 13;
const int ledPin2 = 9;
const int sensorPin = A0;

const int sensorReadings = 1000;

int sensorValue = 0;
int sensorMin = 1023;
int sensorMax = 0;

int x = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, HIGH);
  
  for(x = 0; x < 50, x++) {
    sensorValue = analogRead(sensorPin);
    
    if (sensorValue > )
  }
  
  digitalWrite(ledPin1, LOW);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.print("Value: ");
  Serial.println(sensorValue);
  delay(1000);
}
