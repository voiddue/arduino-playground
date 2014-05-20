#include "interface.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

Interface::Interface(uint8_t increase, uint8_t decrease, uint8_t refill)
{
	init (increase,decrease, refill);
}

void Interface::init(uint8_t increase, uint8_t decrease, uint8_t refill)
{
	increase_pin = increase;
	decrease_pin = decrease;
	refill_pin = refill;
	
	pinMode(increase_pin,INPUT_PULLUP);
	pinMode(decrease_pin,INPUT_PULLUP);
	pinMode(refill_pin,INPUT_PULLUP);
}

void Interface::debounce ()
{
	delay(50);
}

void Interface::pollButtons()
{
	if (digitalRead(increase_pin)==LOW)
	{
		entered.taken++;
		debounce();
	}
	
	if (digitalRead(decrease_pin)==LOW)
	{
		entered.taken--;
		if (entered.taken<1) entered.taken = 1;
		debounce();
	}
	
	if (digitalRead(refill_pin)==LOW)
	{
		entered.refilled = (entered.refilled==true) ? false : true;
		debounce();
	}
}

void Interface::resetInput()
{
	entered.refilled=0;
	entered.taken=0;
}