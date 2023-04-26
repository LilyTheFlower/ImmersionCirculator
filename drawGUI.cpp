#include "drawGUI.h"


void DrawCharFromHex(int x, int y, int h, int w, const uint8_t Arr[]){
  for(int j = 0; j < h; j++){
     for(int i = 0;  i < w/8; i++){
        char s[40];
        Paint_DrawPoint(x + i*8 + 0, y+j, WHITE * (((Arr[3*j + i]) & 1) >> 0), DOT_PIXEL_DFT, DOT_STYLE_DFT);
        Paint_DrawPoint(x + i*8 + 1, y+j, WHITE * (((Arr[3*j + i]) & 2) >> 1), DOT_PIXEL_DFT, DOT_STYLE_DFT);
        Paint_DrawPoint(x + i*8 + 2, y+j, WHITE * (((Arr[3*j + i]) & 4) >> 2), DOT_PIXEL_DFT, DOT_STYLE_DFT);
        Paint_DrawPoint(x + i*8 + 3, y+j, WHITE * (((Arr[3*j + i]) & 8) >> 3), DOT_PIXEL_DFT, DOT_STYLE_DFT);
        Paint_DrawPoint(x + i*8 + 4, y+j, WHITE * (((Arr[3*j + i]) & 16) >> 4), DOT_PIXEL_DFT, DOT_STYLE_DFT);
        Paint_DrawPoint(x + i*8 + 5, y+j, WHITE * (((Arr[3*j + i]) & 32) >> 5), DOT_PIXEL_DFT, DOT_STYLE_DFT);
        Paint_DrawPoint(x + i*8 + 6, y+j, WHITE * (((Arr[3*j + i]) & 64) >> 6), DOT_PIXEL_DFT, DOT_STYLE_DFT);
        Paint_DrawPoint(x + i*8 + 7, y+j, WHITE * (((Arr[3*j + i]) & 128) >> 7), DOT_PIXEL_DFT, DOT_STYLE_DFT);
     }
  }
}



void DrawNumber(int x, int y, int h, int w, int num){
      if(num == 0){
        DrawCharFromHex(x, y, h, w, zero);
      }else if(num == 1){
        DrawCharFromHex(x, y, h, w, one);
      }else if(num == 2){
        DrawCharFromHex(x, y, h, w, two);
      }else if(num == 3){
        DrawCharFromHex(x, y, h, w, three);
      }else if(num == 4){
        DrawCharFromHex(x, y, h, w, four);
      }else if(num == 5){
        DrawCharFromHex(x, y, h, w, five);
      }else if(num == 6){
        DrawCharFromHex(x, y, h, w, six);
      }else if(num == 7){
        DrawCharFromHex(x, y, h, w, seven);
      }else if(num == 8){ 
        DrawCharFromHex(x, y, h, w, eight);
      }else if(num == 9){
        DrawCharFromHex(x, y, h, w, nine);
      }
}

void drawTargetTemp(float targetTemp, int ForC, int set){
    Paint_Clear(BLACK);//clear image

    int x = 10;
    
    

    if(ForC == 0){
     targetTemp = (targetTemp - 32) * (5.0/9.0);
    }
    
    int temp = targetTemp;

    int temph = (temp - temp % 100)/100;
    temp = temp - temph*100;
    int tempt = (temp - temp % 10)/10;
    int tempo = temp - tempt*10; 

    if(temph != 0){
      DrawNumber(x , 36, 48, 24, temph); 
    }
    if(temph != 0 || tempt != 0){
      DrawNumber(x + 24, 36, 48, 24, tempt);
    }
    DrawNumber(x + 24*2, 36, 48, 24, tempo);
    DrawCharFromHex(x + 24*3, 36, 48, 24, degree);

    if(ForC == 1){
      DrawCharFromHex(x + 24*4, 36, 48, 24, F);
    }else{
      DrawCharFromHex(x + 24*4, 36, 48, 24, C48);
    }

    if(set == 1){
       Paint_DrawLine(x + 24*2, 85, x + 24*3, 85, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }else if(set == 2){
       Paint_DrawLine(x + 24, 85, x + 24*2, 85, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }else if(set == 3){
       Paint_DrawLine(x , 85, x + 24, 85, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }
    
    updateDisplay();
}

void drawCurrentTemp(float temp, int ForC){
    Paint_Clear(BLACK);//clear image

    int x = 10; //the horizontal shift on the screen
    
    if(ForC == 0){
      temp = (temp - 32) * (5.0/9.0);
    }
    
    int temph = temp / 100;
    int tempt = (temp-(temph*100))/10;
    int tempo = temp - (temph*100) - (tempt*10);
  
    if(temph != 0){
      DrawNumber(x , 36, 48, 24, temph); 
    }
    
    DrawNumber(x + 24, 36, 48, 24, tempt);
    DrawNumber(x + 24*2, 36, 48, 24, tempo);
    DrawCharFromHex(x + 24*3, 36, 48, 24, degree);

    if(ForC == 1){
      DrawCharFromHex(x + 24*4, 36, 48, 24, F);
    }else{
      DrawCharFromHex(x + 24*4, 36, 48, 24, C48);
    }
     updateDisplay();
}

void drawTimer(int hours, int minutes, int set){
  Paint_Clear(BLACK);//clear image
  int h1;
  int h2;
  if(hours < 1){
    h1 = 0;
    h2 = 0;
  }else{
    h1 = hours / 10;
    h2 = hours % 10;
  }

  int m1;
  int m2;
  
  if(minutes < 1){
    m1 = 0;
    m2 = 0;
  }else{
    m1 = minutes / 10;
    m2 = minutes % 10;
  }

  int x = 2; //the offset from the left side of the screen that the numbers start at

  DrawNumber(x, 40, 40, 24, h1);
  DrawNumber(x + 24, 40, 48, 24, h2);

  DrawCharFromHex(x + 24 * 2, 40, 48, 24, colon);

  DrawNumber(x + 24 * 3, 40, 48, 24, m1);
  DrawNumber(x + 24 * 4, 40, 48, 24, m2);

    if(set == 4){
       Paint_DrawLine(x, 85, x + 24, 85, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }else if(set == 3){
       Paint_DrawLine(x + 24, 85, x + 24*2, 85, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }else if(set == 2){
       Paint_DrawLine(x + 24*3 , 85, x + 24*4, 85, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }else if(set == 1){
       Paint_DrawLine(x +24*4, 85, x + 24*5, 85, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }
  
  updateDisplay();
}

void drawWattage(){
    Paint_Clear(BLACK);//clear image
    Paint_DrawLine(10, 0, 10, 32, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(20, 10, 20, 25, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(30, 10, 30, 25, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(40, 10, 40, 25, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
     updateDisplay();
}
