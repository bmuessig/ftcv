#ifndef PROTO_H
#define PROTO_H

// Protocol version
#define FTCV_PROTO_VER 0x01

// Start of the entire transmission
#define FTCV_PROTO_SOH 0x01
// Successfull end of the current data block, followed by CRC32
#define FTCV_PROTO_STX 0x02
// Successfull end of the whole transmission
#define FTCV_PROTO_ETX 0x03
// Acknowledge
#define FTCV_PROTO_ACK 0x06
// Not Acknowledge (e.g. CRC32 missmatch)
#define FTCV_PROTO_NAK 0x15
// Synchronisation packet with the current block number
#define FTCV_PROTO_SYN 0x16
// Cancel the entire transmission (client to server)
#define FTCV_PROTO_CAN 0x18

#endif
