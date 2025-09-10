#ifndef DATATYPES_H
#define DATATYPES_H

/**
*@def RMA_TYPES
*
*@brief OpenShemem RMA routine types
*
*List of data type that specific RMA routines in the OpenSHMEM spec must support
*/
#define RMA_TYPES         \
  X(float)                \
  X(double)               \
  X(long double)          \
  X(char)                 \
  X(signed char)          \
  X(short)                \
  X(int)                  \
  X(long)                 \
  X(long long)            \
  X(unsigned char)        \
  X(unsigned short)       \
  X(unsigned int)         \
  X(unsigned long)        \
  X(unsigned long long)   \
  X(int8_t)               \
  X(int16_t)              \
  X(int32_t)              \
  X(int64_t)              \
  X(uint8_t)              \
  X(uint16_t)             \
  X(uint32_t)             \
  X(uint64_t)             \
  X(size_t)               \
  X(ptrdiff_t)


#endif