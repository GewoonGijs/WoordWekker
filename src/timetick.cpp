#include <Arduino.h>

#include "ActualTime.h"

void Second_Tick() {
	ActualTime.setUnixTimestamp(ActualTime.getUnixTimeStamp() + 1);
	ActualTime.updateActualTime();
}

void Display_Tick(boolean *update) {
	*update = true;
}
