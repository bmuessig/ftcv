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
#include "config.h"

#ifdef DEBUG
#define DBGS(str) fprintf(stderr, "DEBUG: %s\n", str)
#define DBGSS(str1, str2) fprintf(stderr, "DEBUG: %s : %s\n", str1, str2)
#define DBGI(str, num) fprintf(stderr, "DEBUG: %s : %d\n", str, num)
#else
#define DBGS(str)
#define DBGSS(str1, str2)
#define DBGI(str, num)
#endif

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
