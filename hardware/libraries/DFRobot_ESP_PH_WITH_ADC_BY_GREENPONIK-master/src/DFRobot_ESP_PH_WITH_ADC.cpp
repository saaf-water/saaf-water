/*
 * file DFRobot_ESP_PH_WITH_ADC.cpp * @ https://github.com/GreenPonik/DFRobot_ESP_PH_WITH_ADC_BY_GREENPONIK
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

#include "Arduino.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "EEPROM.h"

DFRobot_ESP_PH_WITH_ADC::DFRobot_ESP_PH_WITH_ADC()
{
    this->_temperature = 25.0;
    this->_phValue = 7.0;
    this->_acidVoltage = PH_4_AT_25;    //buffer solution 4.0 at 25C
    this->_neutralVoltage = PH_7_AT_25; //buffer solution 7.0 at 25C
    this->_voltage = PH_7_AT_25;
}

DFRobot_ESP_PH_WITH_ADC::~DFRobot_ESP_PH_WITH_ADC()
{
}

void DFRobot_ESP_PH_WITH_ADC::begin(int EepromStartAddress)
{
    this->_eepromStartAddress = EepromStartAddress;
    //check if calibration values (neutral and acid) are stored in eeprom
    this->_neutralVoltage = EEPROM.readFloat(this->_eepromStartAddress); //load the neutral (pH = 7.0)voltage of the pH board from the EEPROM
    if (this->_neutralVoltage == float() || isnan(this->_neutralVoltage) || isinf(this->_neutralVoltage))
    {
        this->_neutralVoltage = PH_7_AT_25; // new EEPROM, write typical voltage
        EEPROM.writeFloat(this->_eepromStartAddress, this->_neutralVoltage);
        EEPROM.commit();
    }

    this->_acidVoltage = EEPROM.readFloat(this->_eepromStartAddress + (int)sizeof(float)); //load the acid (pH = 4.0) voltage of the pH board from the EEPROM
    if (this->_acidVoltage == float() || isnan(this->_acidVoltage) || isinf(this->_acidVoltage))
    {
        this->_acidVoltage = PH_4_AT_25; // new EEPROM, write typical voltage
        EEPROM.writeFloat(this->_eepromStartAddress + (int)sizeof(float), this->_acidVoltage);
        EEPROM.commit();
    }
}

float DFRobot_ESP_PH_WITH_ADC::readPH(float voltage, float temperature)
{
    // Serial.print("[readPH]... _neutraVoltage:");
    // Serial.print(this->_neutralVoltage);
    // Serial.print(", _acidVoltage:");
    // Serial.print(this->_acidVoltage);
    float slope = (7.0 - 4.0) / ((this->_neutralVoltage - PH_7_AT_25) / 3.0 - (this->_acidVoltage - PH_7_AT_25) / 3.0); // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
    float intercept = 7.0 - slope * (this->_neutralVoltage - PH_7_AT_25) / 3.0;
    // Serial.print(", slope:");
    // Serial.print(slope);
    // Serial.print(", intercept:");
    // Serial.println(intercept);
    this->_phValue = slope * (voltage - PH_7_AT_25) / 3.0 + intercept; //y = k*x + b
    Serial.print(F(">>>phValue "));
    Serial.print(this->_phValue,4);
    Serial.println(F("<<<"));
    return this->_phValue;
}

void DFRobot_ESP_PH_WITH_ADC::calibration(float voltage, float temperature, char *cmd)
{
    this->_voltage = voltage;
    this->_temperature = temperature;
    strupr(cmd);
    phCalibration(cmdParse(cmd)); // if received Serial CMD from the serial monitor, enter into the calibration mode
}

void DFRobot_ESP_PH_WITH_ADC::calibration(float voltage, float temperature)
{
    this->_voltage = voltage;
    this->_temperature = temperature;
    if (cmdSerialDataAvailable() > 0)
    {
        phCalibration(cmdParse()); // if received Serial CMD from the serial monitor, enter into the calibration mode
    }
}

boolean DFRobot_ESP_PH_WITH_ADC::cmdSerialDataAvailable()
{
    char cmdReceivedChar;
    static unsigned long cmdReceivedTimeOut = millis();
    while (Serial.available() > 0)
    {
        if (millis() - cmdReceivedTimeOut > 500U)
        {
            this->_cmdReceivedBufferIndex = 0;
            memset(this->_cmdReceivedBuffer, 0, (ReceivedBufferLength));
        }
        cmdReceivedTimeOut = millis();
        cmdReceivedChar = Serial.read();
        if (cmdReceivedChar == '\n' || this->_cmdReceivedBufferIndex == ReceivedBufferLength - 1)
        {
            this->_cmdReceivedBufferIndex = 0;
            strupr(this->_cmdReceivedBuffer);
            return true;
        }
        else
        {
            this->_cmdReceivedBuffer[this->_cmdReceivedBufferIndex] = cmdReceivedChar;
            this->_cmdReceivedBufferIndex++;
        }
    }
    return false;
}

byte DFRobot_ESP_PH_WITH_ADC::cmdParse(const char *cmd)
{
    byte modeIndex = 0;
    if (strstr(cmd, "ENTERPH") != NULL)
    {
        modeIndex = 1;
    }
    else if (strstr(cmd, "EXITPH") != NULL)
    {
        modeIndex = 3;
    }
    else if (strstr(cmd, "CALPH") != NULL)
    {
        modeIndex = 2;
    }
    return modeIndex;
}

byte DFRobot_ESP_PH_WITH_ADC::cmdParse()
{
    byte modeIndex = 0;
    if (strstr(this->_cmdReceivedBuffer, "ENTERPH") != NULL)
    {
        modeIndex = 1;
    }
    else if (strstr(this->_cmdReceivedBuffer, "EXITPH") != NULL)
    {
        modeIndex = 3;
    }
    else if (strstr(this->_cmdReceivedBuffer, "CALPH") != NULL)
    {
        modeIndex = 2;
    }
    return modeIndex;
}

void DFRobot_ESP_PH_WITH_ADC::phCalibration(byte mode)
{
    char *receivedBufferPtr;
    static boolean phCalibrationFinish = 0;
    static boolean enterCalibrationFlag = 0;
    switch (mode)
    {
    case 0:
        if (enterCalibrationFlag)
        {
            Serial.println(F(">>>Command Error<<<"));
        }
        break;

    case 1:
        enterCalibrationFlag = 1;
        phCalibrationFinish = 0;
        Serial.println();
        Serial.println(F(">>>Enter PH Calibration Mode<<<"));
        Serial.println(F(">>>Please put the probe into the 4.0 or 7.0 standard buffer solution<<<"));
        Serial.println();
        break;

    case 2:
        if (enterCalibrationFlag)
        {
            // buffer solution:7.0
            // 7795 to 1250
            if ((this->_voltage > PH_VOLTAGE_NEUTRAL_LOW_LIMIT) && (this->_voltage < PH_VOLTAGE_NEUTRAL_HIGH_LIMIT))
            {
                Serial.println();
                Serial.print(F(">>>Buffer Solution:7.0"));
                this->_neutralVoltage = this->_voltage;
                Serial.println(F(",Send EXITPH to Save and Exit<<<"));
                Serial.println();
                phCalibrationFinish = 1;
            }
            //buffer solution:4.0
            //1180 to 1700
            else if ((this->_voltage > PH_VOLTAGE_ACID_LOW_LIMIT) && (this->_voltage < PH_VOLTAGE_ACID_HIGH_LIMIT))
            {
                Serial.println();
                Serial.print(F(">>>Buffer Solution:4.0"));
                this->_acidVoltage = this->_voltage;
                Serial.println(F(",Send EXITPH to Save and Exit<<<"));
                Serial.println();
                phCalibrationFinish = 1;
            }
            else
            {
                Serial.println();
                Serial.print(F(">>>Buffer Solution Error Try Again<<<"));
                Serial.println(); // not buffer solution or faulty operation
                phCalibrationFinish = 0;
            }
        }
        break;

    case 3: //store calibration value in eeprom
        if (enterCalibrationFlag)
        {
            Serial.println();
            if (phCalibrationFinish)
            {
                // buffer solution:7.0
                // 7795 to 1250
                if ((this->_voltage > PH_VOLTAGE_NEUTRAL_LOW_LIMIT) && (this->_voltage < PH_VOLTAGE_NEUTRAL_HIGH_LIMIT))
                {
                    EEPROM.writeFloat(this->_eepromStartAddress, this->_neutralVoltage);
                    EEPROM.commit();
                }
                //buffer solution:4.0
                //1180 to 1700
                else if ((this->_voltage > PH_VOLTAGE_ACID_LOW_LIMIT) && (this->_voltage < PH_VOLTAGE_ACID_HIGH_LIMIT))
                {
                    EEPROM.writeFloat(this->_eepromStartAddress + (int)sizeof(float), this->_acidVoltage);
                    EEPROM.commit();
                }
                Serial.print(F(">>>Calibration Successful"));
            }
            else
            {
                Serial.print(F(">>>Calibration Failed"));
            }
            Serial.println(F(",Exit PH Calibration Mode<<<"));
            Serial.println();
            phCalibrationFinish = 0;
            enterCalibrationFlag = 0;
        }
        break;
    }
}
