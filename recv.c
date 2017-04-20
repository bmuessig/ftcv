#include "recv.h"

unsigned int recv(FILE* file)
{
  unsigned int ptr = 0, try;
  int chr;

  // Clear the transmit buffer
  memset(tcxBuf, 0, TCX_BUFF_LEN);

  DBGS("RECV, Init, waiting for header");

  for(try = 0; try < MAX_TRIES; try++) {
    for(ptr = 0; ptr < protoMagicSize;) {
      if(!(chr = fgetc(stdin))) {
        DBGS("RECV, Aborting");
        fputc(FTCV_PROTO_CAN, stdout); // invalid read
        return 1;
      }

      if(chr == protoMagic[ptr])
        ptr++;
    }
  }
  if(try >= MAX_TRIES) { // 3 invalid tries = fail
    DBGS("RECV, Failed!");
    fputc(FTCV_PROTO_CAN, stdout);
    return 1;
  }
  DBGS("RECV, ACK INIT");
  // Send an ack
  fputc(FTCV_PROTO_ACK, stdout);

  // Await ack
  for(try = 0; try < MAX_TRIES; try++) {
    if((chr = awaitResponse(ACK_TIMEOUT * 10)) == FTCV_PROTO_ACK)
      break;
    else if(chr == FTCV_PROTO_CAN || chr == 0) // if we got an ack break the loop
      return 1; // the stream is over

    // otherwise flush buffer, send ENQ and repeat
    fflushi(stdin);
    fputc(FTCV_PROTO_ENQ, stdout);
  }

  // Now read the number of blocks
  unsigned int numBlocks, numBlocksChk;
  for(try = 0; try < MAX_TRIES; try++) {
    // First number
    if(!freaduint(&numBlocks, ACK_TIMEOUT, stdin)) {
      fputc(FTCV_PROTO_ENQ, stdout); // no data received
      continue;
    }

    // Second checksum
    if(!freaduint(&numBlocksChk, ACK_TIMEOUT, stdin)) {
      fputc(FTCV_PROTO_ENQ, stdout); // no data received
      continue;
    }

    fprintf(stderr, "Receiving %d blocks of data...", numBlocks);
    if(numBlocks == ~numBlocksChk)
      break;

    fputc(FTCV_PROTO_NAK, stdout); // invalid data received
  }


  /*if((chr = awaitResponse(ACK_TIMEOUT * 100)) == FTCV_PROTO_ACK) // we got an ack
    break;
  else if(chr == FTCV_PROTO_CAN || chr == 0) { // the stream is over
    // Confirm with a CAN
    fputc(FTCV_PROTO_CAN, stdout);
    return 1;
  }*/
}
