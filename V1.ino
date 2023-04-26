#include "OLED_Driver.h"
#include "GUI_paint.h"
#include "DEV_Config.h"
#include "drawGUI.h"
#include "setupScreen.h"
#include <SPI.h>
#include "Adafruit_MAX31855.h"

//define thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

//define program variables
int modeNum = 0; //screen mode 0 for temp, 1 for timer and 2 for wattage if its added
int enableSetting = 0;
long timeButtonPressed;
long timeButtonLastPressed;

//initialize time mode tracking variables
int hours = 0;
int minutes = 0;
unsigned long prevTime = 0;
unsigned long elapsedMS; //count of elapsed Milliseconds since the timer was last set

//temperature mode tracking variables
long temperatureButtonStartTime = 0;
int degreeMode = 0; //0 for celcius and 1 for fahreneheit
int degreeModeSwitched = 0;
float targetTemp = 100.0;   //Target Temperature in fahreneheit
float currentTemp = 0;   //this is the temperature right next to the element taken by the thermocouple for control of the element, in fahreneheit

int heatOn = 0; //variable that controls whether or not the heating element is activated

int avgPointer = -1;
float prevAverage = 0;
float temp = 0;
float total = 0;

int largestGap = 5; //the gap between the previous average and the new value being fed in to determine outlier garbage values
int initialCount = 0; //once this value is equal to the AVGCOUNT then we start checking for garbage values
float temps[AVGCOUNT]; //needs the same amount of space as avgCount

int hysteresisLow = 0.2; //the degrees in fahreneheit that once reached below the targetTemp, the heating element turns on
int hysteresisHigh = 0.2; //the degrees in fahreneheit that once reached above the targetTemp, the heating element turns off
int tuningDegrees = -0.5; //the degrees that the immersion circulator's target average is typically off by, does not affect displayed temp measurement
//wattage mode variable tracking (potential future feature)
int volume; //input through the app
int current; //would need to be measured via some internal or external device, could measure either total power or just heating element power


//the interrupt for when the temperature button is pressed
void IRAM_ATTR tempButton(){
  //validate the button press
  if(!validateButtonPress(TEMPBUTTONPIN)){
    //not valid, return
    return;
  }
  
  //Serial.print("temp button pushed\n");

  ////check which mode/screen we are on
  if(modeNum == 0){
    //we are already on the temperature screen, switch between celsius and fahreneheit
    if(degreeMode == 0){
      //currently in celsius, switch to fahreneheit
      degreeMode = 1;

      //temp in celsius now so we have to adjust our setting mode because our temp never gets hot enough to reach the third digit of celsius
      if(enableSetting > 2){
        enableSetting = 2;
      }
    }else{
       //currently in fahreneheit, switch to celsius
      degreeMode = 0;
    }
 
   }else{
    //we arent in temperature mode already, just switch to temperature mode and disable setting
      modeNum = 0;
      enableSetting = 0; 
    }    
}

void IRAM_ATTR timerButton(){
 
  //validate the button press
  if(!validateButtonPress(TIMERBUTTONPIN)){
    //not valid, return
    return;
  }
  
  //Serial.print("timer button pushed\n");
  if(modeNum != 1){
    modeNum = 1;
    enableSetting = 0; 
  }
}

void IRAM_ATTR wattageButton(){

  //validate the button press
  if(!validateButtonPress(WATTAGEBUTTONPIN)){
    //not valid, return
    return;
  }
  
  //Serial.print("wattage button pushed\n");
  if(modeNum != 2){
    modeNum = 2;
    enableSetting = 0; 
   
  }
}

void IRAM_ATTR setButton(){
  //validate the button press
  if(!validateButtonPress(SETBUTTONPIN)){
    //not valid, return
    return;
  }

  //Serial.printf("set button pushed: %d\n", enableSetting);
  if(modeNum == 0){
    //temperature mode
     if(enableSetting < 2){
      enableSetting += 1;
    }else if(degreeMode == 1 && enableSetting < 3){
      // if in fahreneheit, we can have up to 3 places to set values
      enableSetting += 1;
    }else{
      //we have reached the max setting place for temperature depending on the degree mode, and we loop back around to "not setting"
      enableSetting = 0;
    }
   
  }else if(modeNum == 1){
    //timer mode, we have 4 places where setting can take place before looping back to "not setting"
    if(enableSetting < 4){
      enableSetting += 1;
    }else{ 
      //reset the elapsed milliseconds while we are setting the timer, if we are setting a time, it should not be counting down anyway
      elapsedMS = 0;
      enableSetting = 0;
    }
    
  }
}

void IRAM_ATTR upButton(){
  //validate the button press
  if(!validateButtonPress(UPBUTTONPIN)){
    //not valid, return
    return;
  }

  //Serial.print("up button pushed\n");
  if(modeNum == 1){
    //working with the timer
    if(enableSetting == 1){
      //working on timer minutes ones digit
      minutes = minutes + 1;
    }
    if(enableSetting == 2){
      //working on minutes tens digit
      minutes = minutes + 10;
      
    }
    if(enableSetting == 3){
      //working on hours ones digit
      hours = hours +1;
    }
    if(enableSetting == 4){
      //working on hours tens digit
      hours = hours +10;
    }
   
    if(hours > 99){
      hours = 0;
    }
    if(minutes > 99){
      minutes = 0;
    }
  }else if(modeNum == 0){
    //working with temperature
     if(enableSetting == 3){
      //working on hundreds digit
     if(degreeMode){
         targetTemp += 100.0;
      }else{
        //unreachable because in degrees celsius we cant access the hundreds place
      }
    }
    if(enableSetting == 2){
      //working on tens digit
      if(degreeMode){
         targetTemp += 10.0;
      }else{
         targetTemp += 18.0;
      }
    }
    if(enableSetting == 1){
      //working on  ones digit
      if(degreeMode){
         targetTemp += 1.0;
      }else{
         targetTemp += 1.8;
      }
    }
    if(targetTemp > 203.0){
        targetTemp = 95.0;
    }
  }
}
void IRAM_ATTR downButton(){
  //validate the button press
  if(!validateButtonPress(DOWNBUTTONPIN)){
    //not valid, return
    return;
  }
  
  //Serial.print("down button pushed\n");
  if(modeNum == 1){
    //working with the timer
    if(enableSetting == 4){
      //working on timer hours tens digit
      hours = hours - 10;
    }
    if(enableSetting == 3){
      //working on hours ones digit
      hours = hours - 1;
    }
    if(enableSetting == 2){
      //working on minutes tens digit
      minutes = minutes - 10;
    }
    if(enableSetting == 1){
      //working on minutes tens digit
      minutes = minutes - 1;
    }

   //loop back around if we go less than 99
    if(hours < 0){
      hours = 99;
    }
    if(minutes < 0){
      minutes = 99;
    }
    
  }else if(modeNum == 0){
     if(enableSetting == 3){
      //working on hundreds digit
      if(degreeMode){
         targetTemp -= 100.0;
      }else{
        //unreachable because in degrees celsius we cant access the hundreds place
      }
    }
    if(enableSetting == 2){
      //working on tens digit
      if(degreeMode){
         targetTemp -= 10.0;
      }else{
         targetTemp -= 18.0;
      }
    }
    if(enableSetting == 1){
      //working on ones digit
      if(degreeMode){
         targetTemp -= 1.0;
      }else{
         targetTemp -= 1.8;
      }
      Serial.println(targetTemp);
    } 
     if(targetTemp < 95.0){
        targetTemp = 203.0;
     }
  }
}

float avgTemp(float nextCurrentTemp){
 if(initialCount < AVGCOUNT){
    initialCount++;
 }else{
  if(abs(nextCurrentTemp - prevAverage) > largestGap || isnan(nextCurrentTemp)){
    Serial.printf("Measurement is out of range for largest gap, throwing out value and using previous average: %f\n", prevAverage); 
      return prevAverage;
    }
 }

  avgPointer = ++avgPointer;
  if(avgPointer == AVGCOUNT){
    avgPointer = 0;
  }

  temps[avgPointer] = nextCurrentTemp;
  total = 0;
  for(int i = 0; i < AVGCOUNT; i++){
    total += temps[i];
  }
  
  prevAverage = total / AVGCOUNT;
  Serial.printf("Average table: %d ", avgPointer);
  return total / AVGCOUNT;
}

bool validateButtonPress(int buttonPin){
   //read if the pin is low, if it is, return false, the button is not pressed and this is a false positive
  if(digitalRead(buttonPin)== LOW){
    return false;
  }

  //check to see if a button was pressed within the last 250 milliseconds, or a quarter of a second
  timeButtonPressed = millis();
  if(timeButtonPressed - timeButtonLastPressed < 250){
    //if it has, return, this is a false positive as a button was pressed in the last 250 milliseconds, which would indicate bounce, not a true press
    return false;
  }else{
    //else this is a true positive, the button is pressed and the time that the button was pressed is the new time to start checking from
    timeButtonLastPressed = timeButtonPressed;
    return true;
  }
}

void setup() {
  System_Init();

  setUpDisplay();
 //set up pins
  pinMode(TEMPBUTTONPIN, INPUT); //temperature button
  pinMode(TIMERBUTTONPIN, INPUT); //timer button
  pinMode(WATTAGEBUTTONPIN, INPUT); //wattage button
  pinMode(SETBUTTONPIN, INPUT); //set button
  pinMode(UPBUTTONPIN, INPUT); //up button
  pinMode(DOWNBUTTONPIN, INPUT); //down button
  pinMode(HEATERPIN, OUTPUT); //the element pin

  attachInterrupt(TEMPBUTTONPIN, tempButton, RISING);
  attachInterrupt(TIMERBUTTONPIN, timerButton, RISING);
  attachInterrupt(WATTAGEBUTTONPIN, wattageButton, RISING);
  attachInterrupt(SETBUTTONPIN, setButton, RISING);
  attachInterrupt(UPBUTTONPIN, upButton, RISING);
  attachInterrupt(DOWNBUTTONPIN, downButton, RISING);

  timeButtonLastPressed = millis();

  if (!thermocouple.begin()) {
    Serial.println("Thermocouple ERROR.");
  }
}

void loop() {

   currentTemp = thermocouple.readFahrenheit();
   if (isnan(currentTemp) || currentTemp < 0.0 || currentTemp > 200.0) {
     Serial.println("Thermocouple fault(s) detected!");
     Serial.printf("Thermocouple had fault or read out of range. Reading in fahrenheit: %f\n", currentTemp); 
     uint8_t e = thermocouple.readError();
     if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
     if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND new.");
     if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC new.");
   }

  temp = avgTemp(currentTemp + tuningDegrees);
  Serial.printf("averaged: %f measured: %f\n", temp, currentTemp + tuningDegrees); //print the averaged temp, for debugging purposes
  
  if(temp + hysteresisLow < targetTemp && heatOn == 0){
    //Serial.println("heaterPin on");
    heatOn = 1;
    analogWrite(HEATERPIN, 255);
  }

  if(temp - hysteresisHigh > targetTemp && heatOn == 1){
    //Serial.println("heaterPin off");
    heatOn = 0;
    analogWrite(HEATERPIN, 0);
  }
  
  if(modeNum == 0){
    //draw temperature screen
    if(enableSetting > 0){
      //draw target temperature screen
      drawTargetTemp(targetTemp, degreeMode, enableSetting);
    }else{
      //draw current temperature screen
       drawCurrentTemp(temp, degreeMode);
    }
  }else if(modeNum == 1){
    //draw timer screen
    drawTimer(hours, minutes, enableSetting);
  }else if(modeNum == 2){
    //draw wattage screen
  }

  //if not in the condition of setting the time and being in the time mode
  if((enableSetting == 0 || modeNum != 1) && minutes > -1){ 
    //reduce the time shown on the timer
    elapsedMS += millis() - prevTime; //find the new elapsed time,
    if(elapsedMS > 60000){
      elapsedMS = elapsedMS%60000;
      minutes--; 
    }
    if(minutes == -1 && hours != 0){
        hours--;
        minutes = 59;
    }
    prevTime = millis();
  } 
}
