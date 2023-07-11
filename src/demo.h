/** @file   demo.cpp
 *  This file demonstrates the functionality of a monitoring system for the 
 *  Afridev hand pump model. The code estimates volumetric water output using
 *  distance measurements from the VL53L4CX time-of-flight sensor and detects
 *  presence of water using the MLX90614 infrared sensor. The sensor data is 
 *  transmitted through satellite using the STX3 module from GlobalStar.
 * 
 *  References:
 *      1. MCP3425_I2CADC I2C mini module:
 *         https://github.com/ControlEverythingCommunity/MCP3425/blob/master/Arduino/MCP3425.ino
 *      2. L53L4CX Time-of-Flight ranging sensor:
 *         https://github.com/stm32duino/VL53L4CX 
 *      3. Adafruit MLX90614 IR sensor:
 *         https://github.com/adafruit/Adafruit-MLX90614-Library
 * 
 *  @author Juan Luna
 *  @date   2023-Jun-19
 */

#include <Arduino.h> // Main Arduino library
#include <SPI.h> // SPI communication protocol
#include <Wire.h> // I2C communication protocol
#include <assert.h> // Library for the assert function
#include <stdint.h> // Library for fixed-width integer types
#include <stdio.h> // Library for standard input/output functions
#include <stdlib.h> // Library for general purpose functions
#include <string.h> // Library for string manipulation functions
#include <Adafruit_MLX90614.h> // Adafruit MLX90614 temperature sensor
#include <vl53l4cd_class.h> // VL53L4CD distance sensor