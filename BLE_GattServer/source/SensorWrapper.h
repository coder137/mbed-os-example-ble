#pragma once

#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_tsensor.h"

class SensorWrapper {
public:
  SensorWrapper() : _temp_value(0), _humidity_value(0),
                    _pressure_value(0)
  {}

  void init() {
    BSP_TSENSOR_Init();
    BSP_HSENSOR_Init();
    BSP_PSENSOR_Init();

    BSP_MAGNETO_Init();
    BSP_GYRO_Init();
    BSP_ACCELERO_Init();
  }

  void update() {
    _temp_value = BSP_TSENSOR_ReadTemp();
    _humidity_value = BSP_HSENSOR_ReadHumidity();
    _pressure_value = BSP_PSENSOR_ReadPressure();

    // TODO, 
    // BSP_MAGNETO_GetXYZ(pMagnetoXYZ);
    // BSP_GYRO_GetXYZ(pGyroXYZ);
    // BSP_ACCELERO_AccGetXYZ(pAcceleroXYZ);
  }

  // Should be array of size 2
  void getTemp(uint8_t dst[2]) { convertFloatToUint8(_temp_value, dst);}
  void getHumidity(uint8_t dst[2]) { convertFloatToUint8(_humidity_value, dst);}
  void getPressure(uint8_t dst[2]) { convertFloatToUint8(_pressure_value, dst);}

private:
  void convertFloatToUint8(float value, uint8_t dst[2]);

private:
  float _temp_value = 0;
  float _humidity_value = 0;
  float _pressure_value = 0;

  // TODO, 
  int16_t pMagnetoXYZ[3] = {0};
  float pGyroXYZ[3] = {0};
  int16_t pAcceleroXYZ[3] = {0};
};
