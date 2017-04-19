#ifndef CONFIG_H
#define CONFIG_H

// Acknowledge timeout in ms
#define ACK_TIMEOUT 100
// Maximum tries
#define MAX_TRIES 3

// Transceive buffer size, usually 512 bytes
#define TCX_BUFF_BITS 9
#define TCX_BUFF_LEN (1 << (TCX_BUFF_BITS))

#endif
