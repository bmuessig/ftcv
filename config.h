#ifndef CONFIG_H
#define CONFIG_H

// Acknowledge timeout in ms
#define ACK_TIMEOUT 100

// Transceive buffer size
#define TCX_BUFF_BITS 11
#define TCX_BUFF_LEN (1 << (TCX_BUFF_BITS))

#endif
