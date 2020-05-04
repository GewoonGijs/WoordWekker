/*
 *
 * MainMenu class
 *
 * Handles the handling of the button and the change of the alarm.
 *
 */
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

#include "MainMenu.h"
#include "MatrixDriver.h"
#include "ActualTime.h"

SoftwareSerial mySoftwareSerial(D1, D0); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int analogPin = A0;

MainMenuClass::MainMenuClass() {
  // Initialize DFPlayer on construction.
  mySoftwareSerial.begin(9600);

  while (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    delay(1000);
  }
  myDFPlayer.volume(0);  //Set volume low to prevent for noise
}

uint8_t MainMenuClass::getVolume() {
  return _volume;
}

void MainMenuClass::setVolume(uint8_t v) {
  if (v<30) _volume = v;
}

void MainMenuClass::handle() {

  readButton(); //Load actual buttonstate

  switch (_state) {
    case NOALARM:

      if (_justreleased) {  //Button was just released, handle button.
        // Set button pressed longer then 5 seconds, go to SET _state
        if ((millis() - _lastpressed) > LONGPRESSED) {
          if (_button == 1) {
            _state = SET;
            Serial.println("Goto SET");
            matrixDriver.showAlarm(true); // Ligth up Alarm symbol
            matrixDriver.updateDisplay(_alarmhour, _alarmminute, false);
            matrixDriver.showHetIs(false);
            _lastblink = millis();
            _blinkerstate = true;
          }
        } else {  //Short button
          switch (_button) {
            case 1:
              matrixDriver.showAlarm(true); // Ligth up Alarm symbol
              _state = WAITFORALARM;
              Serial.println("Goto WAITFORALARM");
            break;
            case 2:
              matrixDriver.setBrightness(matrixDriver.getBrightness()+1);
            break;
            case 4:
              matrixDriver.setBrightness(matrixDriver.getBrightness()-1);
            break;
          }
        }
        // Mark released button as handled and clear button.
        _justreleased = false;
        _button = 0;
      }
    break;

    case WAITFORALARM:

      if (_justreleased){  //Button was just released, handle button.
        // Set button pressed longer then 5 seconds, go to SET _state
        if ((millis() - _lastpressed) > LONGPRESSED) {
          if (_button == 1) {
            _state = SET;
            Serial.println("Goto SET");
            matrixDriver.showAlarm(true);
            matrixDriver.updateDisplay(_alarmhour, _alarmminute, false);
            matrixDriver.showHetIs(false);
            _lastblink = millis();
            _blinkerstate = true;
          }
        } else {  //Short button
          switch (_button) {
            case 1:
              matrixDriver.showAlarm(false); // Ligth up Alarm symbol
              _state = NOALARM;
              Serial.println("Goto NOALARM");
            break;
            case 2:
              matrixDriver.setBrightness(matrixDriver.getBrightness()+1);
            break;
            case 4:
              matrixDriver.setBrightness(matrixDriver.getBrightness()-1);
            break;
          }
        }
        // Mark released button as handled and clear button.
        _justreleased = false;
        _button = 0;
      }
      // Check for ALARM time, if true go to ALARM state, Note we only have 12 hours
      if ((_alarmhour == ActualTime.getHour() % 12) && (ActualTime.getMinute() == _alarmminute)) {
        _state = ALARM;
        Serial.println("Goto ALARM");
        myDFPlayer.volume(_volume);
        myDFPlayer.playMp3Folder(random(1, _maxtracks));
      }
    break;

    case ALARM:
      if (_justreleased) {  //Button was just released, handle button.
        // Set button pressed longer then 5 seconds, go to SET _state
        if ((millis() - _lastpressed) > LONGPRESSED) {
          if (_button == 1) {
            _state = NOALARM;
            Serial.println("Goto NOALARM");
            myDFPlayer.stop();
            myDFPlayer.volume(0);
            matrixDriver.showAlarm(false);
          }
        } else if (_button) {
          _state = SNOOZE;
          Serial.println("Goto SNOOZE");
          _snoozetime = millis();
          myDFPlayer.stop();
        }
        _justreleased = false;
        _button = 0;
      } else {
        // no buttons, but if the music had stopped, play the next Song
//        if ((myDFPlayer.readState() & 0xf0)==0) {
//          myDFPlayer.playMp3Folder(random(1, _maxtracks));
//      }
      }

    break;

    case SET:
      if ((millis() - _lastblink) > BLINK) {
        _blinkerstate = !_blinkerstate;
        matrixDriver.showAlarm(_blinkerstate);
        _lastblink = millis();
      }
      if (_justreleased) {  //Button was just released, handle button.
        switch(_button) {
          case 1:
            _state = WAITFORALARM;
            Serial.printf("%02d", _alarmhour);
            Serial.print(":");
            Serial.printf("%02d\n", _alarmminute);
            Serial.println("Goto WAITFORALARM");
            matrixDriver.showAlarm(true);  //Maybe off from blinking, so force to ON
            matrixDriver.updateDisplay(ActualTime.getHour(), ActualTime.getMinute(),
                            ActualTime.getUnixTimeStamp() > ActualTime.getSunSet());
          break;

          case 2:
            _alarmminute = (_alarmminute + 1) % 60;
            if (_alarmminute == 0) { _alarmhour = (_alarmhour + 1)%12;}
            if (_alarmminute == 20) { // At twenty we have to show the next hour
              matrixDriver.showHour((_alarmhour+1)%12);
            }
            matrixDriver.showMinute(_alarmminute);
          break;

          case 4:
            _alarmhour = (_alarmhour + 1) % 12;
            if (_alarmminute >= 20) {
              matrixDriver.showHour((_alarmhour+1)%12);
            } else {
              matrixDriver.showHour(_alarmhour);
            }
          break;
        }
        // Mark released button as handled and clear button.
        _justreleased = false;
        _button = 0;
      }
    break;

    case SNOOZE:
      if (_justreleased) {  //Button was just released, handle button.
        // Set button pressed longer then 5 seconds, go to NOALARM
        if ((millis() - _lastpressed) > LONGPRESSED) {
          if (_button == 1) {
            myDFPlayer.stop();
            myDFPlayer.volume(0);
            _state = NOALARM;
            Serial.println("Goto NOALARM");
            matrixDriver.showAlarm(false);
          }
        } else {  //Short button
          if (_button) {
            matrixDriver.showAlarm(true);
            _state = SNOOZE;
            Serial.println("Goto SNOOZE");
            _snoozetime = millis();
          }
        }
        // Mark released button as handled and clear button.
        _justreleased = false;
        _button = 0;
      } else {
        if ((millis() - _snoozetime) > SNOOZETIME) {
          myDFPlayer.volume(_volume);
          myDFPlayer.playMp3Folder(random(1, _maxtracks));
          _state = ALARM;
          Serial.println("Goto ALARM");
        }
      }
    break;
  }
}

uint8_t MainMenuClass::readButton() {
  uint16_t val;
  uint8_t newbtn;

  if (_justreleased) return (_button);

  val = analogRead(analogPin);  // read the input pin

  if (val < 80) newbtn = 0; //30
  if ((val > 120) && (val < 220)) newbtn = 1; //170
  if ((val > 260) && (val < 390)) newbtn = 2; //315
  if ((val > 420) && (val < 520)) newbtn = 3; //475
  if ((val > 570) && (val < 680)) newbtn = 4; //610
  if ((val > 720) && (val < 850)) newbtn = 5; //775
  if ((val > 880) && (val < 975)) newbtn = 6; //920
  if (val > 990) newbtn = 7;

  if (newbtn != _button) {
    if (newbtn != 0) {
      _lastpressed = millis();  // A button Change counts as press time.
      _button = newbtn;
    } else {
      if ((millis() - _lastpressed) > 30) {
        _justreleased = true; // leave buttonstate just notice that it is released
      } else {
        // pushed to short, treat as a glitch
        _button = newbtn;
        Serial.println("Spurious button");
      }
    }
  }
  return newbtn;
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}


MainMenuClass mainMenu;
