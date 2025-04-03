#pragma once
#include <stdint.h>

/** Define word types here **/

/* TODO: how to handle ACC and MQ as one 80-bit register for mult and shft? */
typedef int DoubleWord;

/** All word in the IAS machine are 40 bits long, which is awkward to emulate on modern architectures
 *  so they will be implemented as 64-bit words with padding **/

typedef uint64_t Word;

/* Instructions 20 bits long and are packed in pairs into one word. They will be internally represented with 32-bits */
typedef uint32_t Instruction;

/* Instructions consist of an 8-bit opcode and a 12-bit address */
typedef uint16_t Address;
typedef uint8_t Opcode;

#define TRUE  0x01
#define FALSE 0x0
