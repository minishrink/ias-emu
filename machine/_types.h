#pragma once

/** Define word types here **/

/** All these types should be 40 bits long **/

/* This will take some bitfucking */
typedef struct DataWord {
  unsigned int sign: 1;
  unsigned int data31: 31;
  unsigned int data8: 8;
} Data;

typedef struct InstructionWord {
  unsigned int opcodeL: 8;
  unsigned int operandsL: 12;
  unsigned int opcodeR: 8;
  unsigned int operandsR: 12;
} InstructionPair;

typedef union Word {
  Data d;
  InstructionPair i;
} Word;

