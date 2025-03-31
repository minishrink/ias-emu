#include <stdio.h>
#include <stdint.h>

#include <_assert.h>
#include <_bitmasks.h>
#include <_instructions.h>

static void test_bits(void) {
  const uint64_t longy = ~(uint64_t)0u;
  const uint8_t expecc = 0xFF;
  const uint64_t expected = (uint64_t)0u | expecc;

  START_TEST("Check consts"); {
    ASSERT_UINTEQ((uint64_t)0xFFFFFFFFFFFFFFFF, longy);
    ASSERT_UINTEQ(0x00000000000000FF, expected);
    ASSERT_UINT8(0xFF, expecc);
  }

  START_TEST("Check masks"); {
    const uint32_t lo_instr = 0xfffff;
    const uint64_t hi_instr = (uint64_t)lo_instr << 32;
    const uint64_t dataword = ((uint64_t)1 << 40)-1;
    ASSERT_UINT8(0xFF, MASK_BYTE);
    ASSERT_UINTEQ(0xFF00000000000000, MASK_HIBYTE);
    ASSERT_UINT32(lo_instr, MASK_LO20);
    ASSERT_UINTEQ(hi_instr, MASK_HI20);
    ASSERT_UINTEQ(dataword, MASK_LO40);
  }

  /* Now check bitshuffling */
  START_TEST("Check bitshuffling"); {
    ASSERT_UINTEQ(HI8(longy), expected);
    ASSERT_UINTEQ(HIBYTE(longy), expected);
    ASSERT_UINT8(HIBYTE(longy), expecc);
    ASSERT_UINT8(LOBYTE(longy), expecc);
    ASSERT_UINT32(LO20(longy), MASK_LO20);
    ASSERT_UINTEQ(HI20(longy), MASK_HI20);
  }
  END_TEST();
}

int main(int argc, char** argv) {
  test_bits();
  return 0;
}
