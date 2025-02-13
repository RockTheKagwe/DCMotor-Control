/* This code controls a 3-phase energy meter using CT sensors.
 *  An ESP32 is used for the monitoring.
 *  It calculates RMS current, power, peak power, and energy consumption (kWh).
 *  Data is displayed on an LCD, updating every 3 seconds.   
*/

#include <LiquidCrystal_I2C.h>

int currentPins[3] = {25,26,27};              //Assign phase CT inputs to analog pins
double calib[3] = {11.8337,11.8234,12.0325};
double kilos[3];
unsigned long startMillis[3];
unsigned long endMillis[3];
double RMSCurrent[3];
int RMSPower[3];
int peakPower[3];

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() 
{ 
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
  lcd.clear();
  lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
  lcd.print("3 Phase");
  lcd.setCursor(0,1);
  lcd.print("Energy Meter");
  delay(2000);
}

void readPhase ()      //Method to read information from CTs
{
  for(int i=0;i<=2;i++)
  {
    int current = 0;
    int maxCurrent = 0;
    int minCurrent = 1000;
    for (int j=0 ; j<=200 ; j++)  //Monitors and logs the current input for 200 cycles to determine max and min current
    {
      current =  analogRead(currentPins[i]);    //Reads current input and records maximum and minimum current
      if(current >= maxCurrent)
        maxCurrent = current;
      else if(current <= minCurrent)
        minCurrent = current;
    }
    if (maxCurrent <= 2047)
    {
      maxCurrent = 2046;
    }
    RMSCurrent[i] = ((maxCurrent - 2046)*0.707)/calib[i];    //Calculates RMS current based on maximum value and scales according to calibration
    RMSPower[i] = 220*RMSCurrent[i];    //Calculates RMS Power Assuming Voltage 220VAC, change to 110VAC accordingly
    if (RMSPower[i] > peakPower[i])
    {
      peakPower[i] = RMSPower[i];
    }
    endMillis[i]= millis();
    unsigned long time = (endMillis[i] - startMillis[i]);
    kilos[i] = kilos[i] + ((double)RMSPower[i] * ((double)time/60/60/1000000));    //Calculate kilowatt hours used
    startMillis[i]= millis();
  }
}

void loop()   //Calls the methods to read values from CTs and changes display
{
  readPhase();
  displayKilowattHours ();
  delay(3000);
  readPhase();
  displayCurrent ();
  delay(3000);
  readPhase();
  displayRMSPower ();
  delay(3000);
  readPhase();
  displayPeakPower ();
  delay(3000);
}

void displayKilowattHours ()  //Displays all kilowatt hours data
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(kilos[0]);
  lcd.print("kWh");
  lcd.setCursor(9,0);
  lcd.print(kilos[1]);
  lcd.print("kWh");
  lcd.setCursor(0,1);
  lcd.print(kilos[2]);
  lcd.print("kWh");
  lcd.setCursor(9,1);
  lcd.print("Energy");
}

void displayCurrent ()      //Displays all current data
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(RMSCurrent[0]);
  lcd.print("A");
  lcd.setCursor(9,0);
  lcd.print(RMSCurrent[1]);
  lcd.print("A");
  lcd.setCursor(0,1);
  lcd.print(RMSCurrent[2]);
  lcd.print("A");
  lcd.setCursor(9,1);
  lcd.print("Current");
}

void displayRMSPower ()     //Displays all RMS power data
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(RMSPower[0]);
  lcd.print("W");
  lcd.setCursor(9,0);
  lcd.print(RMSPower[1]);
  lcd.print("W");
  lcd.setCursor(0,1);
  lcd.print(RMSPower[2]);
  lcd.print("W");
  lcd.setCursor(9,1);
  lcd.print("Power");
}

void displayPeakPower ()    //Displays all peak power data
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(peakPower[0]);
  lcd.print("W");
  lcd.setCursor(9,0);
  lcd.print(peakPower[1]);
  lcd.print("W");
  lcd.setCursor(0,1);
  lcd.print(peakPower[2]);
  lcd.print("W");
  lcd.setCursor(9,1);
  lcd.print("Max Pwr");
}
