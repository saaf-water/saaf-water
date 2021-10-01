/*
 * file DFRobot_ESP_PH_WITH_ADC.h * @ https://github.com/GreenPonik/DFRobot_ESP_PH_WITH_ADC_BY_GREENPONIK
 *
 * Arduino library for Gravity: Analog pH Sensor / Meter Kit V2, SKU: SEN0161-V2 + ADC Adafruit ADS1115
 * 
 * Based on the @ https://github.com/DFRobot/DFRobot_PH
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
 * version  V1.2.1
 * date  2019-06
 */

#ifndef _DFROBOT_ESP_PH_WITH_ADC_H_
#define _DFROBOT_ESP_PH_WITH_ADC_H_

#include "Arduino.h"

#define PHVALUEADDR 0 //the start address of the pH calibration parameters stored in the EEPROM

/**
 * first you need to define the raw voltage for your circuit
 * the raw voltage for neutral pH 7.0 and acid pH 4.0 at 25 °c
 * for the actual circuit => ESP32 + ADC (ADS1115)
*/
/**
 * you may have to adapt voltage acid and neutral offset or directly the ranges
 * according to the signal sending by your own pH probe if you don't use the DFRobot pH sensor kit probe
 * same circuit with different probe return different voltage value for buffer4.0 or buffer 7.0
 */
#define PH_VOLTAGE_ACID_OFFSET 200
#define PH_VOLTAGE_NEUTRAL_OFFSET 200
#define PH_8_VOLTAGE 995  //linear culculation
#define PH_7_AT_25 1134   //laboratory measurement with isolation circuit, PH meter V2.0 and PH probe from DFRobot kit
#define PH_6_VOLTAGE 1250 //linear culculation
#define PH_5_VOLTAGE 1380 //linear culculation
#define PH_4_AT_25 1521   //laboratory measurement with isolation circuit, PH meter V2.0 and PH probe from DFRobot kit
#define PH_3_VOLTAGE 1700 //linear culculation

#define PH_VOLTAGE_NEUTRAL_LOW_LIMIT PH_8_VOLTAGE - PH_VOLTAGE_NEUTRAL_OFFSET
#define PH_VOLTAGE_NEUTRAL_HIGH_LIMIT PH_6_VOLTAGE
#define PH_VOLTAGE_ACID_LOW_LIMIT PH_5_VOLTAGE - PH_VOLTAGE_ACID_OFFSET
#define PH_VOLTAGE_ACID_HIGH_LIMIT PH_3_VOLTAGE

#define ReceivedBufferLength 10 //length of the Serial CMD buffer

class DFRobot_ESP_PH_WITH_ADC
{
public:
    DFRobot_ESP_PH_WITH_ADC();
    ~DFRobot_ESP_PH_WITH_ADC();
    void calibration(float voltage, float temperature, char *cmd); //calibration by Serial CMD
    void calibration(float voltage, float temperature);
    float readPH(float voltage, float temperature);   // voltage to pH value, with temperature compensation
    void begin(int EepromStartAddress = PHVALUEADDR); //initialization

private:
    float _phValue;
    float _acidVoltage;
    float _neutralVoltage;
    float _voltage;
    float _temperature;

    char _cmdReceivedBuffer[ReceivedBufferLength]; //store the Serial CMD
    byte _cmdReceivedBufferIndex;

private:
    int _eepromStartAddress;
    boolean cmdSerialDataAvailable();
    void phCalibration(byte mode); // calibration process, wirte key parameters to EEPROM
    byte cmdParse(const char *cmd);
    byte cmdParse();
};

#endif
