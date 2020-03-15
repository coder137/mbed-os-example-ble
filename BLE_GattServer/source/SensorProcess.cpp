#include "SensorProcess.h"

// only 2 values allowed in this one
static void printUint8AsHex(const char * id, uint8_t *value);

static void printUint8AsHex(const char * id, uint8_t * value, int size);

/**
 * Increment the second counter.
 */
// DONE, Update this function
void SensorService::update_sensor(void)
{
    // ? debugging
    static int counter = 0;
    printFunction();

    // Update the values here
    _sensor_wrapper.update();

    // Convert and get the values here
    _sensor_wrapper.getTemp(_temperature_value);
    _sensor_wrapper.getHumidity(_humidity_value);
    _sensor_wrapper.getPressure(_pressure_value);

    _sensor_wrapper.getMagneto(_magneto_value);
    _sensor_wrapper.getAccelero(_accelero_value);
    _sensor_wrapper.getGyro(_gyro_value);

    // Update the GATT Server
    _server->write(_temperature_characteristic.getValueHandle(), _temperature_value, 2);
    _server->write(_humidity_characteristic.getValueHandle(), _humidity_value, 2);
    _server->write(_pressure_characteristic.getValueHandle(), _pressure_value, 2);

    _server->write(_magneto_characteristic.getValueHandle(), _magneto_value, 6);
    _server->write(_accelero_characteristic.getValueHandle(), _accelero_value, 6);
    _server->write(_gyro_characteristic.getValueHandle(), _gyro_value, 6);

    // ? debugging
    printf("Updated: %d\r\n", counter++);
    
    // printUint8AsHex("Accelero", _accelero_value, 6);
    // printUint8AsHex("Magneto", _magneto_value, 6);
    // printUint8AsHex("Pressure", _pressure_value);
    // printUint8AsHex("Humidity", _humidity_value);
    // printUint8AsHex("Temperature", _temperature_value);
}

/**
 * 
 * Static functions
 */
// ? debugging
static void printUint8AsHex(const char * id, uint8_t *value) {
  printf("%s -> %x %x\r\n", id, value[0], value[1]);
}

static void printUint8AsHex(const char * id, uint8_t * value, int size) {
  printf("%s -> ", id);

  for (int i=0;i<size;i++) {
    printf("%x ", value[i]);
  }

  printf("\r\n");
}


