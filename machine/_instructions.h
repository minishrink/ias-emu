#pragma once

#include <stdint.h>

#include <_types.h>
#include <_bitmasks.h>

/** Instructions look like this (little endian):
 *
 * || opcode ||----- address ---- |
*  19--------11-------------------0
*
*  They are packed into a word in pairs, with different instructions to retrieve the left and right-hand instructions
*
 * || L_opcode ||---- L_address --- || R_opcode || ---- R_address ---- ||
*  39----------31-------------------20----------11----------------------0
*
*  In 64-bits an instruction pair word will look like this:
*
 * ||--- padding ---|| L_opcode ||---- L_address --- || R_opcode || ---- R_address ---- ||
*  63---------------39----------31-------------------20----------11----------------------0
*/

/* uint32_t masks */
#define MASK_OPCODE       (0xFF000) /* Bits [19:12] */
#define MASK_ADDR         (0xFFF)   /* Bits [11:0] */
#define MASK_MAJOR_OPCODE (0xF8000) /* Bits [19:15] */
#define MASK_MINOR_OPCODE (0x7000)  /* Bits [14:12] */

/* uint32 padding */
#define    TO_MAJOR_OPCODE(_n) (_n << 15)
#define    TO_MINOR_OPCODE(_n) (_n << 12)
/* uint8_t parsing */
#define GET_MAJOR_OPCODE(_o) ((uint8_t)((_o & MASK_MAJOR_OPCODE) >> 15)) /* From uint32_t */
#define GET_MINOR_OPCODE(_o) ((uint8_t)((_o & MASK_MINOR_OPCODE) >> 12))

/** Macros to extract from 40-bit words (stored in uint64_t values) => uint32_t**/
#define L_INSTR(_word) (HI20(_word)) /* Bits 39 to 20 */
#define R_INSTR(_word) (LO20(_word)) /* Bits 19 to  0 */

/** Macros to extract from 20-bit instructions, i.e. from a uint32_t **/
#define OPCODE(_instr)  ((_instr & MASK_OPCODE) >> 12)
#define ADDRESS(_instr) (_instr & MASK_ADDR)

/* Macro to pack major opcode into high 5 bits and minor opcode into low 3 bits */
#define MAJ_OP(_instr) (_instr & (LAST_N_BITS(uint32_t,5) << 15))
#define MIN_OP(_instr) (_instr & (LAST_N_BITS(uint32_t,3) << 12))

/** Opcode categories : found in high 5 bits **/
typedef enum OpcodeMain {
  _LOAD = 1,
  _MUL,
  _DIV,
  _LDMQ,
  _SHL,
  _SHR,
  _STORE,
  _BR_L,
  _BR_R,
  _CNDBR_L,
  _CNDBR_R,
  _MOVE,
  /* Program modification instructions */
  _AMOD_L,
  _AMOD_R,
  /* IO instructions to write to program cards */
  _IO
} OpcodeMain;

/** Minor opcodes : found in lower 3 bits **/
typedef enum OpcodeMinorLoad {
  LDABS = 0,
  LD,
  LDNEG,
  ADDABS,
  SUBABS,
  ADD,
  SUB
} OpcodeLoad;

typedef enum OpcodeMinorMove {
  MVABS = 0,
  MVNEGABS,
  MV,
  MVNEG,
  ADDMQABS,
  SUBMQABS,
  ADDMQ,
  SUBMQ
} OpcodeMove;

typedef enum OpcodeMinorIO {
  INCARD = 0,
  OUTCARD
} OpcodeIO;
