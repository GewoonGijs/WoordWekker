#ifndef ACTUALTIME_H
#define ACTUALTIME_H

#include <Arduino.h>

#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};

class ActualTimeClass {

public:
  byte getHour() {return _hour; }
  void setHour(byte h) {_hour = h; }
  byte getMinute(){return _minute; }
  void setMinute(byte m) {_minute = m; }
  byte getSecond() {return _second; }
  void setSecond(byte s) {_second = s; }
  int getYear() {return _year; }
  void setYear(int y) {_year = y; }
  byte getMonth() {return _month; }
  void setMonth(byte m) {_month = m; }
  byte getDay() {return _day; }
  void setDay(byte d) {_day = d; }
  byte getWeekday() {return _wday; }
  void setWeekday(byte wd) {_wday = wd; }

  unsigned long getUnixTimeStamp() {return _UnixTimestamp; }
  void setUnixTimestamp (unsigned long ts) {_UnixTimestamp = ts; }
  unsigned long getSunRise() { return _sunrise; }
  void setSunRise(unsigned long sr) { _sunrise = sr; }
  unsigned long getSunSet() { return _sunset; }
  void setSunSet(unsigned long ss) { _sunset = ss; }

  void updateActualTime();
  void CreateUnixTimeStamp();

protected:
  unsigned long _UnixTimestamp = 0;								// GLOBALTIME
  unsigned long _sunset;
  unsigned long _sunrise;

  byte _hour;
	byte _minute;
	byte _second;
	int _year;
	byte _month;
	byte _day;
	byte _wday;

  boolean isSummertime();
  void ConvertUnixTimeStamp( unsigned long TimeStamp);

};

extern ActualTimeClass ActualTime;
#endif //ACTUALTIME_H
