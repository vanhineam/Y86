#ifndef REGISTERS_H
#define REGISTERS_H

// register identifiers
#define EAX    0x0
#define ECX    0x1
#define EDX    0x2
#define EBX    0x3
#define ESP    0x4
#define EBP    0x5
#define ESI    0x6
#define EDI    0x7

#define REGSIZE 8         //number of registers

#define ZF     0x2        //zero flag is bit 2 of CC
#define SF     0x1        //sign flag is bit 1 of CC
#define OF     0x0        //overflow flag is bit 0 of CC
#define RNONE  0xf        //no register needed

/**
 * Returns the value of a single register.
 * @param regNum the number of the register to be return.
 * @return The value of the specified register, or 0 if regNum is invalid.
 */
extern unsigned int getRegister(int regNum);

/**
 * Sets the value of a single register. If regNum is invalid, no registers
 * are modified.
 * @param regNum the number of the register to be set.
 * @param the new value of the register.
 */
extern void setRegister(int regNum, unsigned int regValue);

/**
 * Sets all registers to 0.
 */
extern void clearRegisters();

/**
 * Sets the value of a bit in the condition code register. If bitNumber is
 * invalid the register is unchanged.
 * @param bitNumber the bit to be set.
 * @param value the new value of the bit.
 */
extern void setCC(unsigned int bitNumber, unsigned int value);

/**
 * Returns the value of a bit in the condition code register.
 * @param bitNumber the bit to be read.
 * @return The value of the bit, or 0 if bitNumer is invalid.
 */
extern unsigned int getCC(unsigned int bitNumber);

#endif

