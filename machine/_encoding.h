#pragma once
#include <stdint.h>

#define LAST_N_BITS(_typ, n) (((_typ)1 << n)-1)

#define MASK_BYTE   (LAST_N_BITS(uint8_t,   8))
#define MASK_HIBYTE (LAST_N_BITS(uint64_t,  8) << 56)
#define MASK_LO20   (LAST_N_BITS(uint32_t, 20))
#define MASK_HI20   (LAST_N_BITS(uint64_t, 20) << 32)
#define MASK_LO40   (LAST_N_BITS(uint64_t, 40))

/** longword assumed to be 64 bits **/

#define HIBYTE(_longword) ((_longword & MASK_HIBYTE) >> 56)
#define LOBYTE(_longword) (_longword & MASK_BYTE)
#define LO20(_longword)   (_longword & MASK_LO20)
#define HI20(_longword)   ((_longword & MASK_HI20) >> 32)
#define LO40(_longword)   (_longword & MASK_LO40)

/** Specific encodings for instructions and words **/

/* uint32_t masks */
#define MASK_OPCODE       (0xFF000) /* Bits [19:12] */
#define MASK_ADDR         (0xFFF)   /* Bits [11:0] */
#define MASK_MAJOR_OPCODE (0xF8000) /* Bits [19:15] */
#define MASK_MINOR_OPCODE (0x7000)  /* Bits [14:12] */

/* uint32 padding */
#define ENC_MAJOR_OPCODE(_n) (_n << 15)
#define ENC_MINOR_OPCODE(_n) (_n << 12)
/* uint8_t parsing */
#define DEC_MAJOR_OPCODE(_o) ((uint8_t)((_o & MASK_MAJOR_OPCODE) >> 15)) /* From uint32_t */
#define DEC_MINOR_OPCODE(_o) ((uint8_t)((_o & MASK_MINOR_OPCODE) >> 12))

/** Macros to extract from 40-bit words (stored in uint64_t values) => uint32_t**/
#define DEC_L_INSTR(_word) (HI20(_word)) /* Bits 39 to 20 */
#define ENC_L_INSTR(_instr) ((uint64_t)_instr << 32) /* Bits 39 to 20 */
#define DEC_R_INSTR(_word) (LO20(_word)) /* Bits 19 to  0 */

/** Macros to extract from 20-bit instructions, i.e. from a uint32_t **/
#define DEC_OPCODE(_instr)  ((_instr & MASK_OPCODE) >> 12)
#define DEC_ADDRESS(_instr) (_instr & MASK_ADDR)
#define ENC_OPCODE(_maj, _min) ((_maj << 3) | _min) /* uint8_t */
#define ENC_INSTRUCTION(_maj, _min, _addr) ((ENC_OPCODE(_maj,_min) << 12) | _addr)
#define ENC_INSTR_WORD(_instr_l, _instr_r) (((uint64_t)_instr_l << 32) | _instr_r)

/* Pack opcode and address together in lowest 20 bits, leaving 12 bits padding *
 *
 * ||000000000000| M  |N |  address |
 * 31------------j----e--b----------0
 * */
#define NULL_ADDR (0xFFF)

