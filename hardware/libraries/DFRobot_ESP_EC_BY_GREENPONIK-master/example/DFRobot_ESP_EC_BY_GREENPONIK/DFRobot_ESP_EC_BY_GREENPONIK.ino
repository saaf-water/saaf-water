/*
 * file DFRobot_ESP_EC_BY_GREENPONIK.ino
 * @ https://github.com/GreenPonik/DFRobot_ESP_EC_BY_GREENPONIK
 *
 * This is the sample code for Gravity: Analog EC Sensor / Meter Kit V2, SKU:DFR300
 *  * In order to guarantee precision, a temperature sensor such as DS18B20 is needed, to execute automatic temperature compensation.
 * You can send commands in the serial monitor to execute the calibration.
 * Serial Commands:
 *   enterec -> enter the calibration mode
 *   calec   -> calibrate with the standard buffer solution, two buffer solutions(1413us/cm and 12.88ms/cm) will be automaticlly recognized
 *   exitec  -> save the calibrated parameters and exit from calibration mode
 * 
 * Based on the @ https://github.com/DFRobot/DFRobot_EC
 * Copyright   [DFRobot](http://www.dfrobot.com), 2018
 * Copyright   GNU Lesser General Public License
 *
 * ##################################################
 * ##################################################
 * ########## Fork on github by GreenPonik ##########
 * ############# ONLY ESP COMPATIBLE ################
 * ##################################################
 * ##################################################
 * 
 * version  V1.1.2
 * date  2019-06
 */

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
