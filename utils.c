#include "utils.h"

int fbavail(FILE* stream)
{
  int num, res;
  if(!stream)
    return -1;
  if((res = ioctl(fileno(stream), I_NREAD, &num)) == 0)
    return num;
  else
    return -1;
}

int fflushi(FILE* stream)
{
  int num, fp;
  if(!stream)
    return false;
  if((fp = fileno(stream)) != 0)
    return false;
  while(num > 0) {
    if(ioctl(fp, I_NREAD, &num) != 0)
      return false;
    if(fgetc(stream) == EOF)
      break;
  }
  return true;
}

int fwritebyte(unsigned char value, FILE* file)
{
  return fwrite(&value, sizeof(unsigned int), 1, file);
}

int fwriteint(int value, FILE* file)
{
  return fwrite(&value, sizeof(int), 1, file);
}

int fwriteuint(unsigned int value, FILE* file)
{
  return fwrite(&value, sizeof(unsigned int), 1, file);
}

int freadbyte(unsigned char* value, unsigned int timeout, FILE* file)
{
  return freadt(value, sizeof(unsigned char), 1, timeout, file);
}

int freadint(int* value, unsigned int timeout, FILE* file)
{
  return freadt(value, sizeof(int), 1, timeout, file);
}

int freaduint(unsigned int* value, unsigned int timeout, FILE* file)
{
  return freadt(value, sizeof(unsigned int), 1, timeout, file);
}

size_t freadt(void* ptr, size_t size, size_t count, unsigned int timeout, FILE* stream)
{
  if(timeout) {
    usecs start = tmr_start();
    int timedOut;

    // Pause program execution while the bytes arrive, but max. for timeout microsecs
    while(!(timedOut = tmr_check(start) >= timeout) && fbavail(stream) < size * count);

    if(timedOut) {
      errno = ETIME;
      return 0;
    }
  }

  return fread(ptr, size, count, stream);
}

int fsetblocking(int isBlocking, FILE* file)
{
  int fd, flags;
  if(!file)
    return false;
  if((fd = fileno(file)) == -1)
    return false;
  flags = fcntl(fd, F_GETFL, 0);
  if(isBlocking)
    flags &= ~O_NONBLOCK;
  else
    flags |= O_NONBLOCK;

  return (fcntl(fd, F_SETFL, flags) == 0);
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
