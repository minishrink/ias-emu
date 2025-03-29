#include <stdio.h>
#include <stdint.h>
#include <string.h>

/** Test asserts **/

#define PRINT_PASS() (printf("\n\033[92mPASSED\033[0m"))
#define PRINT_FAIL() (printf("\n\033[91mFAILED\033[0m"))
static uint16_t pass_count = 0u;
static uint16_t test_count = 0u;

static uint32_t inline _assert(uint32_t _bool) {
  if (_bool) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %d", _bool);
  }
  ++test_count;
  return _bool;
}
static void inline _assert_streq(const char* x, const char* y) {
  if (strcmp(x,y) == 0) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %s != %s", x,y);
  }
  ++test_count;
}

#define ASSERT(_b) (_assert(_b))
#define ASSERT_STREQ(_x, _y) (_assert_streq(_x,_y))

/** Instruction data **/
#define    MASK_MAJOR_OPCODE     (~(uint8_t)7)
#define    READ_MAJOR_OPCODE(_o) (_o & MASK_MAJOR_OPCODE)
#define         MAJOR_OPCODE(_n) (((uint8_t)_n) << 3)
#define LOAD    MAJOR_OPCODE(0x01)
#define MUL     MAJOR_OPCODE(0x02)
#define DIV     MAJOR_OPCODE(0x03)
#define LDMQ    MAJOR_OPCODE(0x04)
#define SHL     MAJOR_OPCODE(0x05)
#define SHR     MAJOR_OPCODE(0x06)
#define STORE   MAJOR_OPCODE(0x07)
#define BR_L    MAJOR_OPCODE(0x08)
#define BR_R    MAJOR_OPCODE(0x09)
#define CNDBR_L MAJOR_OPCODE(0x0A)
#define CNDBR_R MAJOR_OPCODE(0x0B)
#define MOVE    MAJOR_OPCODE(0x0C)
#define AMOD_L  MAJOR_OPCODE(0x0D)
#define AMOD_R  MAJOR_OPCODE(0x0E)
#define IO      MAJOR_OPCODE(0x0F)

/* Sub opcodes, paired with certain major opcodes */

#define    MASK_MINOR_OPCODE     (~MASK_MAJOR_OPCODE)
#define         MINOR_OPCODE(_n) ((uint8_t)_n)
#define    READ_MINOR_OPCODE(_o) (_o & MASK_MINOR_OPCODE)
/* LOAD subcodes */
#define LDABS    MINOR_OPCODE(0x00)
#define LD       MINOR_OPCODE(0x01)
#define LDNEG    MINOR_OPCODE(0x02)
#define ADDABS   MINOR_OPCODE(0x03)
#define SUBABS   MINOR_OPCODE(0x04)
#define ADD      MINOR_OPCODE(0x05)
#define SUB      MINOR_OPCODE(0x06)
/* MOVE subcodes */
#define MVABS    MINOR_OPCODE(0x00)
#define MVNEGABS MINOR_OPCODE(0x01)
#define MV       MINOR_OPCODE(0x02)
#define MVNEG    MINOR_OPCODE(0x03)
#define ADDMQABS MINOR_OPCODE(0x04)
#define SUBMQABS MINOR_OPCODE(0x05)
#define ADDMQ    MINOR_OPCODE(0x06)
#define SUBMQ    MINOR_OPCODE(0x07)
/* IO subcodes */
#define INCARD   MINOR_OPCODE(0x00)
#define OUTCARD  MINOR_OPCODE(0x01)

static inline const char* string_opcode_load(uint8_t o) {
  //printf("\nLOAD INSTRUCTION IDENTIFIED... PARSING SUBCODE...");
  switch(READ_MINOR_OPCODE(o)) {
    case LDABS:
      return "LDABS";
    case LD:
      return "LD";
    case LDNEG:
      return "LDNEG";
    case ADDABS:
      return "ADDABS";
    case SUBABS:
      return "SUBABS";
    case ADD:
      return "ADD";
    case SUB:
      return "SUB";
    default:
      return "UNDEF";
  } /* LOAD subswitch */
}

static inline const char* string_opcode_move(uint8_t o) {
  /* MOVE subcodes */
  //printf("\nMOVE INSTRUCTION IDENTIFIED... PARSING SUBCODE...");
  switch(READ_MINOR_OPCODE(o)) {
    case MVABS:
      return "MVABS";
    case MVNEGABS:
      return "MVNEGABS";
    case MV:
      return "MV";
    case MVNEG:
      return "MVNEG";
    case ADDMQABS:
      return "ADDMQABS";
    case SUBMQABS:
      return "SUBMQABS";
    case ADDMQ:
      return "ADDMQ";
    case SUBMQ:
      return "SUBMQ";
    default:
      return "UNDEF";
  }
}

static inline const char* string_opcode_io(uint8_t o) {
  //printf("\nIO INSTRUCTION IDENTIFIED... PARSING SUBCODE...");
  switch(READ_MINOR_OPCODE(o)) {
    case INCARD:
      return "INCARD";
    case OUTCARD:
      return "OUTCARD";
    default:
      return "UNDEF";
  }
}

const char* string_opcode(uint8_t o) {
  switch (o & MASK_MAJOR_OPCODE) {
    case MUL:
      return "MUL";
    case DIV:
      return "DIV";
    case LDMQ:
      return "LDMQ";
    case SHL:
      return "SHL";
    case SHR:
      return "SHR";
    case STORE:
      return "STORE";
    case BR_L:
      return "BR_L";
    case BR_R:
      return "BR_R";
    case CNDBR_L:
      return "CNDBR_L";
    case CNDBR_R:
      return "CNDBR_R";
    case AMOD_L:
      return "AMOD_L";
    case AMOD_R:
      return "AMOD_R";
    case LOAD:
      return string_opcode_load(o);
    case MOVE:
      return string_opcode_move(o);
    case IO:
      return string_opcode_io(o);
    default:
      return "UNDEF";
  }
  return "UNDEF";
}

/* Extract 8-bit opcode from 32-bit instruction */
#define OPCODE8(_i) ((uint8_t)((uint32_t)_i >> 24))
/* Extract 16-bit address from a 32-bit instruction */
#define ADDRESS(_i) ((uint16_t)((_i & 0xff) << 16))

/* Pack address into lower 16-bits, and 8-bit opcode into third highest byte of the int, like so:
 *
 * ||-------||   maj   || min ||---||    address   |
 * 31-------23---------18-----15---11--------------0
 * */
#define INSTRUCTION(_maj, _min, _addr) (((uint16_t)(_maj | _min) << 16) | (uint16_t)_addr)

/* Addresses range from 0 to 0x3e7 */
typedef struct TestVector {
  uint32_t instruction;
  const char* string;
} TestInstruction;

#define NUM_TV (6U)
static const TestInstruction instr_vectors[] = {
  { .instruction = INSTRUCTION(LOAD , LD,       0x42),  .string = "LD 0x42" },
  { .instruction = INSTRUCTION(MOVE , MVNEGABS, 0x2d),  .string = "MVNEGABS 0x2d" },
  { .instruction = INSTRUCTION(IO,    INCARD,   0x2d),  .string = "INCARD 0x2d" },
  { .instruction = INSTRUCTION(MUL,   0x00,     0x3e5), .string = "MUL 0x3e5" },
  { .instruction = INSTRUCTION(SHL,   0x00,     0x00),  .string = "SHL" },
  { .instruction = INSTRUCTION(STORE, 0x00,     0x30),  .string = "STORE 0x30" },
};

static int32_t inline string_instruction(char* strbfr, uint32_t instr) {
  uint16_t address = (uint16_t)instr;
  uint8_t opcode = (uint8_t)(instr >> 16);
  if (strcmp(string_opcode(opcode), "UNDEF") == 0) {
    return -1;
  }
  if (0x00 != address) {
    return snprintf(strbfr, (8 + 1 + 2 + 3 + 1), "%s 0x%x", string_opcode(opcode), address);
  } else {
    return snprintf(strbfr, (8 + 1), "%s", string_opcode(opcode));
  }
}

static void run_tests(void) {
  char strbfr[14] = {};
  for (uint32_t i = 0; i < NUM_TV; ++i) {
    const TestInstruction instr = instr_vectors[i];
    ASSERT(-1 != string_instruction(strbfr, instr.instruction));
    ASSERT_STREQ(strbfr, instr.string);
  }
  printf("\n\033[1mPassed %3u / %3u\033[0m", pass_count, test_count);
}

int main(int argc, char** argv) {
  run_tests();
  return 0;
}
