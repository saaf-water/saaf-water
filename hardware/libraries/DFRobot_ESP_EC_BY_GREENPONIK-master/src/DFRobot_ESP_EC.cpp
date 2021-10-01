/*
 * file DFRobot_ESP_EC.cpp * @ https://github.com/GreenPonik/DFRobot_ESP_EC_BY_GREENPONIK
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

#include "Arduino.h"
#include "DFRobot_ESP_EC.h"
#include "EEPROM.h"

#define RES2 820.0
#define ECREF 200.0

DFRobot_ESP_EC::DFRobot_ESP_EC()
{
    this->_ecvalue = 0.0;
    this->_kvalue = 1.0;
    this->_kvalueLow = 1.0;
    this->_kvalueHigh = 1.0;
    this->_cmdReceivedBufferIndex = 0;
    this->_voltage = 0.0;
    this->_temperature = 25.0;
}

DFRobot_ESP_EC::~DFRobot_ESP_EC()
{
}

void DFRobot_ESP_EC::begin(int EepromStartAddress)
{
    this->_eepromStartAddress = EepromStartAddress;
    //check if calibration values (kvalueLow and kvalueHigh) are stored in eeprom
    this->_kvalueLow = EEPROM.readFloat(this->_eepromStartAddress); //read the calibrated K value from EEPROM
    if (this->_kvalueLow == float() || isnan(this->_kvalueLow) || isinf(this->_kvalueLow))
    {
        this->_kvalueLow = 1.0; // For new EEPROM, write default value( K = 1.0) to EEPROM
        EEPROM.writeFloat(this->_eepromStartAddress, this->_kvalueLow);
        EEPROM.commit();
    }

    this->_kvalueHigh = EEPROM.readFloat(this->_eepromStartAddress + (int)sizeof(float)); //read the calibrated K value from EEPROM
    if (this->_kvalueHigh == float() || isnan(this->_kvalueHigh) || isinf(this->_kvalueHigh))
    {
        this->_kvalueHigh = 1.0; // For new EEPROM, write default value( K = 1.0) to EEPROM
        EEPROM.writeFloat(this->_eepromStartAddress + (int)sizeof(float), this->_kvalueHigh);
        EEPROM.commit();
    }
    this->_kvalue = this->_kvalueLow; // set default K value: K = kvalueLow
}

float DFRobot_ESP_EC::readEC(float voltage, float temperature)
{
    float value = 0, valueTemp = 0;
    this->_rawEC = 1000 * voltage / RES2 / ECREF;
    Serial.print(F(">>>rawEC: "));
    Serial.print(this->_rawEC, 4);
    valueTemp = this->_rawEC * this->_kvalue;
    //automatic shift process
    //First Range:(0,2); Second Range:(2,20)
    if (valueTemp > 2.5)
    {
        this->_kvalue = this->_kvalueHigh;
    }
    else if (valueTemp < 2.0)
    {
        this->_kvalue = this->_kvalueLow;
    }

    value = this->_rawEC * this->_kvalue;                  //calculate the EC value after automatic shift
    value = value / (1.0 + 0.0185 * (temperature - 25.0)); //temperature compensation
    this->_ecvalue = value;                                //store the EC value for Serial CMD calibration
    Serial.print(F(", ecValue: "));
    Serial.print(this->_ecvalue, 4);
    Serial.println(F("<<<"));
    return this->_ecvalue;
}

void DFRobot_ESP_EC::calibration(float voltage, float temperature, char *cmd)
{
    this->_voltage = voltage;
    this->_temperature = temperature;
    strupr(cmd);
    ecCalibration(cmdParse(cmd)); // if received Serial CMD from the serial monitor, enter into the calibration mode
}

void DFRobot_ESP_EC::calibration(float voltage, float temperature)
{
    this->_voltage = voltage;
    this->_temperature = temperature;
    if (cmdSerialDataAvailable() > 0)
    {
        ecCalibration(cmdParse()); // if received Serial CMD from the serial monitor, enter into the calibration mode
    }
}

boolean DFRobot_ESP_EC::cmdSerialDataAvailable()
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

byte DFRobot_ESP_EC::cmdParse(const char *cmd)
{
    byte modeIndex = 0;
    if (strstr(cmd, "ENTEREC") != NULL)
        modeIndex = 1;
    else if (strstr(cmd, "EXITEC") != NULL)
        modeIndex = 3;
    else if (strstr(cmd, "CALEC") != NULL)
        modeIndex = 2;
    return modeIndex;
}

byte DFRobot_ESP_EC::cmdParse()
{
    byte modeIndex = 0;
    if (strstr(this->_cmdReceivedBuffer, "ENTEREC") != NULL)
        modeIndex = 1;
    else if (strstr(this->_cmdReceivedBuffer, "EXITEC") != NULL)
        modeIndex = 3;
    else if (strstr(this->_cmdReceivedBuffer, "CALEC") != NULL)
        modeIndex = 2;
    return modeIndex;
}

void DFRobot_ESP_EC::ecCalibration(byte mode)
{
    char *receivedBufferPtr;
    static boolean ecCalibrationFinish = 0;
    static boolean enterCalibrationFlag = 0;
    static float compECsolution;
    float KValueTemp;
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
        ecCalibrationFinish = 0;
        Serial.println();
        Serial.println(F(">>>Enter EC Calibration Mode<<<"));
        Serial.println(F(">>>Please put the probe into the 1413us/cm or 2.76ms/cm or 12.88ms/cm buffer solution<<<"));
        Serial.println(F(">>>Only need two point for calibration one low (1413us/com) and one high(2.76ms/cm or 12.88ms/cm)<<<"));
        Serial.println();
        break;

    case 2:
        if (enterCalibrationFlag)
        {
            if ((this->_rawEC > RAWEC_1413_LOW) && (this->_rawEC < RAWEC_1413_HIGH))
            {
                Serial.print(F(">>>Buffer 1.413ms/cm<<<"));                            //recognize 1.413us/cm buffer solution
                compECsolution = 1.413 * (1.0 + 0.0185 * (this->_temperature - 25.0)); //temperature compensation
                Serial.print(F(">>>compECsolution: "));
                Serial.print(compECsolution);
                Serial.println(F("<<<"));
            }
            else if ((this->_rawEC > RAWEC_276_LOW) && (this->_rawEC < RAWEC_276_HIGH))
            {
                Serial.print(F(">>>Buffer 2.76ms/cm<<<"));                            //recognize 2.76ms/cm buffer solution
                compECsolution = 2.76 * (1.0 + 0.0185 * (this->_temperature - 25.0)); //temperature compensation
                Serial.print(F(">>>compECsolution: "));
                Serial.print(compECsolution);
                Serial.println(F("<<<"));
            }
            else if ((this->_rawEC > RAWEC_1288_LOW) && (this->_rawEC < RAWEC_1288_HIGH))
            {
                Serial.print(F(">>>Buffer 12.88ms/cm<<<"));                            //recognize 12.88ms/cm buffer solution
                compECsolution = 12.88 * (1.0 + 0.0185 * (this->_temperature - 25.0)); //temperature compensation
                Serial.print(F(">>>compECsolution: "));
                Serial.print(compECsolution);
                Serial.println(F("<<<"));
            }
            else
            {
                Serial.print(F(">>>Buffer Solution Error Try Again<<<   "));
                ecCalibrationFinish = 0;
            }
            Serial.println();
            Serial.print(F(">>>KValueTemp calculation formule: "));
            Serial.print(F("RES2"));
            Serial.print(F(" * "));
            Serial.print(F("ECREF"));
            Serial.print(F(" * "));
            Serial.print(F("compECsolution"));
            Serial.print(F(" / 1000.0 / "));
            Serial.print(F("voltage"));
            Serial.println(F("<<<"));
            Serial.print(F(">>>KValueTemp calculation: "));
            Serial.print(RES2);
            Serial.print(F(" * "));
            Serial.print(ECREF);
            Serial.print(F(" * "));
            Serial.print(compECsolution);
            Serial.print(F(" / 1000.0 / "));
            Serial.print(this->_voltage);
            Serial.println(F("<<<"));
            KValueTemp = RES2 * ECREF * compECsolution / 1000.0 / this->_voltage; //calibrate the k value
            Serial.println();
            Serial.print(F(">>>KValueTemp: "));
            Serial.print(KValueTemp);
            Serial.println(F("<<<"));
            if ((KValueTemp > 0.5) && (KValueTemp < 2.0))
            {
                Serial.println();
                Serial.print(F(">>>Successful,K:"));
                Serial.print(KValueTemp);
                Serial.println(F(", Send EXITEC to Save and Exit<<<"));
                if ((this->_rawEC > RAWEC_1413_LOW) && (this->_rawEC < RAWEC_1413_HIGH))
                {
                    this->_kvalueLow = KValueTemp;
                    Serial.print(">>>kvalueHigh: ");
                    Serial.print(this->_kvalueLow);
                    Serial.println(F("<<<"));
                }
                else if ((this->_rawEC > RAWEC_276_LOW) && (this->_rawEC < RAWEC_276_HIGH))
                {
                    this->_kvalueHigh = KValueTemp;
                    Serial.print(">>>kvalueHigh: ");
                    Serial.print(this->_kvalueHigh);
                    Serial.println(F("<<<"));
                }
                else if ((this->_rawEC > RAWEC_1288_LOW) && (this->_rawEC < RAWEC_1288_HIGH))
                {
                    this->_kvalueHigh = KValueTemp;
                    Serial.print(">>>kvalueHigh: ");
                    Serial.print(this->_kvalueHigh);
                    Serial.println(F("<<<"));
                }
                ecCalibrationFinish = 1;
            }
            else
            {
                Serial.println();
                Serial.println(F(">>>KValueTemp out of range 0.5-2.0<<<"));
                Serial.print(">>>KValueTemp: ");
                Serial.print(KValueTemp, 4);
                Serial.println("<<<");
                Serial.println(F(">>>Failed,Try Again<<<"));
                Serial.println();
                ecCalibrationFinish = 0;
            }
        }
        break;
    case 3:
        if (enterCalibrationFlag)
        {
            Serial.println();
            if (ecCalibrationFinish)
            {
                if ((this->_rawEC > RAWEC_1413_LOW) && (this->_rawEC < RAWEC_1413_HIGH))
                {
                    EEPROM.writeFloat(this->_eepromStartAddress, this->_kvalueLow);
                    EEPROM.commit();
                }
                else if ((this->_rawEC > RAWEC_276_LOW) && (this->_rawEC < RAWEC_276_HIGH))
                {
                    EEPROM.writeFloat(this->_eepromStartAddress + (int)sizeof(float), this->_kvalueHigh);
                    EEPROM.commit();
                }
                else if ((this->_rawEC > RAWEC_1288_LOW) && (this->_rawEC < RAWEC_1288_HIGH))
                {
                    EEPROM.writeFloat(this->_eepromStartAddress + (int)sizeof(float), this->_kvalueHigh);
                    EEPROM.commit();
                }
                Serial.print(F(">>>Calibration Successful"));
            }
            else
            {
                Serial.print(F(">>>Calibration Failed"));
            }
            Serial.println(F(",Exit EC Calibration Mode<<<"));
            Serial.println();
            ecCalibrationFinish = 0;
            enterCalibrationFlag = 0;
        }
        break;
    }
}
