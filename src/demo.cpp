/** @file   demo.cpp
 *  This file demonstrates the functionality of a monitoring system for the 
 *  Afridev hand pump model. The code estimates volumetric water output using
 *  distance measurements from the VL53L4CX time-of-flight sensor and detects
 *  presence of water using the MLX90614 infrared sensor. The sensor data is 
 *  transmitted through satellite using the STX3 module from GlobalStar.
 * 
 *  @author Juan Luna
 *  @date   2023-Jun-19
 */

#include "demo.h"

// Baud rate for serial communication
#define BAUD 115200
// Pin number for I2C SDA (data) line
#define PIN_SDA 3
// Pin number for I2C SCL (clock) line
#define PIN_SCL 2
// Pin number for VL53L4CD XSHUT (shutdown) line
#define PIN_XSHUT 4
// Pin number for VL53L4CD IRQ (interrupt) line
#define PIN_IRQ 5
// I2C address of MCP3421 ADC (ref 1)
#define MCP_ADDR 0x68

// Timing budget (ms) for VL53L4CD ranging
#define TIMING_BUDGET_MS 100
// Maximum value of ADC reading (10-bit)
#define ADC_MAX 2047
// Adjustment value for negative ADC readings
#define ADC_ADJUST 4095

// MEASUREMENT RANGES
// Distance threshold in millimeters
#define DIST_THRESHOLD 5
// Temperature threshold in degrees Celsius
#define TEMP_THRESHOLD 2

// Maximum water temperature in Celsius
#define MAX_WATER_TEMP 50 // equivalent to 122-F
// Minimum water temperature in Celsius
#define MIN_WATER_TEMP 0  // freezing point
// Maximum distance in millimeters
#define MAX_DIST 450
// Minimum distance in millimeters
#define MIN_DIST 50

// Cross-sectional area of pump in square millimeters
#define PUMP_AREA 1963.5
// Conversion factor from cubic millimeters to liters
#define MMC_TO_L 1000000

// Active state
#define ACTIVE 1
// Inactive state
#define INACTIVE 0

// SENSOR OBJECTS
// VL53L4CD distance sensor using I2C and XSHUT pin
VL53L4CD sensor_vl53l4cd_sat(&Wire, PIN_XSHUT);
// MLX90614 temperature sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

volatile int interruptCount = 0;

float totalVolume_L = 0.0;
int prevDist = 0;
int active = 0;
int maxDist = 0;
int minDist = 10000;

void measure() {
    interruptCount = 1;
}

int read_ext_adc() {
    // Ref 1

    unsigned int data[2];

    // Start I2C Transmission
    Wire.beginTransmission(MCP_ADDR);
    // Select data register
    Wire.write(0x00);
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 2 bytes of data
    Wire.requestFrom(MCP_ADDR, 2);

    // Read 2 bytes of data
    // raw_adc msb, raw_adc lsb
    if (Wire.available() == 2) {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    // Convert the data to 12-bits
    int raw_adc = ((data[0] & 0x0F) << 8) + data[1];
    if (raw_adc > ADC_MAX) {
        raw_adc -= ADC_ADJUST;
    }

    return raw_adc;
}

void setup() {
    // Interrupt
    pinMode(PIN_IRQ, INPUT_PULLUP);
    attachInterrupt(PIN_IRQ, measure, FALLING);

    // Initialize serial for output
    Serial.begin(BAUD);
    Serial << "Starting..." << endl;

    // Initialize I2C bus.
    Wire.begin(PIN_SDA, PIN_SCL);

    // Initialize IR sensor
    if (!mlx.begin(MLX90614_I2CADDR, &Wire)) {
        Serial << "Error connecting to IR sensor. Check wiring." << endl;
        ESP.restart();
    };

    Serial << "Emissivity = " << mlx.readEmissivity() << endl;
    Serial << "================================================";

    // Initialize ADC
    Wire.beginTransmission(MCP_ADDR);
    // Send configuration command
    // Continuous conversion mode, 12-bit resolution
    Wire.write(0x10);
    // Stop I2C Transmission
    Wire.endTransmission();

    // Configure VL53L4CD satellite component.
    sensor_vl53l4cd_sat.begin();
    // Switch off VL53L4CD satellite component.
    sensor_vl53l4cd_sat.VL53L4CD_Off();
    // Initialize VL53L4CD satellite component.
    sensor_vl53l4cd_sat.InitSensor();

    // Program the highest possible TimingBudget, without enabling the
    // low power mode. This should give the best accuracy
    sensor_vl53l4cd_sat.VL53L4CD_SetRangeTiming(TIMING_BUDGET_MS, 0);

    // Start Measurements
    sensor_vl53l4cd_sat.VL53L4CD_StartRanging();

    Serial << "Successfully started." << endl;
}

float getVolume_L(int pumpDist, int active) {
    if (active && ((prevDist - pumpDist) > DIST_THRESHOLD)) {
        return ((prevDist-pumpDist) * PUMP_AREA)/MMC_TO_L;
    }
    else {
        return 0;
    }
}

int statusCheck (int dist, int ambTemp, int objTemp) {
    int deltaDist = dist - prevDist;
    int deltaTemp = objTemp - ambTemp;

    if ((objTemp > MAX_WATER_TEMP) || (objTemp < MIN_WATER_TEMP)) {
        // temperature out of normal range
        return INACTIVE;
    }

    if ((abs(deltaDist) > DIST_THRESHOLD) && (abs(deltaTemp) > TEMP_THRESHOLD)) {
        return ACTIVE;
    }

    return INACTIVE;
}

// The following lines of code are used RealTerm or PuTTY, 
// not PlatformIO serial monitor

// Clear the contents of terminal screen
void clearTerminal(void) {
    Serial.write(27);
    Serial.print("[2J");
    Serial.write(27);
    Serial.print("[H");
}

// Sets the position of the cursor to the beginnig of the line
void newLine(void) {
    Serial.write(27);
    Serial.print("[70D");
}

// Print the information to external terminal
void printInformation(int dist, int adcVol, int ambientTemp, int objectTemp, int active) {

    // Clear the terminal screen
    clearTerminal();
    
    // Print the information
    Serial.printf(" MONITORING SYSTEM DEMO \n");
    newLine();
    Serial.printf("Current Distance: %d mm\n", dist);
    newLine();
    Serial.printf("ADC: %d\n", adcVol);
    newLine();
    Serial.printf("Total Volume Pumped: %f L \n\n", totalVolume_L);
    newLine();
    Serial.printf(" PUMP STATUS \n\n");

    newLine();
    Serial.printf("\t\t");
    if (active) {
        Serial.println("ACTIVE");
    }
    else {
        Serial.println("INACTIVE");
    }
    Serial.printf("\n");
    newLine();
    Serial.printf("******************************************* ");
}

void loop() {
    uint8_t NewDataReady = 0;
    VL53L4CD_Result_t results;
    uint8_t status;
    int active = INACTIVE;

    if (interruptCount) {
        interruptCount = 0;

        status = sensor_vl53l4cd_sat.VL53L4CD_CheckForDataReady(&NewDataReady);

        if ((!status) && (NewDataReady != 0)) {
            // (Mandatory) Clear HW interrupt to restart measurements
            sensor_vl53l4cd_sat.VL53L4CD_ClearInterrupt();

            // Read measured distance. RangeStatus = 0 means valid data
            sensor_vl53l4cd_sat.VL53L4CD_GetResult(&results);

            if((results.distance_mm > MIN_DIST) && (results.distance_mm < MAX_DIST)) {
            // Distance measurement is within expected range
                int adc = read_ext_adc();
                double amb = mlx.readAmbientTempC();
                double obj = mlx.readObjectTempC();

                active = statusCheck(results.distance_mm, amb, obj);
                totalVolume_L += getVolume_L(results.distance_mm, active);
                
                //printInformation(results.distance_mm, adc, amb, obj, active);

                prevDist = results.distance_mm;
                Serial.printf("MEAS:%d,%d,%f,%f, %f,%d\n", results.distance_mm, 
                                        adc, amb, obj, totalVolume_L, active);
            }
        }
    }
}