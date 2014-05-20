int LED_neg =2;
int LED_pos =3;

void setup()
{
	pinMode(LED_pos,OUTPUT);
	pinMode(LED_neg,OUTPUT);
	Serial.begin(9600);
}

void loop()
{

	unsigned long time;

	pinMode(LED_neg,OUTPUT);
	pinMode(LED_pos,OUTPUT);
	digitalWrite(LED_neg,HIGH);
	digitalWrite(LED_pos,LOW);

	pinMode(LED_neg,INPUT);
	digitalWrite(LED_neg,LOW);

	time=millis();

	while(digitalRead(LED_neg)==HIGH)
	{
	}
	time=millis()-time;
	Serial.println(time);
}
