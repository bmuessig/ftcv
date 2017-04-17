#include "utils.h"

int freadint(int* val, FILE* file)
{
  return fread(val, sizeof(int), 1, file) > 0;
}

int freaduint(unsigned int* val, FILE* file)
{
  return fread(val, sizeof(unsigned int), 1, file) > 0;
}

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
    byte = data[i];            // Get next byte.
    crc = crc ^ byte;
    for(j = 7; j >= 0; j--) {    // Do eight times.
      mask = -(crc & 1);
      crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }
  }

  return ~crc;
}

usecs tmr_start(void)
{
  struct timeval v;
  gettimeofday(&v, (struct timezone*)0);
  return (v.tv_usec + (v.tv_sec * 1000000));
}

usecs tmr_check(usecs start)
{
  struct timeval now;
  gettimeofday(&now, (struct timezone*)0);
  return (now.tv_usec + (now.tv_sec * 1000000)) - start;
}
