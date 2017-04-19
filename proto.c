#include "proto.h"

int awaitResponse(unsigned int timeout)
{
  unsigned char resp;
  if(freadbyte(&resp, timeout, stdin) >= 0)
    return resp; // We got a valid response
  else
    return -1; // We had an error or a timeout
}
