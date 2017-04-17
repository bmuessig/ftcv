#ifndef UTILS_H
#define UTILS_H

#include <string.h>

int strcicmp(char const *a, char const *b);
unsigned int crc32(unsigned char *data, unsigned int size);

typedef union {
  unsigned int intval;
  unsigned char byteval[sizeof(intval)];
} uintbyte;

#endif
