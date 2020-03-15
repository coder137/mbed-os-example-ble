#include "SensorProcess.h"

static void printUint8AsHex(const char * id, uint8_t *value);

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

    // Update the GATT Server
    _server->write(_temperature_characteristic.getValueHandle(), _temperature_value, 2);
    _server->write(_humidity_characteristic.getValueHandle(), _humidity_value, 2);
    _server->write(_pressure_characteristic.getValueHandle(), _pressure_value, 2);

    // ? debugging
    printf("Updated: %d\r\n", counter++);
    printUint8AsHex("Pressure", _pressure_value);
    printUint8AsHex("Humidity", _humidity_value);
    printUint8AsHex("Temperature", _temperature_value);
}

/**
 * 
 * Static functions
 */
// ? debugging
static void printUint8AsHex(const char * id, uint8_t *value) {
  printf("%s -> %x %x\r\n", id, value[0], value[1]);
}

