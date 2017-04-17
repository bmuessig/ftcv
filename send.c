#include "send.h"

unsigned int send(FILE* file)
{
  unsigned int ptr = 0;
  int chr, char magic[] = {0x1F, 0x78, 0x34, 0x99}, magicSize = sizeof(magic);

  // Clear the transmit buffer
  memset(tcxBuf, 0, TCX_BUFF_LEN);
  // Wait for the magic header
  while((chr = fgetc(file)) != EOF && ptr < magicSize) {
    if(chr == magic[ptr])
      ptr++;
    else
      ptr = 0;
  }
  // If the char is EOF, the stream is over
  if(chr == EOF)
    return 1;

  // Now the size of the data block is transmitted
  uintbyte size;
  size.intval = 0;
  ptr = 0;
  while((chr = fgetc(file)) != EOF && ptr < sizeof(intval))
    intval.byteval[ptr++] = chr;
  if(chr == EOF)
    return 1;


}
