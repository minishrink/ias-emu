#pragma once
#include <stdint.h>

#define LAST_N_BITS(_typ, n) (((_typ)1 << n)-1)

#define MASK_BYTE   (LAST_N_BITS(uint8_t,8))
#define MASK_HIBYTE (LAST_N_BITS(uint64_t,8) << 56)
#define MASK_LO20  (LAST_N_BITS(uint32_t,20))
#define MASK_HI20  (LAST_N_BITS(uint64_t, 20) << 32)
#define MASK_LO40  (LAST_N_BITS(uint64_t, 40))

/** longword assumed to be 64 bits **/

#define HIBYTE(_longword) ((uint8_t)((_longword & MASK_HIBYTE) >> 56))
#define LOBYTE(_longword) ((uint8_t)((_longword & MASK_BYTE)))
#define LO20(_longword)   (_longword & MASK_LO20)
#define HI20(_longword)   (_longword & MASK_HI20)
#define LO40(_longword)   (_longword & MASK_LO40)
