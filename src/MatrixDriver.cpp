#include <stdio.h>

#include "MatrixDriver.h"

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES	2

#define CLK_PIN   D5  // or SCK
#define DATA_PIN  D7  // or MOSI
#define CS_PIN    D8  // or SS

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


void MatrixDriverClass::begin(){
  Serial.println("Matrix Driver started");
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 1);
}

void MatrixDriverClass::setBrightness(uint8_t br) {
  mx.control(MD_MAX72XX::INTENSITY, br % 4);
  brightness = br % 4;
}

void MatrixDriverClass::updateDisplay(short hour, short minutes, boolean isNight) {
  showHetIs(true);
  showMinute(minutes);
  if (minutes >= 20) {
    showHour(hour+1);
  }
  else {
    showHour(hour);
  }
}

// See openweather.com for the icons.

void MatrixDriverClass::showWeather(String icon){
  if (icon.startsWith("01")) { mx.setRow(0, 0, 0b00000001); }
  if (icon.startsWith("02")) { mx.setRow(0, 0, 0b00000010); }
  if (icon.startsWith("03")) { mx.setRow(0, 0, 0b00000100); }
  if (icon.startsWith("04")) { mx.setRow(0, 0, 0b00000100); }
  if (icon.startsWith("09")) { mx.setRow(0, 0, 0b00001000); }
  if (icon.startsWith("10")) { mx.setRow(0, 0, 0b00010000); }
  if (icon.startsWith("11")) { mx.setRow(0, 0, 0b00100000); }
  if (icon.startsWith("13")) { mx.setRow(0, 0, 0b01000000); }
  if (icon.startsWith("50")) { mx.setRow(0, 0, 0b10000000); }
}

void MatrixDriverClass::showHetIs (boolean b) {
  if (b) {
    mx.setRow(1, 6, 0b00110111);
  } else {
    mx.setRow(1, 6, 0);
  }
}

void MatrixDriverClass::showAlarm(boolean b){
  if (b) {
    mx.setRow(1, 7, 0b00001000);
  } else {
    mx.setRow(1, 7, 0);
  }
}

void MatrixDriverClass::showMinute(int minute){
  int q;

  q = minute % 5;
  minute -= q;

  switch(q) {
    case 0: mx.setRow(0, 1, 0b00000000); break;
    case 1: mx.setRow(0, 1, 0b00000100); break;
    case 2: mx.setRow(0, 1, 0b00001100); break;
    case 3: mx.setRow(0, 1, 0b00011100); break;
    case 4: mx.setRow(0, 1, 0b00111100); break;
  }

  switch (minute) {
    case 45 :
      mx.setRow(1, 5, 0);
      mx.setRow(1, 4, 0b01111100); // KWART
      mx.setRow(1, 3, 0b00001111); // VOOR
      mx.setRow(1, 2, mx.getRow(1, 2) & 0b11110000); //HALF_OFF
      mx.setRow(0, 7-4, 0b00000000); // UUR_OFF
      break;
    case 50 :
      mx.setRow(1, 5, 0b11110000); // TIEN
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0b00001111); // VOOR
      mx.setRow(1, 2, mx.getRow(1, 2) & 0b11110000); //HALF_OFF
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 55 :
      mx.setRow(1, 5, 0b00001111); // VIJF
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0b00001111); // VOOR
      mx.setRow(1, 2, mx.getRow(1, 2) & 0b11110000); //HALF_OFF
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 0 :
      mx.setRow(1, 5, 0);
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0);
      mx.setRow(1, 2, mx.getRow(1, 2) & 0b11110000); //HALF_OFF
      mx.setRow(0, 3, 0b00011100); // UUR
      break;
    case 5 :
      mx.setRow(1, 5, 0b00001111); // VIJF
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0b11110000); // OVER
      mx.setRow(1, 2, mx.getRow(1, 2) & 0b11110000); //HALF_OFF
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 10 :
      mx.setRow(1, 5, 0b11110000); // TIEN
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0b11110000); // OVER
      mx.setRow(1, 2, mx.getRow(1, 2) & 0b11110000); //HALF_OFF
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 15 :
      mx.setRow(1, 5, 0);
      mx.setRow(1, 4, 0b01111100); // KWART
      mx.setRow(1, 3, 0b11110000); // OVER
      mx.setRow(1, 2, mx.getRow(1, 2) & 0b11110000); //HALF_OFF
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 20 :
      mx.setRow(1, 5, 0b11110000); // TIEN
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0b00001111); // VOOR
      mx.setRow(1, 2, mx.getRow(1, 2) | 0b00001111); //HALF_ON
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 25 :
      mx.setRow(1, 5, 0b00001111); // VIJF
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0b00001111); // VOOR
      mx.setRow(1, 2, mx.getRow(1, 2) | 0b00001111); //HALF_ON
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 30 :
      mx.setRow(1, 5, 0);
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0);
      mx.setRow(1, 2, mx.getRow(1, 2) | 0b00001111); //HALF_ON
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 35 :
      mx.setRow(1, 5, 0b00001111); // VIJF
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3, 0b11110000); // OVER
      mx.setRow(1, 2, mx.getRow(1, 2) | 0b00001111); //HALF_ON
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
    case 40 :
      mx.setRow(1, 5, 0b11110000); // TIEN
      mx.setRow(1, 4, 0);
      mx.setRow(1, 3 ,0b11110000); // OVER
      mx.setRow(1, 2, mx.getRow(1, 2) | 0b00001111); //HALF_ON
      mx.setRow(0, 3, 0b00000000); // UUR_OFF
      break;
  }
}

void MatrixDriverClass::showHour(int hour) {

  hour %= 12; // We have a 12 hour display, not 24

  mx.setRow(1, 2, mx.getRow(1, 2) & 0b00001111); //VIJF_OFF
  mx.setRow(1, 1, 0);
  mx.setRow(1, 0, 0);
  mx.setRow(0, 7, 0);
  mx.setRow(0, 6, 0);
  mx.setRow(0, 5, 0);
  mx.setRow(0, 4, 0);

  switch(hour) {
    case 1 :
      mx.setRow(1, 0, 0b00000111); // EEN
    break;
    case 2 :
      mx.setRow(0, 7, 0b00001111); //TWEE
    break;
    case 3 :
      mx.setRow(1, 0, 0b11110000); //DRIE
    break;
    case 4 :
      mx.setRow(0, 7, 0b11110000); //VIER
    break;
    case 5 :
      mx.setRow(1, 2, mx.getRow(1, 2) | 0b11110000); //VIJF has to combine with HALF
    break;
    case 6 :
      mx.setRow(1, 1, 0b00000111); //ZES
    break;
    case 7 :
      mx.setRow(1, 1, 0b11111000); //ZEVEN
    break;
    case 8 :
      mx.setRow(0, 6, 0b00001111); //ACHT
    break;
    case 9 :
      mx.setRow(0, 5, 0b11111000); //NEGEN
    break;
    case 10 :
      mx.setRow(0, 6, 0b11110000); //TIEN
    break;
    case 11 :
      mx.setRow(0, 5, 0b00000111); //ELF
    break;
    case 0 :
      mx.setRow(0, 4, 0b01111110); //TWAALF
    break;
  }
}
