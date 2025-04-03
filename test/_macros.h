#pragma once

#define LOAD    (0x01)
#define MUL     (0x02)
#define DIV     (0x03)
#define LDMQ    (0x04)
#define SHL     (0x05)
#define SHR     (0x06)
#define STORE   (0x07)
#define BR_L    (0x08)
#define BR_R    (0x09)
#define CNDBR_L (0x0A)
#define CNDBR_R (0x0B)
#define MOVE    (0x0C)
#define AMOD_L  (0x0D)
#define AMOD_R  (0x0E)
#define IO      (0x0F)

/* Sub opcodes, paired with certain major opcodes */

/* LOAD subcodes */
#define LDABS    (0x00)
#define LD       (0x01)
#define LDNEG    (0x02)
#define ADDABS   (0x03)
#define SUBABS   (0x04)
#define ADD      (0x05)
#define SUB      (0x06)
/* MOVE subcodes */
#define MVABS    (0x00)
#define MVNEGABS (0x01)
#define MV       (0x02)
#define MVNEG    (0x03)
#define ADDMQABS (0x04)
#define SUBMQABS (0x05)
#define ADDMQ    (0x06)
#define SUBMQ    (0x07)
/* IO subcodes */
#define INCARD   (0x00)
#define OUTCARD  (0x01)
