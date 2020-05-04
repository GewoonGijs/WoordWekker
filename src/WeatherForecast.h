#ifndef WEATHERFORECAST_H
#define WEATHERFORECAST_H

#include <Arduino.h>


class WeatherForecast {
  public:
    String getForecast();
    int getWeathercondition();
    String getIcon();
    float getRainfall();
    int getRainIndicator();
    int getClouds(); //percentage
    String getCity();
    void setForecast(String s) { _forecast = s; }
    void setRainfall(float r) { _rainfall = r; }
    void setRainfallIndicator(int ri) { _rainindicator = ri; }
    void setClouds(int c) { _clouds = c; }
    void updateInfo();
    float getTemp();
    float getHumidity();

  protected:
    unsigned long _lastupdated = 0;
    String _forecast;
    String _icon;
    float _rainfall;
    int _rainindicator;
    int _clouds;
    String _city;
    float _temp;
    float _humidity;
    int _weatherCondition;

    void ParseJsonData(String& message);

};
#endif
