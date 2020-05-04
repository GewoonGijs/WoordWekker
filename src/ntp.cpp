/*
**
**  NTP
**
*/

#include <ESP8266Wifi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>

#include "Config.h"
#include "ntp.h"
#include "ActualTime.h"


static WiFiUDP UDPNTPClient;											// NTP Client
const int NTP_PACKET_SIZE = 48;
byte packetBuffer [NTP_PACKET_SIZE];
extern boolean updateDisplay;


void NTPClient::NTPStart() {
  UDPNTPClient.begin(2390);  // Port for NTP receive
}

void NTPClient::NTPRequest() {
  if ((millis() - _lastsent) < TIME_BETWEEN_REQS) return;
	if (WiFi.status() == WL_CONNECTED)
	{
		IPAddress timeServerIP;
    WiFi.hostByName(PKConfig.getNTPServerName().c_str(), timeServerIP);

    Serial.print("sending NTP packet...");
    Serial.println(timeServerIP);
		memset(packetBuffer, 0, NTP_PACKET_SIZE);
		packetBuffer[0] = 0b11100011;   // LI, Version, Mode
		packetBuffer[1] = 0;     // Stratum, or type of clock
		packetBuffer[2] = 6;     // Polling Interval
		packetBuffer[3] = 0xEC;  // Peer Clock Precision
		packetBuffer[12]  = 49;
		packetBuffer[13]  = 0x4E;
		packetBuffer[14]  = 49;
		packetBuffer[15]  = 52;
		UDPNTPClient.beginPacket(timeServerIP, 123);
		UDPNTPClient.write(packetBuffer, NTP_PACKET_SIZE);
		UDPNTPClient.endPacket();
    _lastsent = millis();
  }
}

void NTPClient::NTPHandleResponse() {
  int cb = UDPNTPClient.parsePacket();
  if (!cb) {
    // Serial.println("NTP no packet yet");
  }
  else
  {
    Serial.print("NTP packet received, length=");
    Serial.println(cb);
    UDPNTPClient.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    ActualTime.setUnixTimestamp(epoch);
    _lasttime = epoch;
    ActualTime.updateActualTime();
    Serial.print("NTP timestamp received: ");
    Serial.printf("%2d-%2d-%4d  %02d:%02d\n", ActualTime.getDay(), ActualTime.getMonth(),
                  ActualTime.getYear(), ActualTime.getHour(), ActualTime.getMinute());
    updateDisplay = true;
  }
}
