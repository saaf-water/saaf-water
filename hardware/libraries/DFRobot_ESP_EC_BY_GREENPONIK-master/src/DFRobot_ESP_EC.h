/*
 * file DFRobot_ESP_EC.h * @ https://github.com/GreenPonik/DFRobot_ESP_EC_BY_GREENPONIK
 *
 * Arduino library for Gravity: Analog EC Sensor / Meter Kit V2, SKU: DFR0300
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

#ifndef _DFROBOT_ESP_EC_H_
#define _DFROBOT_ESP_EC_H_

#include "Arduino.h"

#define KVALUEADDR 10 //the start address of the K value stored in the EEPROM
#define RAWEC_1413_LOW 0.70
#define RAWEC_1413_HIGH 1.80
#define RAWEC_276_LOW 1.95
#define RAWEC_276_HIGH 3.2
#define RAWEC_1288_LOW 8
#define RAWEC_1288_HIGH 16.8

#define ReceivedBufferLength 10 //length of the Serial CMD buffer

class DFRobot_ESP_EC
{
public:
    DFRobot_ESP_EC();
    ~DFRobot_ESP_EC();
    void calibration(float voltage, float temperature, char *cmd); //calibration by Serial CMD
    void calibration(float voltage, float temperature);
    float readEC(float voltage, float temperature); // voltage to EC value, with temperature compensation
    void begin(int EepromStartAddress = KVALUEADDR);                                   //initialization

private:
    float _ecvalue;
    float  _kvalue;
    float  _kvalueLow;
    float  _kvalueHigh;
    float  _voltage;
    float  _temperature;
    float  _rawEC;

    char _cmdReceivedBuffer[ReceivedBufferLength]; //store the Serial CMD
    byte _cmdReceivedBufferIndex;

private:
    int _eepromStartAddress;
    boolean cmdSerialDataAvailable();
    void    ecCalibration(byte mode); // calibration process, wirte key parameters to EEPROM
    byte cmdParse(const char *cmd);
    byte cmdParse();
};

#endif
