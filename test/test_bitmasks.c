#include <stdio.h>
#include <stdint.h>

#include <_assert.h>
#include <_bitmasks.h>
#include <_instructions.h>

static void test_bits(void) {
  START_TEST("Check masks"); {
    const uint32_t lo_instr = 0xfffff;
    const uint64_t hi_instr = (uint64_t)lo_instr << 32;
    const uint64_t dataword = ((uint64_t)1 << 40)-1;
    ASSERT_UINT8(0xFF, MASK_BYTE);
    ASSERT_UINT64(0xFF00000000000000, MASK_HIBYTE);
    ASSERT_UINT32(lo_instr, MASK_LO20);
    ASSERT_UINT64(hi_instr, MASK_HI20);
    ASSERT_UINT64(dataword, MASK_LO40);
    ASSERT_UINT32(0xfff, MASK_ADDR);
  }

  START_TEST("Check bitshuffling"); {
    const uint64_t longword = 0xabcdef00fedcbaf0;
    ASSERT_UINT64(HIBYTE(longword), 0xab);
    ASSERT_UINT8(HIBYTE(longword), 0xab);
    ASSERT_UINT8(LOBYTE(longword), 0xf0);
    ASSERT_UINT32(LO20(longword), 0xcbaf0);
    ASSERT_UINT64(HI20(longword), 0xdef00);
  }
}
static void test_instruction_macros(void) {
  START_TEST("Instruction macros"); {   /*  |---L---|---R---| */
    const uint64_t instruction_pair_word = 0x000faded000babed;
    const uint32_t instr = 0x000aa0b0; /* In hex, bits [19:12] are nybbles [4:3] */
    /* Instructions */
    ASSERT_UINT32(R_INSTR(instruction_pair_word), 0xbabed);
    ASSERT_UINT32(L_INSTR(instruction_pair_word), 0xfaded);
    /* Addresses */
    ASSERT_UINT16(DEC_ADDRESS(R_INSTR(instruction_pair_word)), 0xbed);
    ASSERT_UINT16(DEC_ADDRESS(L_INSTR(instruction_pair_word)), 0xded);
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
