
#include <stdio.h>
#include <time.h>


#include "ActualTime.h"
#include "Config.h"

//
// Update the internal memebers to represent the UnixTimestamp
//
void ActualTimeClass::updateActualTime(){
  ConvertUnixTimeStamp(_UnixTimestamp + PKConfig.getTimeZone() * 360);
  if (PKConfig.isDaylightSavingMode() && isSummertime() ){
    ConvertUnixTimeStamp(_UnixTimestamp + PKConfig.getTimeZone() * 360 + 3600);
  }
}

//
// Summertime determines whther daylight saving time is active for a given date.
//
boolean ActualTimeClass::isSummertime()
// input parameters: "UTC normal time" for year, month, day, hour
{
 if (_month<3 || _month>10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
 if (_month>3 && _month<10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
 if ((_month==3 && ((_hour + 24 * _day)>=(1 + 24*(31 - (5 * _year /4 + 4) % 7)))) ||
     (_month==10 && ((_hour + 24 * _day)<(1 + 24*(31 - (5 * _year /4 + 1) % 7)))))
   return true;
 else
   return false;
}


// Update time members form UnixTimeStamp
void ActualTimeClass::ConvertUnixTimeStamp( unsigned long TimeStamp)
{
	uint8_t year;
	uint8_t month, monthLength;
	uint32_t time;
	unsigned long days;

  time = (uint32_t)TimeStamp;
  _second = time % 60;
  time /= 60; // now it is minutes
  _minute = time % 60;
  time /= 60; // now it is hours
  _hour = time % 24;
  time /= 24; // now it is days
  _wday = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
	days = 0;
	while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
		year++;
	}
  year--; // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time  -= days; // now it is days in this year, starting at 0

  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
  	if (month==1) { // february
  	  if (LEAP_YEAR(year)) {
  		monthLength=29;
  	  } else {
  		monthLength=28;
  	  }
  	} else {
  	  monthLength = monthDays[month];
  	}

    if (time >= monthLength) {
      time -= monthLength;
    } else {
    	break;
    }
  }
  _month = month + 1;  // jan is month 1
  _day = time + 1;     // day of month
  _year = year + 1970;
/*  Serial.print(_year);
  Serial.print("-");
  Serial.print(_month);
  Serial.print("-");
  Serial.print(_day);
  Serial.print("  ");
  Serial.print(_hour);
  Serial.print(":");
  Serial.print(_minute);
  Serial.print(":");
  Serial.println(_second);
*/
}

// Make UnixTimestamp from individual members.
void ActualTimeClass::CreateUnixTimeStamp(){

   unsigned long tyears, tdays, leaps, utc_hrs;
   long tzcorr;
   int i;

   tyears = _year - 1970; // tm->tm_year is from 1900.
   leaps = (tyears + 2) / 4; // no of next two lines until year 2100.
   i = (_year - 100) / 100;
   leaps -= ( (i/4)*3 + i%4 );

   tdays = 0;
   for (i=0; i < _month-1; i++) tdays += monthDays[i]; //_month jan = 1

   tdays += _day-1; // days of month passed. _day starts on 1
   tdays = tdays + (tyears * 365) + leaps;

   utc_hrs = _hour;
   tzcorr =  0 - PKConfig.getTimeZone() * 360; // for your time zone.
   if (PKConfig.isDaylightSavingMode() && isSummertime()) {
     tzcorr -= 3600;
   }
   _UnixTimestamp = (tdays * 86400) + (utc_hrs * 3600) + tzcorr + (_minute * 60) + _second;
}


ActualTimeClass ActualTime;
