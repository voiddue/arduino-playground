#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1307new.h>
#include <avr/pgmspace.h>


//#include <SD.h>


// table for the number of clicks in every hour every month
const int data_pos_prg[12][24]PROGMEM = { {-1,-1,-1,-1,-1,-1,-1,-1,-1,69,162,268,376,480,574,657,-3,-3,-3,-1,-1,-1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-1,-1,62,153,259,367,472,567,650,-3,-3,-3,-3,-1,-1,-1,-1},
                                          {-1,-1,-1,-1,-1,-1,-1,-1, 6,69,162,268,376,480,574,656,-3,-3,-3,-3,-1,-1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-1,13,82,177,284,392,495,588,668,-3,-3,-3,-3,-3,-1,-1,-1},
                                          {-1,-1,-1,-1,-1,-1,-1,-2,16,88,184,292,400,502,594,673,-3,-3,-3,-3,-3,-1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-2,14,83,179,286,394,497,589,669,-3,-3,-3,-3,-3,-1,-1,-1},
                                          {-1,-1,-1,-1,-1,-1,-1,-2, 9,73,167,274,382,485,579,661,-3,-3,-3,-3,-3,-1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-2,10,75,170,276,384,488,581,662,-3,-3,-3,-3,-3,-1,-1,-1},
                                          {-1,-1,-1,-1,-1,-1,-1,-1,17,89,186,293,401,503,595,674,-3,-3,-3,-3,-1,-1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-1,26,103,203,311,418,519,608,685,-3,-3,-3,-1,-1,-1,-1,-1},
                                          {-1,-1,-1,-1,-1,-1,-1,-1,-1,104,204,312,420,520,610,686,-3,-3,-3,-1,-1,-1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-1,-1,98,197,305,412,513,604,681,-3,-3,-3,-1,-1,-1,-1,-1}};

                      

// table for the hour angle in every hour every month
const int data_hangle[12][24]PROGMEM = { {-2069,-1919,-1769,-1619,-1469,-1319,-1169,-1019,-869,-719,-569,-419,-269,-119,31,181,331,481,631,781,931,1081,1231,1381}, 
                           {-2082,-1932,-1782,-1632,-1482,-1332,-1182,-1032,-882,-732,-582,-432,-282,-132,18,168,318,468,618,768,918,1068,1218,1368},
                           {-2069,-1919,-1769,-1619,-1469,-1319,-1169,-1019,-869,-719,-569,-419,-269,-119,31,181,331,481,631,781,931,1081,1231,1381},
                           {-2047,-1897,-1747,-1597,-1447,-1297,-1147,-997,-847,-697,-547,-397,-247,-97,53,203,353,503,653,803,953,1103,1253,1403},
                           {-2036,-1886,-1736,-1586,-1436,-1286,-1136,-986,-836,-686,-536,-386,-236,-86,64,214,364,514,664,814,964,1114,1264,1414},
                           {-2036,-1886,-1736,-1586,-1436,-1286,-1136,-986,-836,-686,-536,-386,-236,-86,64,214,364,514,664,814,964,1114,1264,1414},
                           {-2061,-1911,-1761,-1611,-1461,-1311,-1161,-1011,-861,-711,-561,-411,-261,-111,39,189,339,489,639,789,939,1089,1239,1389},
                           {-2058,-1908,-1758,-1608,-1458,-1308,-1158,-1008,-858,-708,-558,-408,-258,-108,42,192,342,492,642,792,942,1092,1242,1392},
                           {-2034,-1884,-1734,-1584,-1434,-1284,-1134,-984,-834,-684,-534,-384,-234,-84,66,216,366,516,666,816,966,1116,1266,1416},
                           {-2010,-1860,-1710,-1560,-1410,-1260,-1110,-960,-810,-660,-510,-360,-210,-60,90,240,390,540,690,840,990,1140,1290,1440},
                           {-2008,-1858,-1708,-1558,-1408,-1258,-1108,-958,-808,-658,-508,-358,-208,-58,92,242,392,542,692,842,992,1142,1292,1442},
                           {-2018,-1868,-1718,-1568,-1418,-1268,-1118,-968,-818,-668,-518,-368,-218,-68,82,232,382,532,682,832,982,1132,1282,1432}};

// table for the zenith angle in every hour every month
const int data_zangle[12][24]PROGMEM = {{1516,1608,1636,1575,1471,1355,1236,1117,1001,891,790,702,633,591,581,605,658,736,830,935,1048,1165,1284,1403},
                          {1449,1528,1555,1512,1425,1316,1199,1080,961,846,738,641,563,512,497,522,580,664,764,874,990,1109,1228,1344},
                          {1371,1433,1448,1412,1336,1237,1125,1007,888,770,656,551,464,407,392,426,497,593,701,818,937,1056,1172,1279},
                          {1280,1325,1332,1297,1228,1137,1032,919,801,681,563,452,355,290,280,332,422,530,647,766,885,1000,1108,1204},
                          {1199,1236,1239,1207,1145,1062,963,854,739,621,501,384,277,199,193,263,367,483,603,722,837,947,1048,1134},
                          {1156,1192,1196,1168,1111,1033,939,833,721,604,484,366,252,162,149,226,336,454,574,691,805,913,1010,1093},
                          {1166,1206,1214,1189,1134,1056,962,856,743,626,507,388,275,185,161,227,333,449,569,687,803,912,1012,1099},
                          {1234,1279,1288,1258,1196,1111,1010,899,783,664,545,430,326,250,234,288,382,494,612,731,849,963,1068,1161},
                          {1341,1388,1391,1349,1272,1174,1064,947,828,709,594,488,400,346,343,391,476,580,694,813,932,1049,1160,1261},
                          {1454,1506,1500,1438,1344,1234,1117,997,879,764,658,565,494,457,461,506,582,678,787,902,1021,1140,1257,1365},
                          {1536,1603,1593,1514,1407,1291,1171,1053,937,829,730,646,585,554,558,597,664,751,853,963,1080,1199,1318,1433},
                          {1555,1633,1629,1546,1436,1318,1198,1080,967,860,763,681,621,590,591,626,688,771,869,976,1091,1209,1328,1446}};

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int pinSELA=0; // CD4051 pin select A
const int pinSELB=1; // CD4051 pin select B
const int pinIN1A=4; // motor driver direction 1 pin
const int pinIN2A=5; // motor driver direction 2 pin
const int pinD1A=6; // motor driver disable input pin (PWM)
const int pinSFA=7; // motor driver status flag
const int pinENA=8; // motor driver enable pin (sleep)
const int pinFBA=14; // motor driver current sensor feedback
const int pinCLKA=2; // reed switch sensor (INT0)
const int pinSENSE=1;// CD4051 output [sensor data]
const int pinJOYx=2; // thumb slide joystick x [A2]
const int pinJOYy=3; // thumb slide joystick y [A3]
const int pinEMERGENCY=3; // emergency button (INT1)
const int pinCS = 10;  // SD card chipselect


const int maxpos=700; // linear actuator maximal position in clicks
const int REEDTIME=1; // reed switch debounce time
const int BUTTONTIME=100; // emergency button debounce time
const int LCD_MAX_STATE=9; // maximum display modes
const int I_OVER=500; // overload current [mA]
const int DATALOG_INTERVAL=600; // datalogging interval [s] 
const int BASE_CONS=70; // base consumption [mA]
const int BAT_VOLTAGE=127; // battery voltage * 10 for KWh calculation

const char* status_text[] = {"SLEEP  ","STOP   ","START  ","MOVE   ","FAULT  ","MRGENCY","PARK   ","OVRLOAD", "EAST   ", "WEST   "}; // tracker status strings on display
String dataString="";

unsigned long inst_cons=0,total_cons=0,daily_cons=0; // instant, total and daily consumption

volatile int pos=0, newpos=0; // tracker position variables
volatile unsigned long clicktime, prevclicktime, BTNtime, prevBTNtime; // interrupt timers
volatile boolean state=1;
unsigned long looptime, prevmaintime, prevlcdtime, prevlcdchangetime; // mainloop timers
byte tracker_state=0, tracker_status=0, prevtracker_state; // tracker state indicators
boolean tracker_dir=0; // tracker direction
boolean sd_present=0;
boolean PV_inst_displaymode=0, PV_daily_displaymode=0;

int daystate=0, lcd_state=0; // mainloop status indicators
int EEPROM_pos, data_pos, ram_hangle, ram_zangle; // table progmem read
int PV1_inst=0, PV2_inst=0, PV1_instv=0, PV2_instv=0, datalog_reads=0, datalog_records=0;
byte PV1_lastavg=0, PV2_lastavg=0;
long PV1_avg=0, PV2_avg=0, PV1_daily=0, PV2_daily=0;

uint16_t startAddr = 0x0000;            // Start address to store in the NV-RAM
uint16_t lastAddr;                      // new address for storing in NV-RAM
uint16_t TimeIsSet = 0xaa55;            // Helper that time must not set again
uint8_t timezone;

/*void set_time()              // use this to set date and time once when RTC battery is replaced
{
    RTC.stopClock();
        
    RTC.fillByYMD(2012,6,22);
    RTC.fillByHMS(8,59,4);
    
    RTC.setTime();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
    RTC.startClock();
}*/

extern int  __bss_end; 
extern int  *__brkval; 

int memoryTest(){ 
  int free_memory; 
  if((int)__brkval == 0) 
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  else 
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  return free_memory; 
} 

void emergencybutton()          // emergency button interrupt (INT1)
{
        prevBTNtime = BTNtime;
        BTNtime=millis();

		if ((BTNtime-prevBTNtime) >= BUTTONTIME) 
		  if (tracker_state==7) {tracker_state=1; daystate=0;}   // overload reset
                  else 		
                    { if (tracker_state!=5) tracker_state=5; else { tracker_state=1; daystate=0; }}    // emergency stop and reset
}

void click()                  // reed switch interrupt (INT0)
{
        prevclicktime = clicktime;
        clicktime=millis();

		if ((clicktime-prevclicktime) >= REEDTIME) 
				if (tracker_dir) pos++; else pos--; // increase or decrease position according to direction

}



void setup()
{
  
  // initialize display and display version information
  lcd.init();                       
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Solar Tracker   ");
  lcd.setCursor(0,1);
  lcd.print("          v0.96b");
  
  looptime = prevmaintime = prevlcdtime=prevlcdchangetime=BTNtime=prevBTNtime= clicktime= millis(); // initialize timers
  delay(2000);
  
  lcd.setCursor(0,0);
  lcd.print("Free memory: "); 
  lcd.setCursor(0,1); 
  lcd.print("      "); 
  lcd.print(memoryTest());
  lcd.print(" bytes ");
  delay(2000);
  
  lcd.setCursor(0,0);
  lcd.print("Initializing    ");
  
  // initialize arduino pins
  lcd.setCursor(0,1);
  lcd.print("    arduino pins");
  pinMode(pinIN1A, OUTPUT);
  pinMode(pinIN2A, OUTPUT);
  pinMode(pinD1A, OUTPUT);
  pinMode(pinSFA, INPUT);
  pinMode(pinENA, OUTPUT);
  pinMode(pinFBA, INPUT);
  pinMode(pinCLKA, INPUT);
  pinMode(pinEMERGENCY, INPUT);
  pinMode(pinSELA, OUTPUT);
  pinMode(pinSELB, OUTPUT);
  pinMode(pinCS, OUTPUT);

  digitalWrite(pinSELB, LOW);
  delay(1000);

  // initialize interrupts
  lcd.setCursor(0,1);
  lcd.print("      interrupts");
  attachInterrupt(0, click, RISING);  
  attachInterrupt(1, emergencybutton, FALLING);
  delay(1000);
  
  // initialize RTC
  lcd.setCursor(0,1);
  lcd.print(" real time clock");
  RTC.getTime();
  delay(1000);
  
    
  
/*  // initialize SD
  lcd.setCursor(0,1);
  lcd.print("SD card:        ");

  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  lcd.setCursor(11,1);
  // see if the card is present and can be initialized:
  if (!SD.begin(pinCS)) { lcd.print("fail"); delay(1000); } 
   else { 
     lcd.print("ok");
     delay(1000);
    
     lcd.setCursor(0,1);
     lcd.print("datafile:       "); delay(500);
     lcd.setCursor(11,1);
  
     // open the file. note that only one file can be open at a time,
     // so you have to close this one before opening another.
     File dataFile = SD.open("datalog.txt", FILE_WRITE);

     // if the file is available, close it for now:
     if (dataFile) {
       dataFile.close(); lcd.print("ok"); sd_present=1;
     }  
     // if the file isn't open, pop up an error:
     else {
       lcd.print("fail");
     } // else
   } // else 
   delay(1000);*/
  
  tracker_state=0;
  tracker_init();
  

}


void tracker_init()
{    
  
  int limitswitch=0;
  lcd.setCursor(0,1);
  lcd.print(" position sensor");

  digitalWrite(pinENA, HIGH); // tracker stop
  digitalWrite(pinD1A, HIGH); 
 
  digitalWrite(pinIN1A, LOW); // tracker start
  digitalWrite(pinIN2A, HIGH);
 
   for (int i=255; i>0; i--) { //PWM start
    analogWrite(pinD1A, i);
    delay(4);
  }
   do
    {    
    inst_cons=analogRead(pinFBA);  // check if limitswitch clicked and stop only if it's off for a whole second
    if (inst_cons==0) limitswitch++; else limitswitch=0;
    delay(100);
  } while ((limitswitch<10) && (tracker_state!=5));    // go east until limitswitch switches off

  digitalWrite(pinD1A, HIGH);  // tracker stop
    
    if (tracker_state==5) { 
        lcd.setCursor(0,0);
        lcd.print(" EMERGENCY STOP ");
        lcd.setCursor(0,1);
        lcd.print(" Please RESTART ");
        do {} while(1);
    }
  
   else {pos=0;
        tracker_state=1; lcd.clear();}
}


void loop()
{ 
  int i;
  int x, y;
 
  
 looptime=millis();
  if ((looptime-prevmaintime)<0)   looptime = prevmaintime = prevlcdtime=prevlcdchangetime=BTNtime=prevBTNtime= clicktime= millis(); // initialize timers if millis overflows once every 50 days

  if ((looptime-prevmaintime)>100)    // do this every 100ms
  {
  if ((!(PINC & (1 << 4))) && (!(PINC & (1 << 3)))) tracker_state=4;    // fault condition if SF and D1 are both LOW
    
  
  x=analogRead(pinJOYx);                    // read the joystick
  y=analogRead(pinJOYy);                    
  if (y>600) {                        // joystick left (scroll display)
  if ((looptime-prevlcdchangetime)>500) { lcd_state--; prevlcdchangetime=looptime; }
  if (lcd_state<0) lcd_state=LCD_MAX_STATE;
  lcd.clear();
  }
  if (y<300) {                        // joystick right (scroll display)
  if ((looptime-prevlcdchangetime)>500) { lcd_state++; prevlcdchangetime=looptime; }
  if (lcd_state>LCD_MAX_STATE) lcd_state=0;
  lcd.clear();
  }
  if (x>600) {                        // joystick up, park if on right display 3, east if on right display 4, west if on display 5
  if (lcd_state==9)
   { 
     PV_inst_displaymode=!PV_inst_displaymode;
   }
  if (lcd_state==8)
   { 
     PV_daily_displaymode=!PV_daily_displaymode;
   }
   
  if (lcd_state==5) 
   { tracker_state=9;
      newpos=maxpos;
      digitalWrite(pinENA, HIGH);
      digitalWrite(pinD1A, HIGH);
    if (abs(pos-newpos)<2) tracker_state=9;
      else {
      if (pos<newpos) tracker_dir=1; else tracker_dir=0;
      if (tracker_dir)  {  
       digitalWrite(pinIN2A, LOW);
       digitalWrite(pinIN1A, HIGH);
      }
      else {  
       digitalWrite(pinIN1A, LOW);
       digitalWrite(pinIN2A, HIGH);
      }  
   
      for (int i=255; i>0; i--) {
       analogWrite(pinD1A, i);
      delay(4);
     } 
    }   
   }
  if (lcd_state==4) 
   { tracker_state=8;
      newpos=0;
      digitalWrite(pinENA, HIGH);
      digitalWrite(pinD1A, HIGH);
     if (abs(pos-newpos)<2) tracker_state=8;
      else {
      if (pos<newpos) tracker_dir=1; else tracker_dir=0;
      if (tracker_dir)  {  
       digitalWrite(pinIN2A, LOW);
       digitalWrite(pinIN1A, HIGH);
      }
      else {  
       digitalWrite(pinIN1A, LOW);
       digitalWrite(pinIN2A, HIGH);
      }  
   
      for (int i=255; i>0; i--) {
       analogWrite(pinD1A, i);
      delay(4);
     }    
    }
   }
  if (lcd_state==3) 
   { tracker_state=6;
     newpos=maxpos/2;  
     digitalWrite(pinENA, HIGH);
     digitalWrite(pinD1A, HIGH);
 
     if (abs(pos-newpos)<2) tracker_state=6;
      else {
      if (pos<newpos) tracker_dir=1; else tracker_dir=0;
      if (tracker_dir)  {  
       digitalWrite(pinIN2A, LOW);
       digitalWrite(pinIN1A, HIGH);
      }
      else {  
       digitalWrite(pinIN1A, LOW);
       digitalWrite(pinIN2A, HIGH);
      }  

      for (int i=255; i>0; i--) {
       analogWrite(pinD1A, i);
      delay(4);
     }    
    }
   }
  }
  
  
  if (x<300) {                            // joystick down, unpark if on right display 3, west or reset if on right display 4
    if (((lcd_state==3) && (tracker_state==6)) || ((lcd_state==4) && (tracker_state==8)) || ((lcd_state==5) && (tracker_state==9))) { tracker_state=1; daystate=0; }
    if (lcd_state==9) {  PV_inst_displaymode=!PV_inst_displaymode; }
    if (lcd_state==8) {  PV_daily_displaymode=!PV_daily_displaymode; }
  }
  
  
  prevmaintime=looptime;
  
  if ((tracker_state!=prevtracker_state) || (tracker_state==3) || (tracker_state==6)) {
  prevtracker_state=tracker_state;
  switch (tracker_state) {
    case 0:                                 // sleep
      digitalWrite(pinENA, LOW);
      break;
    case 1:                                 // stop
      digitalWrite(pinENA, HIGH);
      digitalWrite(pinD1A, HIGH);
      break;
    case 2:                                // start
      if (abs(pos-newpos)<2) tracker_state=1;
      else {
      if (pos<newpos) tracker_dir=1; else tracker_dir=0;
      if (tracker_dir)  {  
       digitalWrite(pinIN2A, LOW);
       digitalWrite(pinIN1A, HIGH);
      }
      else {  
       digitalWrite(pinIN1A, LOW);
       digitalWrite(pinIN2A, HIGH);
      }  
   
      for (int i=255; i>0; i--) {
       analogWrite(pinD1A, i);
      delay(4);
     }
     tracker_state=3;}
     break;
   case 3:                              // move
     if (abs(pos-newpos)<2) 
           tracker_state=1;
   /*  else
       if (inst_cons==0) { if (abs(pos)<10) { pos=0; tracker_state=1;}        // to check at east if position resets to zero
                             if (abs(pos-maxpos)<10) tracker_state=1;}  */      
     break;
   case 4:                              // fault
       digitalWrite(pinENA, LOW);
       digitalWrite(pinD1A, HIGH);
     break;
   case 5:                              // emergency
       digitalWrite(pinENA, LOW);
       digitalWrite(pinD1A, HIGH);
     break;
   case 6:                              // park
     if (abs(pos-newpos)<2) 
      digitalWrite(pinD1A, HIGH);
      digitalWrite(pinENA, HIGH);
     break;
   case 7:                              // overload
       digitalWrite(pinENA, LOW);
       digitalWrite(pinD1A, HIGH);
     break;
   case 8:                              // East
     if (abs(pos-newpos)<2)             
      digitalWrite(pinD1A, HIGH);
      digitalWrite(pinENA, HIGH);
     break;
   case 9:                              // west
     if (abs(pos-newpos)<2) 
      digitalWrite(pinD1A, HIGH);
      digitalWrite(pinENA, HIGH);
     break;
   } //endswitch
  } //endif
  } //endif
 
  
  if ((looptime-prevlcdtime) > 1000) {     // do this every second

  digitalWrite(pinSELA, LOW);     // select CD4051 PV1 sensor
  PV1_instv=analogRead(pinSENSE);  // read current sensor
  digitalWrite(pinSELA, HIGH);    // select CD4051 PV2 sensor
  PV2_instv=analogRead(pinSENSE);  // read current sensor
  PV1_inst = -10*((PV1_instv*(5.00/1024))- 2.5)/ .04 - 3;
  PV2_inst = -10*((PV2_instv*(5.00/1024))- 2.5)/ .04 - 3;
  if (abs(PV1_inst)<2) PV1_inst=0;
  if (abs(PV2_inst)<2) PV2_inst=0;
   
  PV1_avg+=PV1_inst;
  PV2_avg+=PV2_inst;
  datalog_reads++;
  
  if (datalog_reads >= DATALOG_INTERVAL) {
      PV1_lastavg=byte(PV1_avg/datalog_reads);
      PV2_lastavg=byte(PV2_avg/datalog_reads);
      PV1_daily+=PV1_avg;
      PV2_daily+=PV2_avg;
      PV1_avg=PV2_avg=datalog_reads=0;  
           
      datalog_records++;

   } // endif 
   
  
  RTC.getTime();                            // read RTC
  timezone = RTC.isMEZSummerTime();         // read summertime
  
  if ((RTC.hour==0) && (RTC.minute==0))  {  // reset daily values at midnight
    daily_cons=PV1_daily=PV2_daily=0;
  } // endif  
  
  // read and map instant consumption
  inst_cons=analogRead(pinFBA);
  inst_cons=map(inst_cons, 0, 1023, 0, 5000);
  inst_cons*=1000/525;
  
  if (inst_cons>I_OVER) tracker_state=7; // if consumption is higher than I_OVER limit current go to overload state
    
  total_cons+=(inst_cons+BASE_CONS)*(looptime-prevlcdtime)/1000;   // calculate total consumption
  daily_cons+=(inst_cons+BASE_CONS)*(looptime-prevlcdtime)/1000;   // calculate daily consumption
  
  
  prevlcdtime=looptime;
  
  data_pos=pgm_read_word(&data_pos_prg[RTC.month-1][RTC.hour-timezone]);



  if (tracker_state < 4) 
   switch (data_pos) {
    case -1:                                                                           // night
      if (daystate!=-1) { tracker_state=2; newpos=maxpos/2; daystate=-1; }
      break;
    case -2:                                                                          // sunrise
      if (daystate!=-2) { tracker_state=2; newpos=0; daystate=-2; }
      break;
    case -3:                                                                           // sunset
      if (daystate!=-3) { tracker_state=2; newpos=maxpos; daystate=-3; }
      break;  
    case 0:                                                                            // reset
      daystate=data_pos;
  
      break;
    default:                                                                           // daytime
      if (daystate!=data_pos) { tracker_state=2; newpos=data_pos; daystate=data_pos; }
      
  } //endswitch
  
  
switch (lcd_state) {
  case  0:                                  // tracker status date and time
   lcd.setCursor(0,0);
   lcd.print(status_text[tracker_state]);
   lcd.setCursor(0,1);
   switch (daystate) { 
     case -1:
       lcd.print("NIGHT");
       break;
     case -2:
       lcd.print("SRISE");
       break;
     case -3:
       lcd.print("SSET ");
       break;
     default:
       lcd.print("DAY  ");
       break;
   } //endswitch
   
  
   lcd.setCursor(8,0);
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
  
   
   lcd.setCursor(6,1);
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
    lcd.print(RTC.year, DEC);   
break;

case 1:                                                // current position, next position, zenith angle, hour angle
    lcd.setCursor(0,0);
     lcd.print("CP:");
     if (pos<100) lcd.print(" ");
     if (pos<10) lcd.print(" ");
     lcd.print(pos);
     
     lcd.setCursor(0,1);
     lcd.print("NP:");
     if (newpos<100) lcd.print(" ");
     if (newpos<10) lcd.print(" ");
     lcd.print(newpos);
     
     lcd.setCursor(7,0);
     lcd.print("ZA: ");
//     ram_zangle=pgm_read_word(&data_zangle[RTC.month-1][RTC.minute % 24]);
     ram_zangle=pgm_read_word(&data_zangle[RTC.month-1][RTC.hour-timezone]);

     if (abs(ram_zangle/10)<100) lcd.print(" ");
     if (abs(ram_zangle/10)<10) lcd.print(" ");
          
     lcd.print(abs(ram_zangle/10));
     lcd.print (".");
     
     lcd.print (abs(ram_zangle%10));
    
  
   lcd.setCursor(7,1);
     lcd.print("HA:");
 //    ram_hangle=pgm_read_word(&data_hangle[RTC.month-1][RTC.minute % 24]);
     ram_hangle=pgm_read_word(&data_hangle[RTC.month-1][RTC.hour-timezone]);
 
     if ((ram_hangle/10)>0) lcd.print(" ");
     if (abs(ram_hangle/10)<100) lcd.print(" ");
     if (abs(ram_hangle/10)<10) lcd.print(" ");
     if ((ram_hangle/10)<0) lcd.print("-");
     
     lcd.print(abs(ram_hangle/10));
     lcd.print (".");
     
     lcd.print (abs(ram_hangle%10));
     break;
     
 case 2:                                          // instant and total consumption
   lcd.setCursor(0,0);
   lcd.print("Instant   Total");
   lcd.setCursor(0, 1);
     if (inst_cons+BASE_CONS<1000) lcd.print(" ");
     if (inst_cons+BASE_CONS<100) lcd.print(" ");
     if (inst_cons+BASE_CONS<10) lcd.print(" ");
       lcd.print(inst_cons+BASE_CONS);
       lcd.print("mA ");  
   lcd.setCursor(8, 1);
   if (total_cons<100000) {
     if (total_cons<10000) lcd.print(" ");
     if (total_cons<1000) lcd.print(" ");
     if (total_cons<100) lcd.print(" ");
     if (total_cons<10) lcd.print(" ");
       lcd.print(total_cons);
       lcd.print("mAs");  } 
   else if (total_cons<360000000) {
     if (total_cons/3600<10000) lcd.print(" ");
     if (total_cons/3600<1000) lcd.print(" ");
     if (total_cons/3600<100) lcd.print(" ");
     if (total_cons/3600<10) lcd.print(" ");
       lcd.print(total_cons/3600);
       lcd.print("mAh");  } 
    else {
     if (total_cons/3600000<10000) lcd.print(" ");
     if (total_cons/3600000<1000) lcd.print(" ");
     if (total_cons/3600000<100) lcd.print(" ");
     if (total_cons/3600000<10) lcd.print(" ");
       lcd.print(total_cons/3600000);
       lcd.print("Ah");  } 
       
   break;
 
 case 3:                                      // park
  if (tracker_state!=6) {
   lcd.setCursor(0,0);
   lcd.print(" press joystick ");
   lcd.setCursor(0,1);
   lcd.print(" up to P A R K  "); }
  else {
   lcd.setCursor(0,0);
   lcd.print(" press joystick ");
   lcd.setCursor(0,1);
   lcd.print(" down to UNPARK "); }
 break;
 
  case 4:                                      // east 
      if (tracker_state!=8) {
         lcd.setCursor(0,0);
         lcd.print(" press joystick ");
         lcd.setCursor(0,1);
         lcd.print(" up to E A S T  "); }
      else {
         lcd.setCursor(0,0);
         lcd.print(" press joystick ");
         lcd.setCursor(0,1);
         lcd.print(" down to RESET  "); }
       break;
  case 5:                                      // west
      if (tracker_state!=9) {
         lcd.setCursor(0,0);
         lcd.print(" press joystick ");
         lcd.setCursor(0,1);
         lcd.print(" up to W E S T  ");}
      else {
         lcd.setCursor(0,0);
         lcd.print(" press joystick ");
         lcd.setCursor(0,1);
         lcd.print(" down to RESET  "); }
       break;
  case 6:
      lcd.setCursor(0,0);
      lcd.print(" debug D15-D0:  ");
      lcd.setCursor(0,1);
        for (i=8; i--; i>=0) 
          if (!(PINB & (1 << i))) lcd.print("0"); else lcd.print("1");
      for (i=8; i--; i>=0) 
          if (!(PIND & (1 << i))) lcd.print("0"); else lcd.print("1");
      break;
  case 7:
      lcd.setCursor(0,0);
      if (sd_present) lcd.print("SD card present "); else lcd.print("No SD card      ");
      lcd.setCursor(3,1);
      if (datalog_records<1000) lcd.print(" ");  if (datalog_records<100) lcd.print(" "); if (datalog_records<10) lcd.print(" ");
      lcd.print(datalog_records); lcd.print(" records");    
      break;
  case 8:
  
      lcd.setCursor(0,0);
      lcd.print("PV1DAY: ");
      if (!PV_daily_displaymode) {
       if (PV1_daily<0) lcd.print("RECAL "); else {  // if daily total is negative sensor needs recalibrating
        if ((PV1_daily/36000)<100) lcd.print(" ");  if ((PV1_daily/36000)<10) lcd.print(" ");
        lcd.print(PV1_daily/36000); lcd.print("."); lcd.print((PV1_daily/3600)%10); lcd.print("Ah "); } // print daily value
      } else {
       lcd.print(PV1_daily*BAT_VOLTAGE/360000000); lcd.print(".");
       lcd.print((PV1_daily*BAT_VOLTAGE/36000000)%10);
       lcd.print((PV1_daily*BAT_VOLTAGE/3600000)%10); lcd.print("kWh");       
      }
      
      lcd.setCursor(0,1);
      lcd.print("PV2DAY: ");
      if (!PV_daily_displaymode) {
       if (PV2_daily<0) lcd.print("RECAL "); else {  // if daily total is negative sensor needs recalibrating
        if ((PV2_daily/36000)<100) lcd.print(" ");  if ((PV2_daily/36000)<10) lcd.print(" ");
        lcd.print(PV2_daily/36000); lcd.print("."); lcd.print((PV2_daily/3600)%10); lcd.print("Ah "); } // print daily value
      } else {
       lcd.print(PV2_daily*BAT_VOLTAGE/360000000); lcd.print(".");
       lcd.print((PV2_daily*BAT_VOLTAGE/36000000)%10);
       lcd.print((PV2_daily*BAT_VOLTAGE/3600000)%10); lcd.print("kWh");       
      }
         
      break;  
      
  case 9:
  
      lcd.setCursor(0,0);
      lcd.print("PV1: ");
      if (!PV_inst_displaymode) {  // display in Amps
       if (PV1_inst<0) lcd.print("RECAL "); else {  // if current is negative sensor needs recalibrating
        if ((PV1_inst/10)<10) lcd.print(" "); lcd.print(PV1_inst/10); lcd.print("."); lcd.print(PV1_inst%10); lcd.print("A "); } // print instantaneous value [1sec average]
       if (PV1_lastavg<0) lcd.print("RECAL"); else {
        if ((PV1_lastavg/10)<10)  lcd.print(" "); lcd.print(PV1_lastavg/10);  lcd.print("."); lcd.print(PV1_lastavg%10);  lcd.print("A"); } // print average over last DATALOG_INTERVAL time
      } else { // display sensor value [0-1023]
       if (PV1_instv<1000) lcd.print(" "); if (PV1_instv<100) lcd.print(" "); if (PV1_instv<10) lcd.print(" "); lcd.print(PV1_instv); lcd.print("  "); }
        
      lcd.setCursor(0,1);
      lcd.print("PV2: ");  // do the same for second sensor
      if (!PV_inst_displaymode) {  // display in Amps
       if (PV2_inst<0) lcd.print("RECAL "); else {
        if ((PV2_inst/10)<10) lcd.print(" "); lcd.print(PV2_inst/10); lcd.print("."); lcd.print(PV2_inst%10); lcd.print("A "); }
       if (PV2_lastavg<0) lcd.print("RECAL"); else {
        if ((PV2_lastavg/10)<10)  lcd.print(" "); lcd.print(PV2_lastavg/10);  lcd.print("."); lcd.print(PV2_lastavg%10);  lcd.print("A"); }
      } else {
       if (PV2_instv<1000) lcd.print(" "); if (PV2_instv<100) lcd.print(" "); if (PV2_instv<10) lcd.print(" "); lcd.print(PV2_instv); lcd.print("  "); }         
      
      break;
} //endswitch
} //endif 1sec
} // endloop


