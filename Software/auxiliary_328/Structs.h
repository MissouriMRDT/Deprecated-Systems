#ifndef STRUCTS_H_
#define STRUCTS_H_

struct GPS_Data
{
  bool fix;
  uint8_t fixquality;
  uint8_t satellites;
  uint32_t latitude_fixed;
  uint32_t longitude_fixed;
  float altitude;
  float speed;
  float angle;
};

#endif
