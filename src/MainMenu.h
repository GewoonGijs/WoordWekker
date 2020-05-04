/*
 * MainMenu.h
 *
 * Handles the Main Menu of the Wekker
 */


#ifndef MAINMENU_H
#define MAINMENU_H

#include <Arduino.h>

#define LONGPRESSED 2000
#define BLINK 1000
#define SNOOZETIME 10000

typedef enum AlarmState {
  NOALARM = 0,
  WAITFORALARM,
  ALARM,
  SET,
  SNOOZE
} AlarmState;

class MainMenuClass {

public:
  void handle();
  AlarmState getState() { return _state; }
  MainMenuClass();
  void setVolume (uint8_t v);
  uint8_t getVolume ();
  void setAlarmHour(uint8_t h) { if (h<13) _alarmhour = h%12; } //12 is valid input, but translated internally to 0;
  uint8_t getAlarmHour() { return _alarmhour; }
  void setAlarmMinute(uint8_t m) { if (m<60) _alarmminute = m; } //12 is valid input, but translated internally to 0;
  uint8_t getAlarmMinute() { return _alarmminute; }
  void setSnooze(uint16_t s) { _snoozetime = 1000*s; } //Snooze time can be entered in seconds
  uint16_t getSnooze() { return _snoozetime/1000; }
protected:
  uint8_t readButton();

  AlarmState    _state;
  uint8_t       _button;
  unsigned long _lastpressed;
  boolean       _justreleased;
  unsigned long _lastblink;
  unsigned long _snoozetime;

  uint8_t       _alarmhour;
  uint8_t       _alarmminute;
  boolean       _blinkerstate = false;
  uint8_t      _maxtracks = 15;
  uint8_t      _currenttrack;
  uint8_t      _currentadvert;
  uint8_t       _volume = 20;
};

extern MainMenuClass mainMenu;

#endif
