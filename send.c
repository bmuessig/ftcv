#include "send.h"

unsigned int send(FILE* file)
{
  unsigned int ptr = 0, try;
  int chr, magic[] = {FTCV_PROTO_SOH, 'F', 'T', 'C', 'V', FTCV_PROTO_VER,
    TCX_BUFF_BITS, FTCV_PROTO_ETB},
    magicSize = sizeof(magic);
  usecs tmr;

  // Clear the transmit buffer
  memset(tcxBuf, 0, TCX_BUFF_LEN);

  for(try = 0; try < MAX_TRIES; try++) {
    // Clear the input buffer
    fflushi(stdin);

    // Start the transmission and send the header
    for(; ptr < magicSize; ptr++)
      fputc(magic[ptr], stdout);

    // Wait for an Acknowledge
    // awaitResponse will return -1 on error or timeout, 0 on eof and 1 on success
    if((chr = awaitResponse(stdin, ACK_TIMEOUT)) == FTCV_PROTO_ACK) // we got an ack
      break;
    else if(chr == FTCV_PROTO_CAN || chr == 0) { // the stream is over
      // Confirm with a CAN
      fputc(FTCV_PROTO_CAN, stdout);
      return 1;
    }
  }

  // The connection is now established, let's send the number of blocks
  // So calculate the file size first
  unsigned int dataBlocks, dataBytes, lastBlockBytes;
  fseek(file, 0L, SEEK_END);
  dataBytes = ftell(file);
  rewind(file);
  // Now check how many blocks need to be sent
  lastBlockBytes = dataBytes % TCX_BUFF_LEN;
  if(lastBlockBytes) { // is the number of blocks uneven?
    dataBlocks = (dataBytes - lastBlockBytes) / TCX_BUFF_LEN;
    dataBlocks++;
  } else
    dataBlocks = dataBytes / TCX_BUFF_LEN;

  for(try = 0; try < MAX_TRIES; try++) {
    // Send an ACK back
    fputc(FTCV_PROTO_ACK, stdout);

    // Now transmit the number of blocks normally
    fwriteuint(dataBlocks, stdout);
    // And re-transmit it with all bits inverted
    fwriteuint(~dataBlocks, stdout);
    // Clear out the input buffer
    fflushi(stdin);
    // Tell the other side we have finished
    fputc(FTCV_PROTO_ETB, stdout);

    // Wait for an ack
    if((chr = awaitResponse(stdin, ACK_TIMEOUT)) == FTCV_PROTO_ACK) // we got an ack
      break;
    else if(chr == FTCV_PROTO_CAN || chr == 0) { // the stream is over
      // Confirm with a CAN
      fputc(FTCV_PROTO_CAN, stdout);
      return 1;
    }
  }

  // Send an ack back
  fputc(FTCV_PROTO_ACK, stdout);

  // Now starts the transmit loop
  unsigned int currentBlock = 0;
  while(currentBlock < dataBlocks) {
    // Send the "start of data frame" header
    fputc(FTCV_PROTO_STX, stdout);
    // Now send the block number
    fwriteuint(currentBlock, stdout);
    // And the inverse of the block number
    fwriteuint(~currentBlock, stdout);
    // Then send the number of bytes used in the current block
    // Check whether the current block is not the last block
    if(currentBlock + 1 < dataBlocks || !lastBlockBytes) {
      fwriteuint(TCX_BUFF_LEN, stdout); // send that the full frame is used
      fwriteuint(~TCX_BUFF_LEN, stdout); // and re-transmit the inverse for checking
    } else {
      fwriteuint(lastBlockBytes, stdout); // send the bytes used of the last frame
      fwriteuint(~lastBlockBytes, stdout); // and re-transmit the inverse for checking
    }

    // Flush the input buffer
    fflushi(stdin);
    // Tell the client that we are done
    fputc(FTCV_PROTO_ETB, stdout);

    // And wait for an ACK again
    if((chr = awaitResponse(stdin, ACK_TIMEOUT)) == FTCV_PROTO_ACK) // we got an ack
      continue;
    else if(chr == FTCV_PROTO_CAN || chr == 0) { // the stream is over
      // Confirm with a CAN
      fputc(FTCV_PROTO_CAN, stdout);
      return 1;
    }
  }

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
