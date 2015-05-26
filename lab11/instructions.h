#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

// icodes for all instructions
#define IHALT   0x0
#define INOP    0x1
#define IRRMOVL 0x2
#define IIRMOVL 0x3
#define IRMMOVL 0X4
#define IMRMOVL 0x5
#define IOPL    0x6
#define IJXX    0x7
#define ICMOVXX 0x2
#define ICALL   0x8
#define IRET    0x9
#define IPUSHL  0xA
#define IPOPL   0xB
#define IDUMP   0xC

// ifuns for all instructions
#define FNONE 0x0

// opl ifuns
#define FADDL 0x0
#define FSUBL 0x1
#define FANDL 0x2
#define FXORL 0x3

// cmovxx ifuns
#define F_NO_CND 0
#define F_LE 1
#define F_L  2
#define F_E  3
#define F_NE 4
#define F_GE 5
#define F_G  6

// Processor instruction status codes
#define SAOK 1
#define SHLT 2
#define SADR 3
#define SINS 4

// An instruction execution function
typedef unsigned int (*InstructionFunction)();

#endif


