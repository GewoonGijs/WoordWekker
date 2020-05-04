
#ifndef PAGE_WEKKERCONFIGURATION_H
#define PAGE_WEKKERCONFIGURATION_H

const char PAGE_WekkerConfiguration[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Visual Settings</strong>
<hr>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" >
<tr><td align="right">Volume:</td><td><input type="text" id="volume" name="volume" size="2" value=""></td></tr>
<tr><td align="right">Brightness:</td><td><input type="text" id="bright" name="bright" size="1" value=""></td></tr>
<tr><td align="right">Alarm time:</td><td><input type="text" id="hour" name="hour" size="2" value=""></td><td>:</td><td><input type="text" id="minute" name="minute" size="2" value=""></td></tr>
<tr><td align="right">Snooze time:</td><td><input type="text" id="snooze" name="snooze" size="3" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table></form>
<script>
window.onload = function ()
{
	load("style.css","css", function()
	{
		load("microajax.js","js", function()
		{
				setValues("/admin/wekkervalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>
)=====";


extern boolean updateDisplay;

void send_wekker_configuration_html()
{
	if (server.args() > 0 )  // Save Settings
	{
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			if (server.argName(i) == "volume") mainMenu.setVolume(server.arg(i).toInt());
			if (server.argName(i) == "bright") matrixDriver.setBrightness(server.arg(i).toInt());
      if (server.argName(i) == "hour") mainMenu.setAlarmHour(server.arg(i).toInt());
      if (server.argName(i) == "minute") mainMenu.setAlarmMinute(server.arg(i).toInt());
      if (server.argName(i) == "snooze") mainMenu.setSnooze(server.arg(i).toInt());
		}
    updateDisplay = true;
	}
	server.send_P ( 200, "text/html", PAGE_WekkerConfiguration );
}

void send_wekker_configuration_values_html()
{
	String values ="";
	values += "volume|" +  (String) mainMenu.getVolume() + "|input\n";
  values += "bright|" +  (String) matrixDriver.getBrightness() + "|input\n";
  values += "hour|" +  (String) mainMenu.getAlarmHour() + "|input\n";
  values += "minute|" +  (String) mainMenu.getAlarmMinute() + "|input\n";
  values += "snooze|" +  (String) mainMenu.getSnooze() + "|input\n";
	server.send ( 200, "text/plain", values);
}
#endif
