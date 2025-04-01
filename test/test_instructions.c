#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <_assert.h>

/** Instruction data **/
#define    MASK_MAJOR_OPCODE     (0xF8000) /* Bits [19:14] of 32 */
#define   PARSE_MAJOR_OPCODE(_o) ((uint8_t)((_o & MASK_MAJOR_OPCODE) >> 15)) /* From uint32_t */
#define    READ_MAJOR_OPCODE(_o) ((_o & 0xF8)>>3)  /* From uint8_t */
#define      TO_MAJOR_OPCODE(_n) (_n << 15)
#define LOAD    (0x01)
#define MUL     (0x02)
#define DIV     (0x03)
#define LDMQ    (0x04)
#define SHL     (0x05)
#define SHR     (0x06)
#define STORE   (0x07)
#define BR_L    (0x08)
#define BR_R    (0x09)
#define CNDBR_L (0x0A)
#define CNDBR_R (0x0B)
#define MOVE    (0x0C)
#define AMOD_L  (0x0D)
#define AMOD_R  (0x0E)
#define IO      (0x0F)

/* Sub opcodes, paired with certain major opcodes */

#define     MASK_MINOR_OPCODE     (0x7000)
#define       TO_MINOR_OPCODE(_n) (_n << 12)
#define    PARSE_MINOR_OPCODE(_o) ((uint8_t)((_o & MASK_MINOR_OPCODE) >> 12))
#define     READ_MINOR_OPCODE(_o) (_o & 0x7U)
/* LOAD subcodes */
#define LDABS    (0x00)
#define LD       (0x01)
#define LDNEG    (0x02)
#define ADDABS   (0x03)
#define SUBABS   (0x04)
#define ADD      (0x05)
#define SUB      (0x06)
/* MOVE subcodes */
#define MVABS    (0x00)
#define MVNEGABS (0x01)
#define MV       (0x02)
#define MVNEG    (0x03)
#define ADDMQABS (0x04)
#define SUBMQABS (0x05)
#define ADDMQ    (0x06)
#define SUBMQ    (0x07)
/* IO subcodes */
#define INCARD   (0x00)
#define OUTCARD  (0x01)

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
  return "UNDEF";
}

const char* string_opcode(uint8_t o) {
  switch (READ_MAJOR_OPCODE(o)) {
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

#define MASK_OPCODE (0xFF000)
#define MASK_ADDR (0xFFF)
/* Extract 8-bit opcode from 32-bit instruction */
#define GET_OPCODE(_instr)  ((_instr & MASK_OPCODE) >> 12)
/* Extract 16-bit address from a 32-bit instruction */
#define GET_ADDRESS(_instr) (_instr & MASK_ADDR)

/* Pack opcode and address together in lowest 20 bits, leaving 12 bits padding *
 *
 * ||000000000000| M  |N |  address |
 * 31------------j----e--b----------0
 * */
#define TO_OPCODE(_maj, _min) ((_maj << 3) | _min) /* uint8_t */
#define TO_INSTRUCTION(_maj, _min, _addr) ( (((_maj << 3) | _min) << 12) | _addr)
#define NULL_ADDR (0xFFF)

/* Addresses range from 0 to 0x3e7 */
typedef struct TestVector {
  uint32_t instruction;
  uint8_t  opcode;
  const char* string;
} TestInstruction;

#define NUM_TV (6U)
static const TestInstruction instr_vectors[] = {
  { .instruction = TO_INSTRUCTION(LOAD,  LD,       0x42),      .opcode=LOAD,  .string = "LD 0x42" },
  { .instruction = TO_INSTRUCTION(MOVE,  MVNEGABS, 0x2d),      .opcode=MOVE,  .string = "MVNEGABS 0x2d" },
  { .instruction = TO_INSTRUCTION(IO,    INCARD,   0x2d),      .opcode=IO,    .string = "INCARD 0x2d" },
  { .instruction = TO_INSTRUCTION(MUL,   0x00,     0x3e5),     .opcode=MUL,   .string = "MUL 0x3e5" },
  { .instruction = TO_INSTRUCTION(SHL,   0x00,     NULL_ADDR), .opcode=SHL,   .string = "SHL" },
  { .instruction = TO_INSTRUCTION(STORE, 0x00,     0x30),      .opcode=STORE, .string = "STORE 0x30" },
};

static int32_t inline string_instruction(char* strbfr, uint32_t instr) {
  uint16_t address = (uint16_t)GET_ADDRESS(instr);
  uint8_t opcode = (uint8_t)GET_OPCODE(instr);
  if (strcmp(string_opcode(opcode), "UNDEF") == 0) {
    return -1;
  }
  if (NULL_ADDR != address) {
    return snprintf(strbfr, (8 + 1 + 2 + 3 + 1), "%s 0x%x", string_opcode(opcode), address);
  } else {
    return snprintf(strbfr, (8 + 1), "%s", string_opcode(opcode));
  }
}

static void test_instruction_parsing(void) {
  char strbfr[14] = {};
  START_TEST("Instruction opcodes printing"); {
    for (uint32_t i = 0; i < NUM_TV; ++i) {
      const TestInstruction instr = instr_vectors[i];
      ASSERT(-1 != string_instruction(strbfr, instr.instruction));
      ASSERT_STREQ(strbfr, instr.string);
      ASSERT_UINT8(instr.opcode, READ_MAJOR_OPCODE(GET_OPCODE(instr.instruction)));
    }
  } /* TEST */
}

static void test_opcodes(void) {
  START_TEST("Major opcode shifting"); {
    ASSERT_UINT32(0x01 << 15, TO_MAJOR_OPCODE(LOAD));
    ASSERT_UINT32(0x02 << 15, TO_MAJOR_OPCODE(MUL));
    ASSERT_UINT32(0x03 << 15, TO_MAJOR_OPCODE(DIV));
    ASSERT_UINT32(0x04 << 15, TO_MAJOR_OPCODE(LDMQ));
    ASSERT_UINT32(0x05 << 15, TO_MAJOR_OPCODE(SHL));
    ASSERT_UINT32(0x06 << 15, TO_MAJOR_OPCODE(SHR));
    ASSERT_UINT32(0x07 << 15, TO_MAJOR_OPCODE(STORE));
    ASSERT_UINT32(0x08 << 15, TO_MAJOR_OPCODE(BR_L));
    ASSERT_UINT32(0x09 << 15, TO_MAJOR_OPCODE(BR_R));
    ASSERT_UINT32(0x0A << 15, TO_MAJOR_OPCODE(CNDBR_L));
    ASSERT_UINT32(0x0B << 15, TO_MAJOR_OPCODE(CNDBR_R));
    ASSERT_UINT32(0x0C << 15, TO_MAJOR_OPCODE(MOVE));
    ASSERT_UINT32(0x0D << 15, TO_MAJOR_OPCODE(AMOD_L));
    ASSERT_UINT32(0x0E << 15, TO_MAJOR_OPCODE(AMOD_R));
    ASSERT_UINT32(0x0F << 15, TO_MAJOR_OPCODE(IO));
  }
  START_TEST("Opcode parsing"); {
    ASSERT_UINT32(SHL,    PARSE_MAJOR_OPCODE(TO_MAJOR_OPCODE(SHL)));
    ASSERT_UINT32(LOAD,   PARSE_MAJOR_OPCODE(TO_MAJOR_OPCODE(LOAD)));
    ASSERT_UINT32(MOVE,   PARSE_MAJOR_OPCODE(TO_MAJOR_OPCODE(MOVE)));
    ASSERT_UINT32(BR_L,   PARSE_MAJOR_OPCODE(TO_MAJOR_OPCODE(BR_L)));
    ASSERT_UINT32(STORE,  PARSE_MAJOR_OPCODE(TO_MAJOR_OPCODE(STORE)));
    ASSERT_UINT32(IO,     PARSE_MAJOR_OPCODE(TO_MAJOR_OPCODE(IO)));

    ASSERT_UINT32(MVABS,  PARSE_MINOR_OPCODE(TO_MINOR_OPCODE(MVABS)));
    ASSERT_UINT32(LDNEG,  PARSE_MINOR_OPCODE(TO_MINOR_OPCODE(LDNEG)));
    ASSERT_UINT32(ADD,    PARSE_MINOR_OPCODE(TO_MINOR_OPCODE(ADD)));
    ASSERT_UINT32(ADDMQ,  PARSE_MINOR_OPCODE(TO_MINOR_OPCODE(ADDMQ)));
    ASSERT_UINT32(MV,     PARSE_MINOR_OPCODE(TO_MINOR_OPCODE(MV)));
    ASSERT_UINT32(INCARD, PARSE_MINOR_OPCODE(TO_MINOR_OPCODE(INCARD)));
  }
  START_TEST("Minor LOAD opcode shifting"); {
    ASSERT_UINT32(0x00 << 12, TO_MINOR_OPCODE(LDABS));
    ASSERT_UINT32(0x01 << 12, TO_MINOR_OPCODE(LD));
    ASSERT_UINT32(0x02 << 12, TO_MINOR_OPCODE(LDNEG));
    ASSERT_UINT32(0x03 << 12, TO_MINOR_OPCODE(ADDABS));
    ASSERT_UINT32(0x04 << 12, TO_MINOR_OPCODE(SUBABS));
    ASSERT_UINT32(0x05 << 12, TO_MINOR_OPCODE(ADD));
    ASSERT_UINT32(0x06 << 12, TO_MINOR_OPCODE(SUB));
  }
  START_TEST("Minor MOVE opcode shifting"); {
    ASSERT_UINT32(0x00 << 12, TO_MINOR_OPCODE(MVABS));
    ASSERT_UINT32(0x01 << 12, TO_MINOR_OPCODE(MVNEGABS));
    ASSERT_UINT32(0x02 << 12, TO_MINOR_OPCODE(MV));
    ASSERT_UINT32(0x03 << 12, TO_MINOR_OPCODE(MVNEG));
    ASSERT_UINT32(0x04 << 12, TO_MINOR_OPCODE(ADDMQABS));
    ASSERT_UINT32(0x05 << 12, TO_MINOR_OPCODE(SUBMQABS));
    ASSERT_UINT32(0x06 << 12, TO_MINOR_OPCODE(ADDMQ));
    ASSERT_UINT32(0x07 << 12, TO_MINOR_OPCODE(SUBMQ));
  }
  START_TEST("Minor IO opcode shifting"); {
    ASSERT_UINT32(0x00 << 12, TO_MINOR_OPCODE(INCARD));
    ASSERT_UINT32(0x01 << 12, TO_MINOR_OPCODE(OUTCARD));
  }
  START_TEST("Major and minor opcode parsing"); {
    ASSERT_UINT8(LOAD,     READ_MAJOR_OPCODE(TO_OPCODE(LOAD, LDABS)));
    ASSERT_UINT8(LDNEG,    READ_MINOR_OPCODE(TO_OPCODE(LOAD, LDNEG)));
    ASSERT_UINT8(SHL,      READ_MAJOR_OPCODE(TO_OPCODE(SHL, 0x00)));
    ASSERT_UINT8(MOVE,     READ_MAJOR_OPCODE(TO_OPCODE(MOVE, MVNEGABS)));
    ASSERT_UINT8(MVNEGABS, READ_MINOR_OPCODE(TO_OPCODE(MOVE, MVNEGABS)));
  }
}

int main(int argc, char** argv) {
  test_opcodes();
  test_instruction_parsing();
  END_TEST();
  return 0;
}
