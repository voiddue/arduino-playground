#include <Wire.h>
#include <DS1307new.h>
#include <LiquidCrystal_I2C.h>

// *********************************************
// VARIABLES
// *********************************************
LiquidCrystal_I2C lcd(0x27,16,2);
uint16_t startAddr = 0x0000;            // Start address to store in the NV-RAM
uint16_t lastAddr;                      // new address for storing in NV-RAM
uint16_t TimeIsSet = 0xaa55;            // Helper that time must not set again

void set_time()
{
    RTC.stopClock();
        
    RTC.fillByYMD(2012,6,22);
    RTC.fillByHMS(8,59,4);
    
    RTC.setTime();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
    RTC.startClock();
}

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
//  set_time();
}

void loop()
{
  lcd.setCursor(0, 0);
  RTC.getTime();
  if (RTC.hour < 10)                    // correct hour if necessary
  {
    lcd.print("0");
    lcd.print(RTC.hour, DEC);
  } 
  else
  {
    lcd.print(RTC.hour, DEC);
  }
    lcd.print(":");
  if (RTC.minute < 10)                  // correct minute if necessary
  {
    lcd.print("0");
    lcd.print(RTC.minute, DEC);
  }
  else
  {
    lcd.print(RTC.minute, DEC);
  }
    lcd.print(":");
  if (RTC.second < 10)                  // correct second if necessary
  {
    lcd.print("0");
    lcd.print(RTC.second, DEC);
  }
  else
  {
    lcd.print(RTC.second, DEC);
  }
  
  uint8_t MESZ;
  
  MESZ = RTC.isMEZSummerTime();
  lcd.print("  MEZ: ");
  lcd.print(MESZ, DEC);    
 
  
    lcd.setCursor(0, 1);
  if (RTC.day < 10)                    // correct date if necessary
  {
    lcd.print("0");
    lcd.print(RTC.day, DEC);
  }
  else
  {
    lcd.print(RTC.day, DEC);
  }
    lcd.print("-");
  if (RTC.month < 10)                   // correct month if necessary
  {
    lcd.print("0");
    lcd.print(RTC.month, DEC);
  }
  else
  {
    lcd.print(RTC.month, DEC);
  }
    lcd.print("-");
    lcd.print(RTC.year, DEC);          // Year need not to be changed
    lcd.print(" ");
  switch (RTC.dow)                      // Friendly printout the weekday
  {
    case 1:
      lcd.print("MON");
      break;
    case 2:
      lcd.print("TUE");
      break;
    case 3:
      lcd.print("WED");
      break;
    case 4:
      lcd.print("THU");
      break;
    case 5:
      lcd.print("FRI");
      break;
    case 6:
      lcd.print("SAT");
      break;
    case 7:
      lcd.print("SUN");
      break;
  }
}
