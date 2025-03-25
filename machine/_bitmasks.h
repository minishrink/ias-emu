#pragma once

#define LAST_N_BITS(n) (((uint64_t)1 << n)-1)

#define MASK_BYTE   (LAST_N_BITS(8))
#define MASK_HIBYTE (LAST_N_BITS(8) << 56)
#define MASK_ADDR   (LAST_N_BITS(12))
#define MASK_LO20  (LAST_N_BITS(20))
#define MASK_HI20  (LAST_N_BITS(20) << 20)
#define MASK_LO40  (LAST_N_BITS(40))

/** longword assumed to be 64 bits **/

#define HIBYTE(_longword) ((_longword & MASK_HIBYTE) >> 56)
#define LO20(_longword)   (_longword & MASK_LO20)
#define HI20(_longword)   (_longword & MASK_HI20)
#define LO40(_longword)   (_longword & MASK_LO40)
