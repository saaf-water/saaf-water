/*
 * file read_ph.ino
 * @ https://github.com/GreenPonik/DFRobot_ESP_PH_WITH_ADC_BY_GREENPONIK
 *
 * IMPORTANT : to make it work, you will need the help of an additionnal ADC converter because the one on the ESP32 isn't accurate enough. Here we used an ADS1115 from Adafruit
 * You can find it here : https://www.adafruit.com/product/1085
 * And here is the library you'll need to add to your sketch : https://github.com/adafruit/Adafruit_ADS1X15
 * 
 * This is the sample code for Gravity: Analog pH Sensor / Meter Kit V2, SKU:SEN0161-V2
 * In order to guarantee precision, a temperature sensor such as DS18B20 is needed, to execute automatic temperature compensation.
 * You can send commands in the serial monitor to execute the calibration.
 * Serial Commands:
 *   enterph -> enter the calibration mode
 *   calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized
 *   exitph  -> save the calibrated parameters and exit from calibration mode
 * 
 * Based on the @ https://github.com/DFRobot/DFRobot_PH
 * Copyright   [DFRobot](http://www.dfrobot.com), 2018
 * Copyright   GNU Lesser General Public License
 *
 * ##################################################
 * ##################################################
 * ######## Forked on github by GreenPonik ##########
 * ############# ONLY ESP COMPATIBLE ################
 * ##################################################
 * ##################################################
 * 
 * version  V1.2.2
 * date  2019-06
 */

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