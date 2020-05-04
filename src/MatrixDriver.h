#ifndef MATRIXDRIVER_H
#define MATRIXDRIVER_H

#include <Arduino.h>
#include <MD_MAX72xx.h>

class MatrixDriverClass {

public:

  void begin();
  void updateDisplay(short hour, short minute, boolean isNight);
  void setBrightness(uint8_t br);
  uint8_t getBrightness() { return brightness; }
  void showAlarm(boolean b);
  void showWeather(String icon);
  void showHetIs(boolean b);
  void showMinute(int minute);
  void showHour(int hour);

protected:
  uint8_t brightness = 3;
};


extern MatrixDriverClass matrixDriver;

#endif //MATRIXDRIVER_H
