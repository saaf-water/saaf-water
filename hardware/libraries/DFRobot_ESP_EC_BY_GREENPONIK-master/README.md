## DFRobot_ESP_EC_BY_GREENPONIK Library
---------------------------------------------------------

ESP EC Reading and Calibration
@ https://github.com/greenponik/DFRobot_ESP_EC_BY_GREENPONIK

>using Gravity: Analog ec Sensor / Meter Kit V2, SKU:DFR0300

>based on DFRobot_EC @ https://github.com/DFRobot/DFRobot_EC

IMPORTANT : to make it work, you will need the help of an additionnal ADC converter because the one on the ESP32 isn't accurate enough. Here we used an ADS1115 from Adafruit
>You can find it here : https://www.adafruit.com/product/1085

>And here is the library you'll need to add to your sketch : https://github.com/adafruit/Adafruit_ADS1X15

example how to use ESP EC read library
DFRobot ESP EC BY GREENPONIK

#1 clone the repo
git clone https://github.com/GreenPonik/esp32-read-ec-example.git

cd esp32-read-ec-example

#2 run platformio build and upload on esp32
pio run -t upload

#3 debug through serial monitor
pio device monitor

## Example
```C++
#include "Arduino.h"
#include "Adafruit_ADS1015.h"
#include "DFRobot_ESP_EC.h"
#include "EEPROM.h"

DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

float voltage, ecValue, temperature = 25;

void setup()
{
	Serial.begin(115200);
	EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
	ec.begin();//by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30
	ads.setGain(GAIN_ONE);
	ads.begin();
}

void loop()
{
	static unsigned long timepoint = millis();
	if (millis() - timepoint > 1000U) //time interval: 1s
	{

		timepoint = millis();
		voltage = ads.readADC_SingleEnded(0) / 10;
		Serial.print("voltage:");
		Serial.println(voltage, 4);

		//temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
		Serial.print("temperature:");
		Serial.print(temperature, 1);
		Serial.println("^C");

		ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
		Serial.print("EC:");
		Serial.print(ecValue, 4);
		Serial.println("ms/cm");
	}
	ec.calibration(voltage, temperature); // calibration process by Serail CMD
}

float readTemperature()
{
	//add your code here to get the temperature from your temperature sensor
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
[![Tutorial here](http://img.youtube.com/vi/n1EBzMDPI74/0.jpg)](https://www.youtube.com/watch?v=n1EBzMDPI74 "EC Meter with ESP32 and DFRobot EC module (DFR0300)")

### Suscribe on our newsletter here : 
en: http://bit.ly/2NuaKbN

fr: http://bit.ly/2XNf61R

https://www.greenponik.com


## support us
[become a patreon](https://www.patreon.com/bePatron?u=32614023)
