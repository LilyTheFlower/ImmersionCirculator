#include "OLED_Driver.h"
#include "GUI_paint.h"
#include "DEV_Config.h"
#include "Debug.h"
#include "drawGUI.h"
void setup() {
  System_Init();
  Serial.print(F("OLED_Init()...\r\n"));
  OLED_1in5_Init();
  Driver_Delay_ms(500); 
  OLED_1in5_Clear();  

  //temperature button
  pinMode(5, INPUT);

  //Timer Button
  pinMode(6, INPUT);
  
  pinMode(9, OUTPUT);
  //0.Create a new image cache
  UBYTE *BlackImage;
  UWORD Imagesize = ((OLED_1in5_WIDTH%2==0)? (OLED_1in5_WIDTH/2): (OLED_1in5_WIDTH/2+1)) * OLED_1in5_HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) { //No enough memory
      while(1) {
        Serial.print("Failed to apply for black memory...\r\n");
      }
  }
  Serial.print("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, OLED_1in5_WIDTH, OLED_1in5_HEIGHT, 0, BLACK);  
  Paint_SetScale(16);

  //1.Select Image
  Paint_SelectImage(BlackImage);
  Paint_Clear(BLACK); //clear the screen
  OLED_1in5_Display(BlackImage);
  Driver_Delay_ms(500); //give the screen half a second to draw
  
  //define program variables
  
  int modeNum = 0; //screen mode 0 for temp, 1 for timer and 2 for wattage if its added
  int targetTemp = 100;   //Target Temperature in Degrees farenheit
  int currentTemp = 60;   //Current Temperature in Degrees farenheit
  
  //define timer values
  int hours = 2;
  int minutes = 30;
  long startTime = millis();
  int elapsedHours;
  int elapsedMinutes;
  
  int temperatureButtonInput = LOW;
  int temperatureButtonReleased = 1;
  int temperatureMode = 0;
  long tempButtonStartTime = 0;
  int degreeMode = 0; //0 for celcius and 1 for farenheit
  int degreeModeSwitched = 0;
  
  int timerButtonInput = LOW;

  int heatOn = 0;
  
  while(1) {
     //Driver_Delay_ms(100);
    timerButtonInput = digitalRead(6);
    temperatureButtonInput = digitalRead(5);
    if(timerButtonInput == HIGH){
      modeNum = 1;
    }else if(temperatureButtonInput == HIGH && temperatureButtonReleased == 1 && temperatureMode == 0){
      tempButtonStartTime = millis();
      temperatureButtonReleased = 0;
      modeNum = 2;
    }else if(temperatureButtonInput == HIGH && temperatureButtonReleased == 1 && temperatureMode == 1){
      tempButtonStartTime = millis();
      temperatureButtonReleased = 0;
      modeNum = 0;
    }

    if(temperatureButtonInput == LOW){
      temperatureButtonReleased == 1;
      degreeModeSwitched = 0;
    }

    if(millis()-tempButtonStartTime > (3*1000) && degreeModeSwitched == 0){
      degreeModeSwitched = 1;
      if(degreeMode == 0){
        degreeMode = 1;
      }else{
        degreeMode = 0;
      }
    }

    elapsedHours = (millis() - startTime)/(1000*60*60);
    elapsedMinutes = (millis() - startTime)/(1000*60);
    
    //Check mode and draw GUI
    switch(modeNum){
      case 0:
        drawCurrentTemp(currentTemp, degreeMode);
        break;
      case 1:    
        drawTimer(hours-elapsedHours, minutes-elapsedMinutes);
        break;
      case 2:    
        drawTargetTemp(targetTemp, degreeMode);
        break;
      case 3:    
         Paint_DrawLine(10, 0, 10, 32, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
         Paint_DrawLine(20, 10, 20, 25, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
         Paint_DrawLine(30, 10, 30, 25, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
         Paint_DrawLine(40, 10, 40, 25, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
        break;
    }

    if(heatOn){
      heatOn = 0;
    }else{
      heatOn = 1;
    }

    if(heatOn){
      digitalWrite(9, HIGH);
    }else{
      digitalWrite(9, LOW);
    }
    float tempIn = analogRead(4);
    
    tempIn = tempIn * (3.3/8191);
    Serial.print(tempIn);
    Serial.print("\n");
    OLED_1in5_Display(BlackImage);
    
  }   
}

void loop() {
  //because of the messy library we cant put some of the drawing code in here, so might as well just stay in setup phase ;-;
}
