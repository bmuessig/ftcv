#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stropts.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

int fbavail(FILE* stream);
int fflushi(FILE* stream);
int fwritebyte(unsigned char value, FILE* file);
int fwriteint(int value, FILE* file);
int fwriteuint(unsigned int value, FILE* file);
int freadbyte(unsigned char* value, unsigned int timeout, FILE* file);
int freaduint(unsigned int* value, unsigned int timeout, FILE* file);
int freadint(int* value, unsigned int timeout, FILE* file);
size_t freadt(void* ptr, size_t size, size_t count, unsigned int timeout, FILE* stream);
int fsetblocking(int isBlocking, FILE* file);

int strcicmp(char const* a, char const* b);
unsigned int crc32(unsigned char* data, unsigned int size);
unsigned int fcrc32(FILE* file);

typedef unsigned long usecs;

usecs tmr_start(void);
usecs tmr_check(usecs start);

#endif
