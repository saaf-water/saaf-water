## DFRobot_EC.py Library for Raspberry pi
---------------------------------------------------------
This is the sample code for Gravity: Analog Electrical Conductivity Sensor / Meter Kit V2 (K=1.0), SKU: DFR0300.
## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
<snippet>
<content>

## Installation
The Analog Electrical Conductivity Sensor should work with ADS1115
(https://github.com/DFRobot/DFRobot_ADS1115/tree/master/RaspberryPi/Python) 

Run the program:

```cpp

$> python DFRobot_ADS1115.py

$> python DFRobot_EC.py

```
## Methods

```C++

/*
 * @brief Init The Analog Electrical Conductivity Sensor
 */
def begin(self);

/*
 * @brief Convert voltage to EC with temperature compensation
 */
def readEC(self,voltage,temperature);

/*
 * @brief Calibrate the calibration data
 */
def calibration(self,voltage,temperature);

/*
 * @brief Reset the calibration data to default value
 */
def reset(self);

```
## Credits

Written by Jiawei Zhang, 2018. (Welcome to our [website](https://www.dfrobot.com/))
