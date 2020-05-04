/*
 * This is the main entry for the Polka Klok project.
 * This clock features the time with 16 LED's and gives
 * a Weather forecast for the next 3 hours.
 *
 * Time synchronisation via NTP
 * Startup configuration via Access Point mode
 * Configuration through WebServer
 * Weather forcast form Webservice openweathermap.org
 */
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <EEPROM.h>

#include "Config.h"
#include "helpers.h"
#include "ntp.h"
#include "timetick.h"
#include "MatrixDriver.h"
#include "ActualTime.h"
#include "Weatherforecast.h"
#include "MainMenu.h"

// Some globals
ESP8266WebServer server(80);									// The Webserver
Ticker tkSecond;
Ticker tkDisplay;
NTPClient NTP;
MatrixDriverClass matrixDriver;
boolean updateDisplay = true;
WeatherForecast forecast;

// Webpages
#include "Page_Script.js.h"
#include "Page_Style.css.h"

#include "PAGE_WekkerConfiguration.h"
#include "PAGE_NetworkConfiguration.h"
#include "PAGE_NTPSettings.h"
#include "PAGE_Weather.h"
#include "PAGE_Admin.h"


void setup ()
{
  Serial.begin(115200);
  EEPROM.begin(512);

  delay(3000);
  Serial.println("WoordWekker");

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  // Check if there is a Wifi Config available, if so try to connect, else
  // start AP mode.

  if (PKConfig.ReadConfig()) {

    // Set WiFi in station mode and configure IP address when not using DHCP
    PKConfig.ConfigureWifi();
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println ("Start in AP Mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP( ACCES_POINT_NAME, ACCES_POINT_PASSWORD);
  } else {
    Serial.println("Connected to Wifi in STA mode");
  }

  if (!MDNS.begin("Wekker")) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
  }
  //Webserver intialisation
  server.on ( "/favicon.ico",   []() { server.send ( 200, "text/html", "" );   }  );

  server.on ( "/admin.html", []() { server.send_P ( 200, "text/html", PAGE_AdminMainPage );   }  );
	server.on ( "/config.html", send_network_configuration_html );
  server.on ( "/style.css", []() { server.send_P( 200, "text/plain", PAGE_Style_css ); } );
	server.on ( "/microajax.js", []() { server.send_P ( 200, "text/plain", PAGE_microajax_js );  } );
	server.on ( "/admin/values", send_network_configuration_values_html );
	server.on ( "/admin/connectionstate", send_connection_state_values_html );

  server.on ( "/ntp.html", send_NTP_configuration_html  );
  server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
  server.on ( "/weather.html", send_weather_html  );
  server.on ( "/admin/weathervalues", send_weather_values_html );
  server.on ( "/wekker.html", send_wekker_configuration_html );
  server.on ( "/admin/wekkervalues", send_wekker_configuration_values_html );

  server.onNotFound ( []() { Serial.println("Page Not Found"); server.send ( 400, "text/html", "Page not Found" );   }  );
	server.begin();

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  NTP.NTPStart();
  NTP.NTPRequest();
  delay(10);
  NTP.NTPHandleResponse();

  tkSecond.attach(1, Second_Tick);
  tkDisplay.attach(15, Display_Tick, &updateDisplay);

  matrixDriver.begin();
}

void loop ()
{
 delay(3); //Leaves enough time for background tasks
 MDNS.update();

 if (updateDisplay | true) { //updateDisplay
   if (mainMenu.getState() != SET) {
        matrixDriver.updateDisplay(ActualTime.getHour(), ActualTime.getMinute(),
                  ActualTime.getUnixTimeStamp() > ActualTime.getSunSet());
    }
    matrixDriver.showWeather(forecast.getIcon());
    updateDisplay = false;
  }
  mainMenu.handle();
  server.handleClient();

  if (PKConfig.getNTPUpdateInterval() > 0) {
    if ((ActualTime.getUnixTimeStamp() - NTP.getLastTime() > PKConfig.getNTPUpdateInterval()*60) ||
        (NTP.getLastTime() == 0)) {
      NTP.NTPRequest();
    }
  }
  NTP.NTPHandleResponse();
}
