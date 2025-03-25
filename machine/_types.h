#pragma once
#include <stdint.h>

/** Define word types here **/

/** All word in the IAS machine are 40 bits long, which is awkward to emulate on modern architectures
 *  so they will be implemented as 64-bit words with padding **/

typedef uint64_t Word;

typedef Word Data;
typedef Word InstructionPair;

