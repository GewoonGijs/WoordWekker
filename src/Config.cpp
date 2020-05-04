
#include <EEPROM.h>
#include <ESP8266WiFi.h>

#include "Config.h"


void PKConfigClass::ConfigureWifi()
{
  int retries = 0;

  if (!_dhcp) {
      WiFi.config(_IP, _Gateway, _Netmask);
  }
  /*
   * try to connect to Wifi
   */
  WiFi.mode(WIFI_STA);
  while ((retries++ < MAX_RETRIES) && (WiFi.status() != WL_CONNECTED)) {
    if (retries == 1 ) {
      WiFi.begin(_ssid.c_str(), _password.c_str());
      Serial.print("Trying to connect to: ");
      Serial.println (_ssid);
    } else {
      WiFi.begin();  //Saw this in ESPEAsy
      Serial.print("retry");
    }
    for (byte t = 0; t<10; t++){
      Serial.print(".");
      if (WiFi.status() != WL_CONNECTED)
        delay(500);
      else {
        break;
      }
    }
    Serial.println("!");
  }
}

void PKConfigClass::WriteConfig()
{

	Serial.println("Writing Config");
	EEPROM.write(0,'C');
	EEPROM.write(1,'F');
	EEPROM.write(2,'G');

	EEPROM.write(16, _dhcp);
	EEPROM.write(17, _daylight);

	EEPROMWritelong(18, _timezone);  // 4 Bytes

	EEPROM.write(22, _IP[0]);
	EEPROM.write(23, _IP[1]);
	EEPROM.write(24, _IP[2]);
	EEPROM.write(25, _IP[3]);

	EEPROM.write(26, _Netmask[0]);
	EEPROM.write(27, _Netmask[1]);
	EEPROM.write(28, _Netmask[2]);
	EEPROM.write(29, _Netmask[3]);

	EEPROM.write(30, _Gateway[0]);
	EEPROM.write(31, _Gateway[1]);
	EEPROM.write(32, _Gateway[2]);
	EEPROM.write(33, _Gateway[3]);

	EEPROMWritestring(64, _ssid);
	EEPROMWritestring(96, _password);
	EEPROMWritestring(128, _ntpServerName);
  EEPROMWritestring(160, _city);

	EEPROM.commit();
}

boolean PKConfigClass::ReadConfig()
{

	Serial.println("Reading Configuration");
	if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
	{
		Serial.println("Configurarion Found!");
		 _dhcp = 	EEPROM.read(16);
		 _daylight = EEPROM.read(17);
		 _timezone = EEPROMReadlong(18); // 4 Byte

		 _IP[0] = EEPROM.read(22);
		 _IP[1] = EEPROM.read(23);
		 _IP[2] = EEPROM.read(24);
		 _IP[3] = EEPROM.read(25);
		 _Netmask[0] = EEPROM.read(26);
		 _Netmask[1] = EEPROM.read(27);
		 _Netmask[2] = EEPROM.read(28);
		 _Netmask[3] = EEPROM.read(29);
		 _Gateway[0] = EEPROM.read(30);
		 _Gateway[1] = EEPROM.read(31);
		 _Gateway[2] = EEPROM.read(32);
		 _Gateway[3] = EEPROM.read(33);
		 _ssid = EEPROMReadstring(64);
		 _password = EEPROMReadstring(96);
		 _ntpServerName = EEPROMReadstring(128);
     _city = EEPROMReadstring(160);

		return true;
	}
	else
	{
		Serial.println("Configurarion NOT FOUND!!!!");
    Serial.print (EEPROM.read(0));
    Serial.print (EEPROM.read(1));
    Serial.println (EEPROM.read(2));
		return false;
	}
}


// Some private functions to write to EEPROM
void PKConfigClass::EEPROMWritelong(int address, long value)
      {
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }

long PKConfigClass::EEPROMReadlong(int address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void PKConfigClass::EEPROMWritestring(int address, String string)
{
	char  charBuf[string.length()+1];
	string.toCharArray(charBuf, string.length()+1);
	for (unsigned int t=0; t<sizeof(charBuf); t++)
	{
			EEPROM.write(address + t,charBuf[t]);
	}
}
String  PKConfigClass::EEPROMReadstring(int address)
{
	byte counter=0;
	char rChar;
	String retString = "";
	while (1)
	{
		rChar = EEPROM.read(address + counter);
		if (rChar == 0) break;
		if (counter > 31) break;
		counter++;
		retString.concat(rChar);

	}
	return retString;
}

PKConfigClass PKConfig;
