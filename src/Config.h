/*
 * Config.h
 *
 * Handles the configuration settings
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <ESP8266Wifi.h>

#define ACCES_POINT_NAME "PolkaKlok"
#define ACCES_POINT_PASSWORD "12345678"
#define MAX_RETRIES 5

class PKConfigClass {

public:

   String getSSID() { return _ssid; }
   void setSSID(String s) { _ssid = s; }
   String getPassword() { return _password; }
   void setPassword(String s) { _password = s; }
   IPAddress getIP() { return _IP ; }
   void setIP(IPAddress ip) { _IP = ip; }
   IPAddress getNetmask() { return _Netmask; }
   void setNetmask(IPAddress netmask) { _Netmask = netmask; }
   IPAddress getGateway() { return _Gateway; }
   void setGateway(IPAddress gw) { _Gateway = gw; }
   boolean getDHCP() { return _dhcp; }
   void setDHCP(boolean b) { _dhcp = b; }
   String getNTPServerName() { return _ntpServerName; }
   void setNTPServerName(String s) { _ntpServerName = s; }
   unsigned long getNTPUpdateInterval() { return _Update_Time_Via_NTP_Every; }
   void setNTPUpdateInterval(long interval) {_Update_Time_Via_NTP_Every = interval; }
   long getTimeZone () { return _timezone; }
   void setTimeZone(long tz) { _timezone = tz; }
   boolean isDaylightSavingMode () { return _daylight; }
   void setDaylightSavingMode(boolean mode) { _daylight = mode; }

   boolean ReadConfig ();
   void WriteConfig();
   void ConfigureWifi();

   String getCity() { return _city; }
   void setCity(String c) {_city = c;}

protected:
     String _ssid = ACCES_POINT_NAME;
     String _password = ACCES_POINT_PASSWORD;
     IPAddress _IP;
     IPAddress _Netmask;
     IPAddress _Gateway;
     boolean _dhcp;
     String _ntpServerName;
     unsigned long _Update_Time_Via_NTP_Every = 5; //Default 5 minutes
     long _timezone;
     boolean _daylight;
     String _city = "Delft,NL";

     void EEPROMWritelong(int address, long value);
     long EEPROMReadlong(int address);
     void EEPROMWritestring(int address, String string);
     String EEPROMReadstring(int address);
};

extern PKConfigClass PKConfig;

#endif //CONFIG_H
