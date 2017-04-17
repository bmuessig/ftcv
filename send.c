#include "send.h"

unsigned int send(FILE* file)
{
  unsigned int ptr = 0, timedOut = 0;
  int chr, magic[] = {FTCV_PROTO_SOH, 'F', 'T', 'C', 'V', FTCV_PROTO_VER, TCX_BUFF_BITS},
    magicSize = sizeof(magic);
  usecs tmr;

  // Clear the transmit buffer
  memset(tcxBuf, 0, TCX_BUFF_LEN);

  // Start the transmission and send the header
  for(; ptr < magicSize; ptr++)
    fputc(magic[ptr], file);

  // Initialize the Timer
  tmr = tmr_start();
  // Wait for an Acknowledge or a timeout
  while(!(timedOut = (tmr_check(tmr) / 1000) >= ACK_TIMEOUT)) {

  }

  // If the char is EOF, the stream is over
  if(chr == EOF)
    return 1;
  // If everything matches, send an ACK

  // Now the number of the data blocks is transmitted
  /*uintbyte size;
  size.intval = 0;
  ptr = 0;
  while((chr = fgetc(file)) != EOF && ptr < sizeof(intval))
    intval.byteval[ptr++] = chr;
  if(chr == EOF)
    return 1;
*/

}
