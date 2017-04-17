#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/time.h>

int freadint(int* val, FILE* file);
int freaduint(unsigned int* val, FILE* file);

int strcicmp(char const *a, char const *b);
unsigned int crc32(unsigned char *data, unsigned int size);

typedef unsigned long usecs;

usecs tmr_start(void);
usecs tmr_check(usecs start);

#endif
