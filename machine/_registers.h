#pragma once

#include <_types.h>

/** Used for AC, MQ, etc. **/
typedef Word WordReg;

/** Represents IBR */
typedef Instruction InstrReg;

/* Represents IR */
typedef Opcode OpReg;

/* Represents MAR */
typedef Address AddrReg;

/** externs for defined registers **/

/* Arithmetic registers */
extern WordReg ACC;
extern WordReg MQ;

/* Memory data parsing register */
extern WordReg MBR;

/* Next instruction */
extern InstrReg IBR;

/* Current opcode */
extern OpReg IR;

/* Counter goes up to 1000 */
extern uint16_t PC;

/* Memory fetching register */
extern AddrReg MAR;
