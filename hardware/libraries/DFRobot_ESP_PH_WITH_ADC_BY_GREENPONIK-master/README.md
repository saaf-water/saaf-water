## DFRobot_ESP_PH_WITH_ADC_BY_GREENPONIK Library
---------------------------------------------------------

ESP Ph Reading and Calibration
@ https://github.com/greenponik/DFRobot_ESP_PH_WITH_ADC_BY_GREENPONIK

>using Gravity: Analog pH Sensor / Meter Kit V2, SKU:SEN0161-V2 + ADC ADS1115

>based on DFRobot_PH @ https://github.com/DFRobot/DFRobot_PH

>IMPORTANT : to make it work, you will need the help of an additionnal ADC converter because the one on the ESP32 isn't accurate enough. Here we used an ADS1115 from Adafruit
>You can find it here : https://www.adafruit.com/product/1085
>
>And here is the library you'll need to add to your sketch : https://github.com/adafruit/Adafruit_ADS1X15


## Example

```C++

#include <Arduino.h>
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "Adafruit_ADS1015.h"
#include "EEPROM.h"

#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DFRobot_ESP_PH_WITH_ADC ph;
Adafruit_ADS1115 ads;

float voltage, phValue, temperature = 25;

float readTemperature()
{
	//add your code here to get the temperature from your temperature sensor
	sensors.requestTemperatures();
	return sensors.getTempCByIndex(0);
}

void setup()
{
	Serial.begin(115200);
	EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
	ph.begin();
	sensors.begin();
	ads.setGain(GAIN_ONE);
	ads.begin();
}

void loop()
{
	static unsigned long timepoint = millis();
	if (millis() - timepoint > 1000U) //time interval: 1s
	{
		timepoint = millis();
		/**
		 * index 0 for adc's pin A0
 		 * index 1 for adc's pin A1
		 * index 2 for adc's pin A2
		 * index 3 for adc's pin A3
		*/
		voltage = ads.readADC_SingleEnded(1) / 10; // read the voltage
		Serial.print("voltage:");
		Serial.println(voltage, 4);

		temperature = readTemperature(); // read your temperature sensor to execute temperature compensation
		Serial.print("temperature:");
		Serial.print(temperature, 1);
		Serial.println("^C");

		phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
		Serial.print("pH:");
		Serial.println(phValue, 4);
	}
	ph.calibration(voltage, temperature); // calibration process by Serail CMD
}
```

## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
ESP32  |      √       |             |            | 
ESP8266  |             |      √       |            | 

## Credits

Written by Mickael Lehoux from [@greenponik](https://www.greenponik.com/)

>Based on written by Jiawei Zhang(Welcome to our [website](https://www.dfrobot.com/))

# Tutorial here
[![Tutorial here](http://img.youtube.com/vi/EqFw561pO5k/0.jpg)](https://www.youtube.com/watch?v=EqFw561pO5k "PH Meter with ESP32 and DFRobot PH module (SEN0161-V2)")

### Suscribe on our newsletter here: 
en: http://bit.ly/2NuaKbN

fr: http://bit.ly/2XNf61R

https://www.greenponik.com

## support us
[become a patreon](https://www.patreon.com/bePatron?u=32614023)
