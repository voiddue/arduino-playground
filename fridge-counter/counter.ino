#include "interface.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
Interface UI (5, 3, 4);

int buttonPin = 2;
int buttonState =0;
int count_bottles;
int count_refill_overall;
int count_bottles_overall;
unsigned int count_opened;
unsigned int rounds=0;

bool fridge_open=false;

void setup() {
  pinMode(buttonPin,INPUT_PULLUP);
  count_opened = EEPROMReadInt(0);
  count_bottles_overall = EEPROMReadInt(2);
  count_refill_overall = EEPROMReadInt(4);
  lcd.begin(16, 2);
  lcd.print("Fridge closed");
  lcd.setCursor(0, 1);
  lcd.print(count_opened);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()){
    switch(Serial.read()){
      case 'd':
        deleteEEPROM();
        Serial.println("done");
        break;
    }
  }
  
  writeDisplay();
  
  UI.pollButtons();
  count_bottles = UI.entered.taken;
     
  if (buttonState != digitalRead(buttonPin))
  {
	UI.resetInput();
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH)
    {
		count_opened++;
		EEPROMWriteInt(0,count_opened);
		fridge_open = true;
    }
	else
	{
		fridge_open = false;
		count_bottles_overall += count_bottles;
		count_refill_overall = UI.entered.refilled==true ? count_refill_overall + 1 : count_refill_overall;
		EEPROMWriteInt(2,count_bottles_overall);
		EEPROMWriteInt(4,count_refill_overall);
	}
  }
  delay(100);
}

void writeDisplay()
{
	
	lcd.clear();
	lcd.setCursor(0, 0);
	
	if (fridge_open==true)
	{
		lcd.print("I take ");
		lcd.print(count_bottles);
		lcd.setCursor(0, 1);
		lcd.print("bottles.");
	} 
	else
	{
		rounds++;
		if (rounds>=300)rounds=0;
		lcd.print("Fridge closed");
		lcd.setCursor(0, 1);
		switch ((int)rounds/100)
		{
		case 0:
			lcd.print(count_opened);
			lcd.print(" times");
			break;
			
		case 1:
			lcd.print(count_bottles_overall);
			lcd.print(" Bottles");
			break;
			
		case 2:
			lcd.print(count_refill_overall);
			lcd.print(" refills");
			break;
		}
	}
	
}

//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
void EEPROMWriteInt(int p_address, int p_value)
      {
      byte lowByte = ((p_value >> 0) & 0xFF);
      byte highByte = ((p_value >> 8) & 0xFF);

      EEPROM.write(p_address, lowByte);
      EEPROM.write(p_address + 1, highByte);
      }

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
      {
      byte lowByte = EEPROM.read(p_address);
      byte highByte = EEPROM.read(p_address + 1);

      return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
      }
//This function will delete first 512 byte of the eeprom
void deleteEEPROM()
{
    Serial.println("deleting EEPROM");
    for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
    count_opened =0;
    Serial.println("EEPROM deleted");
}
