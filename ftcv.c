#include "ftcv.h"

int main(int argc, char** argv)
{
  if(argc == 3 && argv) {
    FILE *file;
    
    if(strcicmp(argv[1], "send") == 0) {
      if((file = fopen(argv[2], "r")) != NULL) {
        unsigned int success = send(file);
        fclose(file);
        return (!success) ? 0 : -2;
      } else {
        printf("Cannot open file '%s' for reading!\nExiting.\n", argv[2]);
        return -1;
      }
    } else if(strcicmp(argv[1], "receive") == 0) {
      if((file = fopen(argv[2], "w")) != NULL) {
        unsigned int success = recv(file);
        fflush(file);
        fclose(file);
        return (!success) ? 0 : -2;
      } else {
        printf("Cannot open file '%s' for writing!\nExiting.\n", argv[2]);
        return -1;
      }
    }
  } else {
    printUsage((argc > 0) ? argv[0] : NULL);
    return -1;
  }
}

void printUsage(char* execName)
{
  if(!execName)
    execName = "ftcv";
  printf("Usage:  %s send|receive filename\n", execName);
}
