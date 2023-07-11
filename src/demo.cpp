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
#define DIST_THRESHOLD 5
#define TEMP_THRESHOLD 2

#define MAX_WATER_TEMP 50 // celcius, translates to 122 deg farenheit
#define MIN_WATER_TEMP 0  // freezing point
#define MAX_DIST 450
#define MIN_DIST 50

#define PUMP_CROSSSECTIONAL_AREA_MM 1963.5
#define MM_TO_L_CONVERT 1000000

#define ACTIVE 1
#define INACTIVE 0

// Components.
VL53L4CD sensor_vl53l4cd_sat(&Wire, PIN_XSHUT);
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

    // Initialize serial for output.
    Serial.begin(BAUD);
    Serial.println("Starting...");

    // Initialize I2C bus.
    Wire.begin(PIN_SDA, PIN_SCL);

    // initialize the sensor
    if (!mlx.begin(MLX90614_I2CADDR, &Wire)) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");
        ESP.restart();
    };

    Serial.print("Emissivity = ");
    Serial.println(mlx.readEmissivity());
    Serial.println("================================================");

    // Init ADC
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

    Serial.println("Begin ok!");
}

float getVolume_L(int pumpDist, int active){
    if(active && ((prevDist - pumpDist) > DIST_THRESHOLD)){
        return ((prevDist-pumpDist) * PUMP_CROSSSECTIONAL_AREA_MM)/MM_TO_L_CONVERT;
    }
    else{
        return 0;
    }

}

int sanityCheck(int dist, int ambTemp, int objTemp){
    int deltaDist = dist - prevDist;
    int deltaTemp = objTemp - ambTemp;

    if((objTemp > MAX_WATER_TEMP) || (objTemp < MIN_WATER_TEMP)){
        // temperature out of normal range
        return INACTIVE;
    }

    if((abs(deltaDist) > DIST_THRESHOLD) && (abs(deltaTemp) > TEMP_THRESHOLD)){
        return ACTIVE;
    }

    return INACTIVE;
}

/* **************USED FOR REALTERM OR PUTTY - NOT PLATFORMIO SERIAL MONITOR ************/
/*                          MATCHING BAUD & ANSI-VT100                  */

// clear the contents of terminal screen
void clearTerminal(void){
    Serial.write(27);
    Serial.print("[2J");
    Serial.write(27);
    Serial.print("[H");
}

// Sets the position of the cursor to the beginnig of the line
void newLine(void){
    Serial.write(27);
    Serial.print("[70D");
}

// print the information to external terminal
void printInformation(int dist, int adcVol, int ambientTemp, int objectTemp, int active){

    // clear the terminal screen
    clearTerminal();
    
    // print the information
    Serial.printf("*************** WORKING WELL ***************\n");
    newLine();
    Serial.printf("Current Distance: %d mm\n", dist);
    newLine();
    Serial.printf("ADC: %d\n", adcVol);
    newLine();
    Serial.printf("Total Volume Pumped: %f L \n\n", totalVolume_L);
    newLine();
    Serial.printf("*************** PUMP STATUS **************** \n\n");

    newLine();
    Serial.printf("\t\t");
    if(active){
        Serial.println("ACTIVE");
    }
    else{
        Serial.println("INACTIVE");
    }
    Serial.printf("\n");
    newLine();
    Serial.printf("******************************************* ");
}
/* ************************************************************************** */

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

            if((results.distance_mm > MIN_DIST) && (results.distance_mm < MAX_DIST)){
            // distance measurement is within expected range
                int adc = read_ext_adc();
                double amb = mlx.readAmbientTempC();
                double obj = mlx.readObjectTempC();

                active = sanityCheck(results.distance_mm, amb, obj);
                totalVolume_L += getVolume_L(results.distance_mm, active);
                
                //printInformation(results.distance_mm, adc, amb, obj, active);

                prevDist = results.distance_mm;
                Serial.printf("MEAS:%d,%d,%f,%f, %f,%d\n", results.distance_mm, adc, amb, obj, totalVolume_L, active);
            }
        }
    }
}