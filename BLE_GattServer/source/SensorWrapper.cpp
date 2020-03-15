#include "SensorWrapper.h"

void SensorWrapper::convertFloatToUint8(float value, uint8_t dst[2]) {
  uint16_t src = (uint16_t)(value * 100); // resolution of 2
  dst[0] = (uint8_t)(src & 0x00FF);
  dst[1] = (uint8_t)((src >> 8) & 0xFF);
}
