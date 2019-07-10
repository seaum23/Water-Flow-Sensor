/*
YFâ€� S201 Water Flow Sensor
Water Flow Sensor output processed to read in litres/hour
Adaptation Courtesy: www.hobbytronics.co.uk
*/
#include <LiquidCrystal.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned int estimateAvg = 250;
unsigned int avarageValue = 0;
unsigned int avarageCount = 0;
unsigned int avarageResult = 0;

unsigned int checkTime = 10;
unsigned int checkTimeCount = 1;
unsigned int hour = 0;
unsigned int returnValue = 0;
unsigned int secondInput = 0;
unsigned int initialValue;
const int ledPin =  13;
const int ledPin1 =  12;
volatile int flow_frequency1;
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned char flowsensor1 = 3;
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
   secondInput = 1;
}
void flow1 () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
   // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
   
   pinMode(flowsensor, INPUT);
   pinMode(ledPin, OUTPUT);
   pinMode(ledPin1, OUTPUT);
   pinMode(flowsensor1, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(0, flow,CHANGE); // Setup Interrupt
   sei(); // Enable interrupts                                                                                
   currentTime = millis();
   cloopTime = currentTime;
}
void loop ()
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      lcd.clear();
      hour += (millis()/1000)/60/60;
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.print(" L/hour   ");
      Serial.println(initialValue);
      if(l_hour > 0 && initialValue > 0 && secondInput != 1){
        avarageValue = avarageValue + l_hour;
        avarageCount+=1;
        avarageResult = avarageValue / avarageCount;
        }
      if(secondInput == 1 && l_hour > 0){
        returnValue = returnValue + 1;
        }
        if((millis()/1000) > (checkTime * checkTimeCount)){
            checkTimeCount+=1;
         }
        if((millis()/1000) <= (checkTime * checkTimeCount) && l_hour > estimateAvg && l_hour > 0 && initialValue > 0){
          lcd.setCursor(0,0);
          lcd.print("You took more");
          lcd.setCursor(0,1);
          lcd.print("water");
          }else{
            if(l_hour < initialValue && l_hour > (initialValue-200) && l_hour > 0 && initialValue > 0){
                lcd.setCursor(0,0);
                lcd.print("Waterflow Normal");
                lcd.setCursor(0,1);
                lcd.print(l_hour);
                lcd.setCursor(5,1);
                lcd.print("l/H");
                lcd.setCursor(9,1);
                lcd.print(avarageResult);
              }else if(l_hour > initialValue && initialValue > 0 && l_hour > 0){
                  lcd.setCursor(0,0);
                  lcd.print("OVERFLOW");
                  lcd.setCursor(0,1);
                  lcd.print(l_hour);
                  lcd.setCursor(5,1);
                  lcd.print("l/H");
                  lcd.setCursor(9,1);
                  lcd.print(avarageResult);
                }else if(l_hour == 0 && initialValue > 0){
                    lcd.setCursor(0,0);
                    lcd.print("NO FLOW");
                    lcd.setCursor(0,1);
                    lcd.print(l_hour);
                    lcd.setCursor(5,1);
                    lcd.print("l/H");
                    lcd.setCursor(9,1);
                    lcd.print(avarageResult);
                  }else if(l_hour < (initialValue-200) && l_hour > 0 && initialValue > 0){
                      lcd.setCursor(0,0);
                      lcd.print("LEAK");
                      lcd.setCursor(0,1);
                      lcd.print(l_hour);
                      lcd.setCursor(5,1);
                      lcd.print("l/H");
                      lcd.setCursor(9,1);
                      lcd.print(avarageResult);
                    }
        }
      if(l_hour > 100 && secondInput == 1 && returnValue == 3){
            initialValue = l_hour;
            detachInterrupt(0);
            attachInterrupt(1, flow1, RISING);
            secondInput = 2;
         }
   }
}

