#ifndef ds1307
#define ds1307
#include "i2c.h"
#include <stdbool.h>
#include <stdlib.h>
#define SLAVE_ADDRESS 0x68 // the slave address (0xD0 read / 0xD1 write)
#define TIME_STRUCT_SIZE 0x08
#define RAM_SIZE 55

typedef struct {
    u8 seconds;
    u8 minutes;
    u8 hours;
    bool am;
    u8 day_of_week;
    u8 date;
    u8 month;
    u8 year;
    bool clock_halt;
    bool out;
    bool sqwe;
    bool rs1;
    bool rs0;
} time;
// RS1 | RS0 | SQ output | SQWE | OUT
//  0  |  0  |    1 Hz   |   1  |  X
//  0  |  1  |  4.096 Hz |   1  |  X
//  1  |  0  |  8.192 Hz |   1  |  X
//  1  |  1  | 32.768 Hz |   1  |  X
//  X  |  X  |     0     |   0  |  0
//  X  |  X  |     1     |   0  |  1

u8* readTime(void);
void decodeTime(const u8 *data, time *s_time);
u8* encodeData(const time *s_time);
void writeTime(const time *s_time);
void printTime(const time *s_time);
void printByte(const u8 n);
void printRawData(const u8 *data, const u8 size);
time getTime(void);

// INFO: Addresses are relative!
// According to documentation:
// 0x00 - Timekeeper start
// 0x07 - Timekeeper end (8 bytes)
// 0x08 - RAM start
// 0x3F - RAM end (56 bytes)
// In this function - start address = 0x00 and end address = 0x38 (56 in dec)
u8* read(const u8 start_address, const u8 bytes, const bool ram);
void write(const u8 start_address, const u8 bytes, const bool ram, u8* data);

#endif // ds1307
