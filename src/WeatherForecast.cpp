#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "WeatherForecast.h"
#include "ActualTime.h"
#include "Config.h"

#define UPDATE_INTERVAL 3600

//Note: Time could be shifted.
// T ocheck if that's the case, we check of last_updated is in the future as well

String WeatherForecast::getForecast() {
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _forecast;
}

String WeatherForecast::getIcon() {
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _icon;
}

int WeatherForecast::getWeathercondition() {
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _weatherCondition;
}

String WeatherForecast::getCity() {
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _city;
}


float WeatherForecast::getRainfall(){
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _rainfall;
}

int WeatherForecast::getRainIndicator(){
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _rainindicator;
}

int WeatherForecast::getClouds(){
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _clouds;
}

float WeatherForecast::getTemp(){
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _temp;
}

float WeatherForecast::getHumidity(){
  if ((ActualTime.getUnixTimeStamp() - _lastupdated > UPDATE_INTERVAL ) |
      (ActualTime.getUnixTimeStamp() < _lastupdated)) {
    updateInfo();
    _lastupdated = ActualTime.getUnixTimeStamp();
  }
  return _humidity;
}


void WeatherForecast::updateInfo() {
  //
  // Get info from openweathermap.org
  //
  // Chosen to use weather instead of forecast, gives sunrise and
  // sunset as well
  //
  String URL = "http://api.openweathermap.org/data/2.5/weather?q="
               + PKConfig.getCity() +
               "&apikey=11f2ba8f50b27b62e407aff54a512491&lang=nl&units=metric&cnt=1";

  WiFiClient client;
  HTTPClient http;

  // Initiate JSON request
  http.begin(client, URL);
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          ParseJsonData(payload);
        }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void WeatherForecast::ParseJsonData(String& message){

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.println("JSON parsing failed!");
    return;
  }

  //@TODO: Check that city is a copy, not a reference.
  _city = doc["name"].as<String>(); //Need to cast explicitly.

  _temp = doc["main"]["temp"];
  _humidity = doc["main"]["humidity"];
  _clouds = doc["clouds"]["all"];
  _forecast = doc["weather"][0]["description"].as<String>(); //Need to cast explicitly
  _icon = doc["weather"][0]["icon"].as<String>();
  _weatherCondition = doc["weather"][0]["id"];
  ActualTime.setSunRise(doc["sys"]["sunrise"]);
  ActualTime.setSunSet(doc["sys"]["sunset"]);
  _rainfall = doc["rain"]["3h"];

  // Alternative: Determin rainfall on weatherCondition
  if ((_weatherCondition / 100) < 7) {
    // Seems like weather is in 200, 300, 500 or 600 range
    _rainindicator = ((_weatherCondition % 10)+1) * 20; //4 is max
  } else {
    _rainindicator = _rainfall*20;
  }
}
