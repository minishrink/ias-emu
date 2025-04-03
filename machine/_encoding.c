#include <stdint.h>
#include <stdlib.h>

#include <_types.h>
#include <_instructions.h>

const char* enc_err_sprintf(EncodingStatus e) {
  switch(e) {
  case ENC_OK:
    return "ENC_OK";
  case ENC_ERR_MEM:
    return "ENC_ERR_MEM";
  case ENC_ERR_OPCODE_MAJ:
    return "ENC_ERR_OPCODE_MAJ";
  case ENC_ERR_OPCODE_MIN:
    return "ENC_ERR_OPCODE_MIN";
  case ENC_ERR_ADDR:
    return "ENC_ERR_ADDR";
  case ENC_ERR_INSTR:
    return "ENC_ERR_INSTR";
  case ENC_ERR_WORD:
    return "ENC_ERR_WORD";
  case ENC_ERR_PADDING:
    return "ENC_ERR_PADDING";
  default:
    return "ENC_ERR_UNKNOWN";
  }
}

EncodingStatus enc_opcode(uint8_t major, uint8_t minor, uint8_t *opcode_bfr) {
  if (NULL == opcode_bfr) {
    return ENC_ERR_MEM;
  }
  else if (major > 0x1F) {
    return ENC_ERR_OPCODE_MAJ;
  }
  else if (minor > 0x7) {
    return ENC_ERR_OPCODE_MIN;
  }
  *opcode_bfr = (major << 3) | minor;
  return ENC_OK;
}

/* Called by other functions */
static uint8_t inline _addr_valid(uint8_t major, uint16_t addr) {
  /* Most instructions use the address to search memory
   * Memory holds 1000 addresses, so valid addr values are 0x0 to 0x3e7
   * Some instructions do not require an addr field at all
   * TODO: for security reasons, should these not accept one at all?
   * Others do not use X as a lookup but instead as, e.g.
   *   SHL/R: uses X as a shift value. This means stricter bounds: X must be within 40.
   */
  switch(major) {
    case _LDMQ:
    case _MOVE:
      /* addr immaterial */
      return TRUE;
    case _SHL:
    case _SHR:
      /* Shift value must be within 0 to 0x27 */
      return (addr > 0x27) ? FALSE : TRUE;
    case _BR_L:
    case _BR_R:
    case _CNDBR_L:
    case _CNDBR_R:
      /* Branching writes to PC */
    case _LOAD:
    case _MUL:
    case _DIV:
    case _STORE:
    case _AMOD_L:
    case _AMOD_R:
    case _IO:
    default:
      return (addr > MAX_ADDR) ? FALSE : TRUE;
  }
  return TRUE;
}

EncodingStatus enc_instruction(uint8_t major, uint8_t minor, uint16_t addr, Instruction *instr_bfr) {
  if (NULL ==  instr_bfr) {
    return ENC_ERR_MEM;
  }
  else if (major >= _MAX_MAJOR_OPCODE) {
    return ENC_ERR_OPCODE_MAJ;
  }
  else if (minor > (uint8_t)7) {
    return ENC_ERR_OPCODE_MIN;
  }
  else if (!_addr_valid(major, addr)) {
    return ENC_ERR_ADDR;
  }
  switch (major) {
    case _LOAD:
      if (minor >= _MAX_LOAD_OPCODE) {
        return ENC_ERR_OPCODE_MIN;
      }
      break;
    case _MOVE:
      if (minor >= _MAX_MOVE_OPCODE) {
        return ENC_ERR_OPCODE_MIN;
      }
      break;
    case _IO:
      if (minor >= _MAX_IO_OPCODE) {
        return ENC_ERR_OPCODE_MIN;
      }
      break;
    default:
      /* TODO: Check instructions which REQUIRE an address */
      break;
  }
  *instr_bfr = ENC_INSTRUCTION(major, minor, addr);
  return ENC_OK;
}
EncodingStatus enc_instruction_pair(Instruction l, Instruction r, Word *word_bfr) {
  if (NULL == word_bfr) {
    return ENC_ERR_MEM;
  }
  *word_bfr = ENC_INSTR_WORD(l, r);
  return ENC_OK;
}
