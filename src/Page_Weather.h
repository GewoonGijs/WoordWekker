
#ifndef PAGE_WEATHER_H
#define PAGE_WEATHER_H

const char PAGE_Weather[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Actual Weather</strong>
<hr>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" >
<tr><td align="right">Forecast:</td><td><div id="forecast" name="forecast" ></td></tr>
<tr><td align="right">Temperature:</td><td><div id="temp" name="temp" ></td></tr>
<tr><td align="right">Humidity:</td><td><div id="humidity" name="humidity" ></td></tr>
<tr><td align="right">Clouds:</td><td><input type="text" id="clouds" name="clouds" size="3" value="">%</td></tr>
<tr><td align="right">Rainfall:</td><td><input type="text" id="rainfall" name="rainfall" size="5" value="">mm</td></tr>
<tr><td align="right">RainIndicator:</td><td><input type="text" id="rainindicator" name="rainindicator" size="3" value=""></td></tr>
<tr><td align="right">City:</td><td><input type="text" id="city" name="city" maxlength="20" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>
<script>


window.onload = function ()
{
	load("style.css","css", function()
	{
		load("microajax.js","js", function()
		{
				setValues("/admin/weathervalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";


extern boolean updateDisplay;

void send_weather_html()
{
	if (server.args() > 0 )  // Save Settings
	{
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			if (server.argName(i) == "clouds") forecast.setClouds(server.arg(i).toInt());
      if (server.argName(i) == "rainfall") forecast.setRainfall(server.arg(i).toFloat());
      if (server.argName(i) == "rainindicator") forecast.setRainfallIndicator(server.arg(i).toInt());
      if (server.argName(i) == "city") PKConfig.setCity(urldecode(server.arg(i)));
		}
    updateDisplay = true;
    PKConfig.WriteConfig();
    if (PKConfig.getCity() != forecast.getCity()) forecast.updateInfo();
	}
	server.send_P ( 200, "text/html", PAGE_Weather );
}

void send_weather_values_html()
{
	String values ="";
  values += "forecast|" +  (String) forecast.getForecast() + "|div\n";
	values += "clouds|" +  (String) forecast.getClouds() + "|input\n";
	values += "temp|" +  (String) forecast.getTemp() +" &#8451" + "|div\n";
	values += "humidity|" +  (String) forecast.getHumidity()+ "%" + "|div\n";
  values += "rainfall|" +  (String) forecast.getRainfall() + "|input\n";
  values += "rainindicator|" +  (String) forecast.getRainIndicator() + "|input\n";
  values += "city|" + (String) forecast.getCity() + "|input\n";
	server.send ( 200, "text/plain", values);
}
#endif
