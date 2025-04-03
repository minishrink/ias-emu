#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <_assert.h>
#include <_encoding.h>
#include <_macros.h>

/* Addresses range from 0 to 0x3e7 */
typedef struct TestVector {
  uint8_t  op_major;
  uint8_t  op_minor;
  uint16_t addr;
  uint32_t enc;
} TestInstruction;

#define NUM_TV (6U)
static const TestInstruction instr_vectors[] = {
  { .op_major=LOAD,  .op_minor=LD,       .addr=0x42, .enc=0x9042},
  { .op_major=MOVE,  .op_minor=MVNEGABS, .addr=0x2d, .enc=0x6102d}, /* 12 << 3 => 96 or 0x60. 0x60 | 1 => 0x61 << 12 */
  { .op_major=IO,    .op_minor=INCARD,   .addr=0x04, .enc=0x78004}, /* 0xf << 3 => 120 or 0x78. 0x58000 | 0x04 */
  { .op_major=MUL,   .op_minor=0x00,     .addr=0x3e5, .enc=0x103e5}, /* 0x2 << 3 => 0x10. 0x10 << 12  => 0x10000 */
  { .op_major=SHL,   .op_minor=0x00,     .addr=0x20, .enc=0x28020}, /* 0x5 << 3 => 0x28020 */
  { .op_major=STORE, .op_minor=0x00,     .addr=0x30, .enc=0x38030}, /* 0x07 << 3 => 0x38030 */
};

static void test_instruction_parsing(void) {
  START_TEST("Instruction encoding"); {
    for (uint32_t i = 0; i < NUM_TV; ++i) {
      const TestInstruction instr = instr_vectors[i];
      uint32_t encoded_instr = 0xFFFFFFFF;
      EncodingStatus err = enc_instruction(instr.op_major, instr.op_minor, instr.addr, &encoded_instr);
      ASSERT_UINT8(ENC_OK, err);
      ASSERT_UINT32(instr.enc, encoded_instr);
      ASSERT_UINT8(instr.op_major, READ_MAJOR_OPCODE(DEC_OPCODE(encoded_instr)));
      ASSERT_UINT8(instr.op_minor, READ_MINOR_OPCODE(DEC_OPCODE(encoded_instr)));
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
