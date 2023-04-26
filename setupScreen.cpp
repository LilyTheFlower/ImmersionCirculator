#include "setupScreen.h"
UBYTE *BlackImage;

void setUpDisplay(){
  Serial.print("OLED_Init()...\r\n");
  OLED_1in5_Init();
  Driver_Delay_ms(500); 
  OLED_1in5_Clear();  
  
  //Create a new image cache

  UWORD Imagesize = ((OLED_1in5_WIDTH%2==0)? (OLED_1in5_WIDTH/2): (OLED_1in5_WIDTH/2+1)) * OLED_1in5_HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) { //No enough memory
      while(1) {
        Serial.print("Failed to apply for black image memory...\r\n");
      }
  }
  Serial.print("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, OLED_1in5_WIDTH, OLED_1in5_HEIGHT, 0, BLACK);  
  Paint_SetScale(16);

  //1.Select Image
  Paint_SelectImage(BlackImage);
  Paint_Clear(BLACK); //clear the screen
  OLED_1in5_Display(BlackImage);
}

void updateDisplay(){
  OLED_1in5_Display(BlackImage);
}
