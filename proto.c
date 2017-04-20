#include "proto.h"

unsigned char protoMagic[] = {FTCV_PROTO_SOH, 'F', 'T', 'C', 'V', FTCV_PROTO_VER,
  TCX_BUFF_BITS, FTCV_PROTO_ETB}, protoMagicSize = sizeof(protoMagic);

int awaitResponse(unsigned int timeout)
{
  unsigned char resp;
  if(freadbyte(&resp, timeout, stdin) >= 0)
    return resp; // We got a valid response
  else
    return -1; // We had an error or a timeout
}
