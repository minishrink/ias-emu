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
#define MASK_ADDR   (LAST_N_BITS(12))
#define MASK_NYBB   (LAST_N_BITS(4))

/** Macros to extract from 40-bit words (stored in uint64_t values) **/
#define L_INSTR(_word) (HI20(_word)) /* Bits 39 to 20 */
#define R_INSTR(_word) (LO20(_word)) /* Bits 19 to  0 */

/** Macros to extract from 20-bit instructions **/
#define OPCODE(_instr)  (_instr & (MASK_BYTE << 12))
#define ADDRESS(_instr) (_instr & MASK_ADDR)

/* Macro to pack major opcode into high 5 bits and minor opcode into low 3 bits */
#define MAJ_OP(_instr) (_instr & ((LAST_N_BITS(5) << 15)
#define MIN_OP(_instr) (_instr & ((LAST_N_BITS(3) << 12)

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
