#include "send.h"

unsigned int send(FILE* file)
{
  unsigned int ptr = 0, try;
  int chr, magic[] = {FTCV_PROTO_SOH, 'F', 'T', 'C', 'V', FTCV_PROTO_VER,
    TCX_BUFF_BITS, FTCV_PROTO_ETB},
    magicSize = sizeof(magic);

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
    if((chr = awaitResponse(ACK_TIMEOUT)) == FTCV_PROTO_ACK) // we got an ack
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
    if((chr = awaitResponse(ACK_TIMEOUT)) == FTCV_PROTO_ACK) // we got an ack
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
    if((chr = awaitResponse(ACK_TIMEOUT)) == FTCV_PROTO_CAN ||
        chr == 0) { // the stream is over
      // Confirm with a CAN
      fputc(FTCV_PROTO_CAN, stdout);
      return 1;
    } else if(chr != FTCV_PROTO_ACK) // if we got no ack, repeat
      continue;

    // Now send an ack back
    fputc(FTCV_PROTO_ACK, stdout);

    // Now buffer the data, by copying as much as we can
    if(!(tcxPtr = fread(tcxBuf, 1, TCX_BUFF_LEN, file))) {
      // send can back
      fputc(FTCV_PROTO_CAN, stdout);
      // reading failed, exit!
      fprintf(stderr, "Reading %d bytes failed!\nAborting!\n", TCX_BUFF_LEN);
      return 2;
    }
    // And calculate the checksum
    unsigned int crc = crc32(tcxBuf, tcxPtr);

    // Shift out all the data
    for(ptr = 0; ptr < tcxPtr; ptr++) {
      // bytes that are equal to DLE need to be shifted out twice
      if(tcxBuf[ptr] == FTCV_PROTO_DLE)
        fputc(FTCV_PROTO_DLE, stdout);
      // Just shift out the byte
      fputc(tcxBuf[ptr], stdout);
    }

    while(fbavail(stdin)) {
      if((chr = fgetc(stdin)) == FTCV_PROTO_CAN) {
        // send dle + can back, since we still need to escape
        fputc(FTCV_PROTO_DLE, stdout);
        fputc(FTCV_PROTO_CAN, stdout);
        return 1;
      }
    }

    // End the data block with dle + eot
    fputc(FTCV_PROTO_DLE, stdout);
    fputc(FTCV_PROTO_EOT, stdout);
    // And write the checksum
    fwriteuint(crc, stdout);
    // Clear the input buffer
    fflushi(stdin);
    // And say that we're done
    fputc(FTCV_PROTO_ETB, stdout);

    // Wait for an ack, timeout is 8 times the usual one, since the other side
    // needs to calculate the checksum and finish writing
    if((chr = awaitResponse(ACK_TIMEOUT * 8)) == FTCV_PROTO_ACK) { // we got an ack
      // Send back an ack
      fputc(FTCV_PROTO_ACK, stdout);
      // And increment the block
      currentBlock++;
      // And clear the tcx pointer
      tcxPtr = 0;
    } else if(chr == FTCV_PROTO_CAN || chr == 0) { // the stream is over
      // Confirm with a CAN
      fputc(FTCV_PROTO_CAN, stdout);
      return 1;
    } else // this will handle garbage or an actual NAK
      fputc(FTCV_PROTO_ACK, stdout); // Acknowledge the failure
  }

  // Calculate the CRC for the entire file
  rewind(file);
  unsigned int crc = fcrc32(file);

  // In the end, we send an eot
  fputc(FTCV_PROTO_EOT, stdout);
  // Followed by the crc
  fwriteuint(crc, stdout);
  // We flush stdin again
  fflushi(stdin);

  // And we wait for an ack for the last time
  if((chr = awaitResponse(ACK_TIMEOUT * 100)) == FTCV_PROTO_ACK) { // we got an ack
    // Send the final ack
    fputc(FTCV_PROTO_ACK, stdout);
    // And return
    return 0;
  }
  // Confirm the failure with a CAN
  fputc(FTCV_PROTO_CAN, stdout);
  // And return failure
  return 1;
}
