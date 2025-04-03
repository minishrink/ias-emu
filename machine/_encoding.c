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
  else if (addr > (uint16_t)0x3e7) {
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
