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

int main(int argc, char** argv) {
  test_bits();
  test_instruction_macros();
  END_TEST();
  return 0;
}
