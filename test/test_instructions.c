#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <_assert.h>
#include <_encoding.h>
#include <_macros.h>

#define READ_MAJOR_OPCODE(_o) ((_o & 0xF8)>>3)  /* From uint8_t */
#define READ_MINOR_OPCODE(_o) (_o & 7)  /* From uint8_t */

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


/* Addresses range from 0 to 0x3e7 */
typedef struct TestVector {
  uint32_t instruction;
  uint8_t  opcode;
  const char* string;
} TestInstruction;

#define NUM_TV (6U)
static const TestInstruction instr_vectors[] = {
  { .instruction = ENC_INSTRUCTION(LOAD,  LD,       0x42),      .opcode=LOAD,  .string = "LD 0x42" },
  { .instruction = ENC_INSTRUCTION(MOVE,  MVNEGABS, 0x2d),      .opcode=MOVE,  .string = "MVNEGABS 0x2d" },
  { .instruction = ENC_INSTRUCTION(IO,    INCARD,   0x2d),      .opcode=IO,    .string = "INCARD 0x2d" },
  { .instruction = ENC_INSTRUCTION(MUL,   0x00,     0x3e5),     .opcode=MUL,   .string = "MUL 0x3e5" },
  { .instruction = ENC_INSTRUCTION(SHL,   0x00,     NULL_ADDR), .opcode=SHL,   .string = "SHL" },
  { .instruction = ENC_INSTRUCTION(STORE, 0x00,     0x30),      .opcode=STORE, .string = "STORE 0x30" },
};

static int32_t inline string_instruction(char* strbfr, uint32_t instr) {
  uint16_t address = (uint16_t)DEC_ADDRESS(instr);
  uint8_t opcode = (uint8_t)DEC_OPCODE(instr);
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
      ASSERT_UINT8(instr.opcode, READ_MAJOR_OPCODE(DEC_OPCODE(instr.instruction)));
    }
  } /* TEST */
}

static void test_opcodes(void) {
  START_TEST("Major opcode encoding"); {
    ASSERT_UINT32(0x01 << 15, ENC_MAJOR_OPCODE(LOAD));
    ASSERT_UINT32(0x02 << 15, ENC_MAJOR_OPCODE(MUL));
    ASSERT_UINT32(0x03 << 15, ENC_MAJOR_OPCODE(DIV));
    ASSERT_UINT32(0x04 << 15, ENC_MAJOR_OPCODE(LDMQ));
    ASSERT_UINT32(0x05 << 15, ENC_MAJOR_OPCODE(SHL));
    ASSERT_UINT32(0x06 << 15, ENC_MAJOR_OPCODE(SHR));
    ASSERT_UINT32(0x07 << 15, ENC_MAJOR_OPCODE(STORE));
    ASSERT_UINT32(0x08 << 15, ENC_MAJOR_OPCODE(BR_L));
    ASSERT_UINT32(0x09 << 15, ENC_MAJOR_OPCODE(BR_R));
    ASSERT_UINT32(0x0A << 15, ENC_MAJOR_OPCODE(CNDBR_L));
    ASSERT_UINT32(0x0B << 15, ENC_MAJOR_OPCODE(CNDBR_R));
    ASSERT_UINT32(0x0C << 15, ENC_MAJOR_OPCODE(MOVE));
    ASSERT_UINT32(0x0D << 15, ENC_MAJOR_OPCODE(AMOD_L));
    ASSERT_UINT32(0x0E << 15, ENC_MAJOR_OPCODE(AMOD_R));
    ASSERT_UINT32(0x0F << 15, ENC_MAJOR_OPCODE(IO));
  }
  START_TEST("Opcode decoding"); {
    ASSERT_UINT32(SHL,    DEC_MAJOR_OPCODE(ENC_MAJOR_OPCODE(SHL)));
    ASSERT_UINT32(LOAD,   DEC_MAJOR_OPCODE(ENC_MAJOR_OPCODE(LOAD)));
    ASSERT_UINT32(MOVE,   DEC_MAJOR_OPCODE(ENC_MAJOR_OPCODE(MOVE)));
    ASSERT_UINT32(BR_L,   DEC_MAJOR_OPCODE(ENC_MAJOR_OPCODE(BR_L)));
    ASSERT_UINT32(STORE,  DEC_MAJOR_OPCODE(ENC_MAJOR_OPCODE(STORE)));
    ASSERT_UINT32(IO,     DEC_MAJOR_OPCODE(ENC_MAJOR_OPCODE(IO)));

    ASSERT_UINT32(MVABS,  DEC_MINOR_OPCODE(ENC_MINOR_OPCODE(MVABS)));
    ASSERT_UINT32(LDNEG,  DEC_MINOR_OPCODE(ENC_MINOR_OPCODE(LDNEG)));
    ASSERT_UINT32(ADD,    DEC_MINOR_OPCODE(ENC_MINOR_OPCODE(ADD)));
    ASSERT_UINT32(ADDMQ,  DEC_MINOR_OPCODE(ENC_MINOR_OPCODE(ADDMQ)));
    ASSERT_UINT32(MV,     DEC_MINOR_OPCODE(ENC_MINOR_OPCODE(MV)));
    ASSERT_UINT32(INCARD, DEC_MINOR_OPCODE(ENC_MINOR_OPCODE(INCARD)));
  }
  START_TEST("Minor LOAD opcode encoding"); {
    ASSERT_UINT32(0x00 << 12, ENC_MINOR_OPCODE(LDABS));
    ASSERT_UINT32(0x01 << 12, ENC_MINOR_OPCODE(LD));
    ASSERT_UINT32(0x02 << 12, ENC_MINOR_OPCODE(LDNEG));
    ASSERT_UINT32(0x03 << 12, ENC_MINOR_OPCODE(ADDABS));
    ASSERT_UINT32(0x04 << 12, ENC_MINOR_OPCODE(SUBABS));
    ASSERT_UINT32(0x05 << 12, ENC_MINOR_OPCODE(ADD));
    ASSERT_UINT32(0x06 << 12, ENC_MINOR_OPCODE(SUB));
  }
  START_TEST("Minor MOVE opcode encoding"); {
    ASSERT_UINT32(0x00 << 12, ENC_MINOR_OPCODE(MVABS));
    ASSERT_UINT32(0x01 << 12, ENC_MINOR_OPCODE(MVNEGABS));
    ASSERT_UINT32(0x02 << 12, ENC_MINOR_OPCODE(MV));
    ASSERT_UINT32(0x03 << 12, ENC_MINOR_OPCODE(MVNEG));
    ASSERT_UINT32(0x04 << 12, ENC_MINOR_OPCODE(ADDMQABS));
    ASSERT_UINT32(0x05 << 12, ENC_MINOR_OPCODE(SUBMQABS));
    ASSERT_UINT32(0x06 << 12, ENC_MINOR_OPCODE(ADDMQ));
    ASSERT_UINT32(0x07 << 12, ENC_MINOR_OPCODE(SUBMQ));
  }
  START_TEST("Minor IO opcode encoding"); {
    ASSERT_UINT32(0x00 << 12, ENC_MINOR_OPCODE(INCARD));
    ASSERT_UINT32(0x01 << 12, ENC_MINOR_OPCODE(OUTCARD));
  }
  START_TEST("Major and minor opcode parsing"); {
    ASSERT_UINT8(LOAD,     READ_MAJOR_OPCODE(ENC_OPCODE(LOAD, LDABS)));
    ASSERT_UINT8(LDNEG,    READ_MINOR_OPCODE(ENC_OPCODE(LOAD, LDNEG)));
    ASSERT_UINT8(SHL,      READ_MAJOR_OPCODE(ENC_OPCODE(SHL, 0x00)));
    ASSERT_UINT8(MOVE,     READ_MAJOR_OPCODE(ENC_OPCODE(MOVE, MVNEGABS)));
    ASSERT_UINT8(MVNEGABS, READ_MINOR_OPCODE(ENC_OPCODE(MOVE, MVNEGABS)));
  }
}

int main(int argc, char** argv) {
  test_opcodes();
  test_instruction_parsing();
  END_TEST();
  return 0;
}
