#include "SensorWrapper.h"

#define DEBUG 0

#if DEBUG == 1
#include <stdio.h>
#endif

// int16 (xyz) -> uint8 arr
void SensorWrapper::getMagneto(uint8_t dst[6]) {
  convertInt16toUint8(_pMagnetoXYZ[0], dst);    
  convertInt16toUint8(_pMagnetoXYZ[1], (dst + 2));
  convertInt16toUint8(_pMagnetoXYZ[2], (dst + 4));

  #if DEBUG == 1
    printf("%d %d %d\r\n", _pMagnetoXYZ[0],_pMagnetoXYZ[1],_pMagnetoXYZ[2]);
    printf("%x %x %x\r\n", _pMagnetoXYZ[0],_pMagnetoXYZ[1],_pMagnetoXYZ[2]);
  #endif
}

void SensorWrapper::getAccelero(uint8_t dst[6]){
  convertInt16toUint8(_pAcceleroXYZ[0], dst);    
  convertInt16toUint8(_pAcceleroXYZ[1], (dst + 2));
  convertInt16toUint8(_pAcceleroXYZ[2], (dst + 4));

  #if DEBUG == 1
    printf("%d %d %d\r\n", _pAcceleroXYZ[0],_pAcceleroXYZ[1],_pAcceleroXYZ[2]);
    printf("%x %x %x\r\n", _pAcceleroXYZ[0],_pAcceleroXYZ[1],_pAcceleroXYZ[2]);
  #endif
}

// float (xyz) -> uint8 arr
void SensorWrapper::getGyro(uint8_t dst[6]){
  convertFloatToUint8(_pAcceleroXYZ[0], dst);    
  convertFloatToUint8(_pAcceleroXYZ[1], (dst + 2));
  convertFloatToUint8(_pAcceleroXYZ[2], (dst + 4));

  #if DEBUG == 1
    printf("%d %d %d\r\n", _pAcceleroXYZ[0],_pAcceleroXYZ[1],_pAcceleroXYZ[2]);
    printf("%x %x %x\r\n", _pAcceleroXYZ[0],_pAcceleroXYZ[1],_pAcceleroXYZ[2]);
  #endif
}


/**
 * 
 * HELPER FUNCTIONS
 * 
 */
void SensorWrapper::convertFloatToUint8(float value, uint8_t dst[2]) {
  uint16_t src = (uint16_t)(value * 100); // resolution of 2
  dst[0] = (uint8_t)(src & 0x00FF);
  dst[1] = (uint8_t)((src >> 8) & 0xFF);
}

void SensorWrapper::convertInt16toUint8(int16_t value, uint8_t dst[2]) {
  uint16_t src = (uint16_t)(value);
  dst[0] = (uint8_t)(src & 0x00FF);
  dst[1] = (uint8_t)((src >> 8) & 0xFF);
}

