#include "utils.h"

// http://stackoverflow.com/a/5820991
int strcicmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a)
            return d;
    }
}

// adapted from http://www.hackersdelight.org/hdcodetxt/crc.c.txt
unsigned int crc32(unsigned char *data, unsigned int size)
{
  unsigned char byte;
  unsigned int i, j, mask, crc = 0xFFFFFFFF;

  for(i = 0; i < size; i++) {
    byte = message[i];            // Get next byte.
    crc = crc ^ byte;
    for(j = 7; j >= 0; j--) {    // Do eight times.
      mask = -(crc & 1);
      crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }
  }

  return ~crc;
}
