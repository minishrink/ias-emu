#include <stdio.h>
#include <stdint.h>

#include <_assert.h>
#include <_encoding.h>
#include <_instructions.h>
#include <_macros.h>

static void test_bits(void) {
  START_TEST("Check masks"); {
    const uint64_t longword = 0xabcdef00fedcbaf0;
    ASSERT_UINT8(LOBYTE(longword),  0xf0);
    ASSERT_UINT32(LO20(longword),   0xcbaf0);
    ASSERT_UINT64(HI20(longword),   0xdef00);
  }
}
static void test_instruction_macros(void) {
  START_TEST("Instruction macros"); {   /*  |---L---|---R---| */
    const uint64_t instruction_pair_word = 0x000faded000babed;
    const uint32_t instr = 0x000aa0b0; /* In hex, bits [19:12] are nybbles [4:3] */
    /* Instructions */
    ASSERT_UINT32(DEC_R_INSTR(instruction_pair_word), 0xbabed);
    ASSERT_UINT32(DEC_L_INSTR(instruction_pair_word), 0xfaded);
    /* Addresses */
    ASSERT_UINT16(DEC_ADDRESS(DEC_R_INSTR(instruction_pair_word)), 0xbed);
    ASSERT_UINT16(DEC_ADDRESS(DEC_L_INSTR(instruction_pair_word)), 0xded);
    /* Opcodes */
    ASSERT_UINT32(0xff000, MASK_OPCODE);
    ASSERT_UINT32(DEC_OPCODE(instr), 0xaa);
    ASSERT_UINT8(DEC_OPCODE(instr), 0xaa);
  }
}

static void test_encoding(void) {
  START_TEST("Test opcode encoding error reporting"); {
    uint8_t opcode_bfr = 0xFF;

    EncodingStatus e = enc_opcode(LOAD, LD, NULL);
    ASSERT_UINT8(ENC_ERR_MEM, e);

    e = enc_opcode(LOAD << 6, LD, &opcode_bfr);
    ASSERT_UINT8(ENC_ERR_OPCODE_MAJ, e);
    ASSERT_UINT8(0xFF, opcode_bfr);

    e = enc_opcode(LOAD, LD << 4, &opcode_bfr);
    ASSERT_UINT8(ENC_ERR_OPCODE_MIN, e);
    ASSERT_UINT8(0xFF, opcode_bfr);

    e = enc_opcode(IO, OUTCARD, &opcode_bfr);
    ASSERT_UINT8(ENC_OK, e);
    ASSERT_UINT8((IO<<3) | OUTCARD, opcode_bfr);
    ASSERT_UINT8(0x79, (IO<<3) | OUTCARD);
  }
  START_TEST("Test instruction error reporting"); {
    uint32_t instr_bfr = 0xFFFFFFFF;

    EncodingStatus e = enc_instruction(LOAD, LD, 0x2BE, NULL);
    ASSERT_UINT8(ENC_ERR_MEM, e);

    e = enc_instruction(LOAD << 6, LD, 0x02, &instr_bfr);
    ASSERT_UINT8(ENC_ERR_OPCODE_MAJ, e);
    ASSERT_UINT32(0xFFFFFFFF, instr_bfr);

    e = enc_instruction(LOAD, LDNEG << 4, 0x14e, &instr_bfr);
    ASSERT_UINT8(ENC_ERR_OPCODE_MIN, e);
    ASSERT_UINT32(0xFFFFFFFF, instr_bfr);

    e = enc_instruction(LOAD, LD, 0x4ff, &instr_bfr);
    ASSERT_UINT8(ENC_ERR_ADDR, e);
    ASSERT_UINT32(0xFFFFFFFF, instr_bfr);

    e = enc_instruction(IO, OUTCARD, 0x3e4, &instr_bfr);
    ASSERT_UINT8(ENC_OK, e);
    /* (IO<<3) | OUTCARD) << 12) | 0x3e4 =>
     * (0xF << 3) => 0x78 | 0x01 =>
     * 0x79 << 12 => 0x79000 | 0x3e4 => 0x793e4
     * */
    ASSERT_UINT32(0x793e4, instr_bfr);
  }
}

int main(int argc, char** argv) {
  test_bits();
  test_instruction_macros();
  test_encoding();
  END_TEST();
  return 0;
}
