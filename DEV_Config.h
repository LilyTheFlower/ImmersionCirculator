/******************************************************************************
**************************Hardware interface layer*****************************
  | file      	:	DEV_Config.h
  |	version		  :	V1.0
  | date		    :	2020-06-16
  | function	  :	Provide the hardware underlying interface
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <SPI.h>
#include <Wire.h>
#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#ifndef ARDUINO_ARCH_AVR
#include <Arduino.h>
#endif
/**
   data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define USE_SPI_4W  1
#define USE_IIC     0

#define IIC_CMD        0X00
#define IIC_RAM        0X40

/****************************************************************************************
    //Use the  library function definition
*****************************************************************************************/
//GPIO config
#define OLED_CS 8
#define OLED_CS_0     digitalWrite(OLED_CS, LOW)
#define OLED_CS_1     digitalWrite(OLED_CS, HIGH)

#define OLED_RST 34
#define OLED_RST_0    digitalWrite(OLED_RST, LOW)
#define OLED_RST_1    digitalWrite(OLED_RST, HIGH)

#define OLED_DC 9
#define OLED_DC_0     digitalWrite(OLED_DC, LOW)
#define OLED_DC_1     digitalWrite(OLED_DC, HIGH)

/*------------------------------------------------------------------------------------------------------*/

#define TEMPBUTTONPIN 13
#define TIMERBUTTONPIN 14
#define WATTAGEBUTTONPIN 15

#define SETBUTTONPIN 42
#define UPBUTTONPIN 21
#define DOWNBUTTONPIN 16

#define HEATERPIN A0

#define TEMPTRANSISTORPIN A1

#define MAXDO   7
#define MAXCS   6
#define MAXCLK  5

#define AVGCOUNT 20 //the number of readings to average for temperatures, more readings means less shifting but more delay in accurate measurement

uint8_t System_Init(void);

void SPI4W_Write_Byte(uint8_t DATA);
void I2C_Write_Byte(uint8_t value, uint8_t Cmd);

void Driver_Delay_ms(unsigned long xms);
void Driver_Delay_us(int xus);

#endif
