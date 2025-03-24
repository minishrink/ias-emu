# IAS Machine Simulator

---

This project aims to build a simulator for the IAS Machine, built by John von Neumann from 1946-51 at the Institute for Advanced Study.  
Described as the first electronic computer, it is the first computer to use binary encodings and von Neumann architecture.  
  
## TODO

Project under construction. Current aims:  
- write an assembler utility to interpret the assembly
- write an emulator of the architecture so that the assembly can 'run' in here
- finally: write example programs that can run in this environment.
  
---
    
## Architecture

#### AEBI

Words are 40 bits long, and can be either:
- a signed value, with 1 sign bit and 39 numeric bits
- a pair of 20-bit instructions.  (This implies no complex alignment).
  
There are no other data types defined at this level.  

#### Instruction Set Architecture

The instruction set consists of 21 unique instructions, including two undefined behaviour ones.  
These instructions are largely for simple arithmetic or LOAD/STORE memory operations.  

An instruction consists of an 8-bit opcode and 12 bits for operands, although not all instructions use all 12 bits.  
Note that this instruction set includes self-modifying code to rewrite the 12 operand bits of different instructions.  

#### Registers

There are two programmer-facing 40-bit registers:  
- **AC**: the accumulator, where addition and subtraction results are stored  
- **MQ**: the multiplication quotient. Multiplication and division results take 80-bits, so the most significant 40 bits are stored in the AC, with the least significant in the MQ.  

Internal registers include:  
- **PC (Program Counter)**:  used to hold the 8-bit opcode of the current instruction  
- **MBR (Memory Buffer Register)** : a temporary register to hold a value for ALU computations, or a memory address offset  
- **MAR (Memory Address Register)**: a temporary register to hold memory addresses as a result of computation or before retrieval from memory.  
- **IR (Instruction Register)**: register holding the current instruction to execute  
- **IBR (Instruction Buffer Register)**: register holding the next instruction to execute  


#### Memory

The memory consists of numeric punch cards which can be loaded, interpreted, and swapped out. 
One memory card can be loaded at a time, and holds 1000 words.  
There are instructions to load different cards, although this will have to be emulated in a very different way here.  

