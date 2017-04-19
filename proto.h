#ifndef PROTO_H
#define PROTO_H

// Protocol version
#define FTCV_PROTO_VER 0x01

// Start of the entire transmission
#define FTCV_PROTO_SOH 0x01
// Start of a data block, followed by the size in bytes used
#define FTCV_PROTO_STX 0x02
// Successfull end of the current data block, followed by CRC32
#define FTCV_PROTO_ETX 0x03
// Successfull end of the whole transmission
#define FTCV_PROTO_EOT 0x04
// End of data block, needs an ack to continiue
#define FTCV_PROTO_ETB 0x17
// Acknowledge
#define FTCV_PROTO_ACK 0x06
// Not Acknowledge (e.g. CRC32 missmatch)
#define FTCV_PROTO_NAK 0x15
// Synchronisation packet with the current block number
#define FTCV_PROTO_SYN 0x16
// Cancel the entire transmission (client to server)
#define FTCV_PROTO_CAN 0x18
// Data escape sequence
// 0x10 0x10 referrs the the data byte 0x10. (server to client)
// 0x10 0x02 definitively marks the end of block. (server to client)
// 0x10 0x16 requests the current offset in the block (client to server)
// 0x10 0x18 cancels the entire transmission (client to server)
// All other sequences are undefined and are skipped
#define FTCV_PROTO_DLE 0x10;

int awaitAck(unsigned int timeout);

#endif
