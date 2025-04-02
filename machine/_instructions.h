#pragma once

#include <stdint.h>

#include <_types.h>
#include <_encoding.h>

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
  _IO,
  _MAX_MAJOR_OPCODE
} OpcodeMain;

/** Minor opcodes : found in lower 3 bits **/
typedef enum OpcodeMinorLoad {
  _LDABS = 0,
  _LD,
  _LDNEG,
  _ADDABS,
  _SUBABS,
  _ADD,
  _SUB,
  _MAX_LOAD_OPCODE
} OpcodeLoad;

typedef enum OpcodeMinorMove {
  _MVABS = 0,
  _MVNEGABS,
  _MV,
  _MVNEG,
  _ADDMQABS,
  _SUBMQABS,
  _ADDMQ,
  _SUBMQ,
  _MAX_MOVE_OPCODE
} OpcodeMove;

typedef enum OpcodeMinorIO {
  _INCARD = 0,
  _OUTCARD,
  _MAX_IO_OPCODE
} OpcodeIO;

