#include <iostream>
#include "Z80.h"


// Constructor
Z80::Z80(Memory* memory)
	: pMemory(memory)
{
	registers = { 0x01, 0xB0, 0x00, 0x13, 0x00, 0xD8, 0x01, 0x4D, 0x100, 0xFFFE };
	bIME = false;
	bHalt = false;
	bStop = false;
	cpuClockCycles = 0;
	
	instructionSet = new Function[256]{
		& Z80::i00, &Z80::i01, &Z80::i02, &Z80::i03, &Z80::i04, &Z80::i05, &Z80::i06, &Z80::i07,
		& Z80::i08, &Z80::i09, &Z80::i0A, &Z80::i0B, &Z80::i0C, &Z80::i0D, &Z80::i0E, &Z80::i0F,            // 00 - 0F
		& Z80::i10, &Z80::i11, &Z80::i12, &Z80::i13, &Z80::i14, &Z80::i15, &Z80::i16, &Z80::i17,
		& Z80::i18, &Z80::i19, &Z80::i1A, &Z80::i1B, &Z80::i1C, &Z80::i1D, &Z80::i1E, &Z80::i1F,            // 10 - 1F
		& Z80::i20, &Z80::i21, &Z80::i22, &Z80::i23, &Z80::i24, &Z80::i25, &Z80::i26, &Z80::i27,
		& Z80::i28, &Z80::i29, &Z80::i2A, &Z80::i2B, &Z80::i2C, &Z80::i2D, &Z80::i2E, &Z80::i2F,            // 20 - 2F
		& Z80::i30, &Z80::i31, &Z80::i32, &Z80::i33, &Z80::i34, &Z80::i35, &Z80::i36, &Z80::i37,
		& Z80::i38, &Z80::i39, &Z80::i3A, &Z80::i3B, &Z80::i3C, &Z80::i3D, &Z80::i3E, &Z80::i3F,            // 30 - 3F
		& Z80::i40, &Z80::i41, &Z80::i42, &Z80::i43, &Z80::i44, &Z80::i45, &Z80::i46, &Z80::i47,
		& Z80::i48, &Z80::i49, &Z80::i4A, &Z80::i4B, &Z80::i4C, &Z80::i4D, &Z80::i4E, &Z80::i4F,            // 40 - 4F
		& Z80::i50, &Z80::i51, &Z80::i52, &Z80::i53, &Z80::i54, &Z80::i55, &Z80::i56, &Z80::i57,
		& Z80::i58, &Z80::i59, &Z80::i5A, &Z80::i5B, &Z80::i5C, &Z80::i5D, &Z80::i5E, &Z80::i5F,            // 50 - 5F
		& Z80::i60, &Z80::i61, &Z80::i62, &Z80::i63, &Z80::i64, &Z80::i65, &Z80::i66, &Z80::i67,
		& Z80::i68, &Z80::i69, &Z80::i6A, &Z80::i6B, &Z80::i6C, &Z80::i6D, &Z80::i6E, &Z80::i6F,            // 60 - 6F
		& Z80::i70, &Z80::i71, &Z80::i72, &Z80::i73, &Z80::i74, &Z80::i75, &Z80::i76, &Z80::i77,
		& Z80::i78, &Z80::i79, &Z80::i7A, &Z80::i7B, &Z80::i7C, &Z80::i7D, &Z80::i7E, &Z80::i7F,            // 70 - 7F
		& Z80::i80, &Z80::i81, &Z80::i82, &Z80::i83, &Z80::i84, &Z80::i85, &Z80::i86, &Z80::i87,
		& Z80::i88, &Z80::i89, &Z80::i8A, &Z80::i8B, &Z80::i8C, &Z80::i8D, &Z80::i8E, &Z80::i8F,            // 80 - 8F
		& Z80::i90, &Z80::i91, &Z80::i92, &Z80::i93, &Z80::i94, &Z80::i95, &Z80::i96, &Z80::i97,
		& Z80::i98, &Z80::i99, &Z80::i9A, &Z80::i9B, &Z80::i9C, &Z80::i9D, &Z80::i9E, &Z80::i9F,            // 90 - 9F
		& Z80::iA0, &Z80::iA1, &Z80::iA2, &Z80::iA3, &Z80::iA4, &Z80::iA5, &Z80::iA6, &Z80::iA7,
		& Z80::iA8, &Z80::iA9, &Z80::iAA, &Z80::iAB, &Z80::iAC, &Z80::iAD, &Z80::iAE, &Z80::iAF,            // A0 - AF
		& Z80::iB0, &Z80::iB1, &Z80::iB2, &Z80::iB3, &Z80::iB4, &Z80::iB5, &Z80::iB6, &Z80::iB7,
		& Z80::iB8, &Z80::iB9, &Z80::iBA, &Z80::iBB, &Z80::iBC, &Z80::iBD, &Z80::iBE, &Z80::iBF,            // B0 - BF
		& Z80::iC0, &Z80::iC1, &Z80::iC2, &Z80::iC3, &Z80::iC4, &Z80::iC5, &Z80::iC6, &Z80::iC7,
		& Z80::iC8, &Z80::iC9, &Z80::iCA, &Z80::iCB, &Z80::iCC, &Z80::iCD, &Z80::iCE, &Z80::iCF,            // C0 - CF
		& Z80::iD0, &Z80::iD1, &Z80::iD2, &Z80::iD3, &Z80::iD4, &Z80::iD5, &Z80::iD6, &Z80::iD7,
		& Z80::iD8, &Z80::iD9, &Z80::iDA, &Z80::iDB, &Z80::iDC, &Z80::iDD, &Z80::iDE, &Z80::iDF,            // D0 - DF
		& Z80::iE0, &Z80::iE1, &Z80::iE2, &Z80::iE3, &Z80::iE4, &Z80::iE5, &Z80::iE6, &Z80::iE7,
		& Z80::iE8, &Z80::iE9, &Z80::iEA, &Z80::iEB, &Z80::iEC, &Z80::iED, &Z80::iEE, &Z80::iEF,            // E0 - EF
		& Z80::iF0, &Z80::iF1, &Z80::iF2, &Z80::iF3, &Z80::iF4, &Z80::iF5, &Z80::iF6, &Z80::iF7,
		& Z80::iF8, &Z80::iF9, &Z80::iFA, &Z80::iFB, &Z80::iFC, &Z80::iFD, &Z80::iFE, &Z80::iFF				// F0 - FF
	};

	cbInstructionSet = new Function[256]{
		& Z80::iCB00, &Z80::iCB01, &Z80::iCB02, &Z80::iCB03, &Z80::iCB04, &Z80::iCB05, &Z80::iCB06, &Z80::iCB07,
		& Z80::iCB08, &Z80::iCB09, &Z80::iCB0A, &Z80::iCB0B, &Z80::iCB0C, &Z80::iCB0D, &Z80::iCB0E, &Z80::iCB0F,	// 00 - 0F
		& Z80::iCB10, &Z80::iCB11, &Z80::iCB12, &Z80::iCB13, &Z80::iCB14, &Z80::iCB15, &Z80::iCB16, &Z80::iCB17,
		& Z80::iCB18, &Z80::iCB19, &Z80::iCB1A, &Z80::iCB1B, &Z80::iCB1C, &Z80::iCB1D, &Z80::iCB1E, &Z80::iCB1F,    // 10 - 1F
		& Z80::iCB20, &Z80::iCB21, &Z80::iCB22, &Z80::iCB23, &Z80::iCB24, &Z80::iCB25, &Z80::iCB26, &Z80::iCB27,
		& Z80::iCB28, &Z80::iCB29, &Z80::iCB2A, &Z80::iCB2B, &Z80::iCB2C, &Z80::iCB2D, &Z80::iCB2E, &Z80::iCB2F,    // 20 - 2F
		& Z80::iCB30, &Z80::iCB31, &Z80::iCB32, &Z80::iCB33, &Z80::iCB34, &Z80::iCB35, &Z80::iCB36, &Z80::iCB37,
		& Z80::iCB38, &Z80::iCB39, &Z80::iCB3A, &Z80::iCB3B, &Z80::iCB3C, &Z80::iCB3D, &Z80::iCB3E, &Z80::iCB3F,    // 30 - 3F
		& Z80::iCB40, &Z80::iCB41, &Z80::iCB42, &Z80::iCB43, &Z80::iCB44, &Z80::iCB45, &Z80::iCB46, &Z80::iCB47,
		& Z80::iCB48, &Z80::iCB49, &Z80::iCB4A, &Z80::iCB4B, &Z80::iCB4C, &Z80::iCB4D, &Z80::iCB4E, &Z80::iCB4F,    // 40 - 4F
		& Z80::iCB50, &Z80::iCB51, &Z80::iCB52, &Z80::iCB53, &Z80::iCB54, &Z80::iCB55, &Z80::iCB56, &Z80::iCB57,
		& Z80::iCB58, &Z80::iCB59, &Z80::iCB5A, &Z80::iCB5B, &Z80::iCB5C, &Z80::iCB5D, &Z80::iCB5E, &Z80::iCB5F,    // 50 - 5F
		& Z80::iCB60, &Z80::iCB61, &Z80::iCB62, &Z80::iCB63, &Z80::iCB64, &Z80::iCB65, &Z80::iCB66, &Z80::iCB67,
		& Z80::iCB68, &Z80::iCB69, &Z80::iCB6A, &Z80::iCB6B, &Z80::iCB6C, &Z80::iCB6D, &Z80::iCB6E, &Z80::iCB6F,    // 60 - 6F
		& Z80::iCB70, &Z80::iCB71, &Z80::iCB72, &Z80::iCB73, &Z80::iCB74, &Z80::iCB75, &Z80::iCB76, &Z80::iCB77,
		& Z80::iCB78, &Z80::iCB79, &Z80::iCB7A, &Z80::iCB7B, &Z80::iCB7C, &Z80::iCB7D, &Z80::iCB7E, &Z80::iCB7F,    // 70 - 7F
		& Z80::iCB80, &Z80::iCB81, &Z80::iCB82, &Z80::iCB83, &Z80::iCB84, &Z80::iCB85, &Z80::iCB86, &Z80::iCB87,
		& Z80::iCB88, &Z80::iCB89, &Z80::iCB8A, &Z80::iCB8B, &Z80::iCB8C, &Z80::iCB8D, &Z80::iCB8E, &Z80::iCB8F,    // 80 - 8F
		& Z80::iCB90, &Z80::iCB91, &Z80::iCB92, &Z80::iCB93, &Z80::iCB94, &Z80::iCB95, &Z80::iCB96, &Z80::iCB97,
		& Z80::iCB98, &Z80::iCB99, &Z80::iCB9A, &Z80::iCB9B, &Z80::iCB9C, &Z80::iCB9D, &Z80::iCB9E, &Z80::iCB9F,    // 90 - 9F
		& Z80::iCBA0, &Z80::iCBA1, &Z80::iCBA2, &Z80::iCBA3, &Z80::iCBA4, &Z80::iCBA5, &Z80::iCBA6, &Z80::iCBA7,
		& Z80::iCBA8, &Z80::iCBA9, &Z80::iCBAA, &Z80::iCBAB, &Z80::iCBAC, &Z80::iCBAD, &Z80::iCBAE, &Z80::iCBAF,    // A0 - AF
		& Z80::iCBB0, &Z80::iCBB1, &Z80::iCBB2, &Z80::iCBB3, &Z80::iCBB4, &Z80::iCBB5, &Z80::iCBB6, &Z80::iCBB7,
		& Z80::iCBB8, &Z80::iCBB9, &Z80::iCBBA, &Z80::iCBBB, &Z80::iCBBC, &Z80::iCBBD, &Z80::iCBBE, &Z80::iCBBF,    // B0 - BF
		& Z80::iCBC0, &Z80::iCBC1, &Z80::iCBC2, &Z80::iCBC3, &Z80::iCBC4, &Z80::iCBC5, &Z80::iCBC6, &Z80::iCBC7,
		& Z80::iCBC8, &Z80::iCBC9, &Z80::iCBCA, &Z80::iCBCB, &Z80::iCBCC, &Z80::iCBCD, &Z80::iCBCE, &Z80::iCBCF,    // C0 - CF
		& Z80::iCBD0, &Z80::iCBD1, &Z80::iCBD2, &Z80::iCBD3, &Z80::iCBD4, &Z80::iCBD5, &Z80::iCBD6, &Z80::iCBD7,
		& Z80::iCBD8, &Z80::iCBD9, &Z80::iCBDA, &Z80::iCBDB, &Z80::iCBDC, &Z80::iCBDD, &Z80::iCBDE, &Z80::iCBDF,    // D0 - DF
		& Z80::iCBE0, &Z80::iCBE1, &Z80::iCBE2, &Z80::iCBE3, &Z80::iCBE4, &Z80::iCBE5, &Z80::iCBE6, &Z80::iCBE7,
		& Z80::iCBE8, &Z80::iCBE9, &Z80::iCBEA, &Z80::iCBEB, &Z80::iCBEC, &Z80::iCBED, &Z80::iCBEE, &Z80::iCBEF,    // E0 - EF
		& Z80::iCBF0, &Z80::iCBF1, &Z80::iCBF2, &Z80::iCBF3, &Z80::iCBF4, &Z80::iCBF5, &Z80::iCBF6, &Z80::iCBF7,
		& Z80::iCBF8, &Z80::iCBF9, &Z80::iCBFA, &Z80::iCBFB, &Z80::iCBFC, &Z80::iCBFD, &Z80::iCBFE, &Z80::iCBFF		// F0 - FF
	};
}


// Destructor
Z80::~Z80() {
	delete[] instructionSet;
	delete[] cbInstructionSet;
}


// Execute Loop
void Z80::execute() {
	BIT8 aValue = (pMemory->readByte(registers.PC++));
	printf("Z80::execute() - %i : %02X\n", registers.PC-1, aValue);
	Function instruction = instructionSet[aValue];
	(this->*instruction)();
}


// Debug Code: Print Registers. Add wherever necessary for debugging.
void Z80::printRegisters() {
	printf("A(%i)\tF(%i)\tB(%i)\tC(%i)\n", registers.A, registers.F, registers.B, registers.C);
	printf("D(%i)\tE(%i)\tH(%i)\tL(%i)\n", registers.D, registers.E, registers.H, registers.L);
	printf("PC(%i)\tSP(%i)\n", registers.PC, registers.SP);
}


// Get current CPU Clock Cycles
int Z80::getCpuClockCycles() {
	return cpuClockCycles;
}


// Set the Program Counter
void Z80::setProgramCounter(BIT16 val) {
	registers.PC = val;// -1; // TODO: Should I be removing 1 here?
}


// Put the Program Counter onto the stack
void Z80::putPCOnStack()
{
	pMemory->writeByte(--registers.SP, (registers.PC>>8));
	pMemory->writeByte(--registers.SP, (registers.PC&0xFF));
}


// Combine H&L
BIT16 Z80::getHL() {
	return (BIT16)(((BIT16)registers.H << 8) + registers.L);
}


// Combine A&F
BIT16 Z80::getAF() {
	return (BIT16)(((BIT16)registers.A << 8) + registers.F);
}


// Combine B&C
BIT16 Z80::getBC() {
	return (BIT16)(((BIT16)registers.B << 8) + registers.C);
}


// Combine D&E
BIT16 Z80::getDE() {
	return (BIT16)(((BIT16)registers.D << 8) + registers.E);
}


// Interrupt Master Enable
void Z80::setIme(bool interrupt) {
	bIME = interrupt;
}
bool Z80::getIme() {
	return bIME;
}


/**
	Put this in array where instuction doesn't exist.
	This is mainly for debug so i get output if accidentally called.
*/
void Z80::xx() {
	printf("This instruction does not exist! (%i)\n", pMemory->readByte(registers.PC-1));
}


/**
	NOP
	No Operation
	- - - -
*/
void Z80::nop() {
	cpuClockCycles = 4;
}


/**
	STOP
	Stop instructions
	- - - -
*/
void Z80::stop() {
	bStop = bIME;		// TODO: Why doesnt this always set to true?
	cpuClockCycles = 4;
}


/**
	HALT
	Halt instructions.
	- - - -
*/
void Z80::halt() {
	bHalt = bIME;		// TODO: Why doesnt this always set to true?
	cpuClockCycles = 4;
}


/**
	PREFIX CB
	Run CB instruction.
	- - - -
*/
void Z80::runCB() {
	BIT8 i = pMemory->readByte(registers.PC++);
	printf("Z80::runCB() - Instruction: %i\n", i);
	cpuClockCycles = 4;							// TODO: Should this be added to the cb instruction? If so, move after if loop and just add 4.
	Function cbInstruction = instructionSet[i];
	(this->*cbInstruction)();
}


/**
	DI / EI
	Enable/Disable Interrupts
	- - - -
*/
void Z80::diei(bool interrupt) {
	setIme(interrupt);
	cpuClockCycles = 4;
}


/**
	JR nn
	Add n to current address and jump to it.
	- - - -
*/
void Z80::jrn() {
	signed char j = pMemory->readByte(registers.PC);
	registers.PC += j +1; // TODO: Not sure if adding 1 is correct or not.
	cpuClockCycles = 8;
}


/**
	JR NZ,nn
	If Flag Z reset, JR nn
	- - - -
*/
void Z80::jrnz() {
	if (registers.F &flags.Z) {
		registers.PC++;
		cpuClockCycles = 12;
	}
	else {
		jrn();
		cpuClockCycles = 16;
	}
}


/**
	JR Z,nn
	If Flag Z set, JR nn
	- - - -
*/
void Z80::jrz() {
	if (registers.F &flags.Z) {
		jrn();
		cpuClockCycles = 16;
	}
	else {
		registers.PC++;
		cpuClockCycles = 12;
	}
}


/**
	JR NC,nn
	If Flag C reset, JR nn
	- - - -
*/
void Z80::jrnc() {
	if (registers.F &flags.C) {
		registers.PC++;
		cpuClockCycles = 12;
	}
	else {
		jrn();
		cpuClockCycles = 16;
	}
}


/**
	JR C,nn
	If Flag C set, JR nn
	- - - -
*/
void Z80::jrc() {
	if (registers.F &flags.C) {
		jrn();
		cpuClockCycles = 16;
	}
	else {
		registers.PC++;
		cpuClockCycles = 12;
	}
}


/**
	RST n
	Push present address onto stack and jump to address n
	- - - -
*/
void Z80::rst(BIT8 X) {
	pMemory->writeByte(--registers.SP, (registers.PC >> 8));
	pMemory->writeByte(--registers.SP, (registers.PC & 0xFF));
	registers.PC = X;
	cpuClockCycles = 32;
}


/**
	CALL nn
	Push address of next instruction onto stack, and jump to address nn
	- - - -
*/
void Z80::calln() {
	BIT8 i = pMemory->readByte(registers.PC++);
	BIT8 j = pMemory->readByte(registers.PC++);
	pMemory->writeByte(--registers.SP, (registers.PC >> 8));
	pMemory->writeByte(--registers.SP, ((BIT8)(registers.PC &0xFF)));
	registers.PC = ((j << 8) + i);
	cpuClockCycles = 24;
}


/**
	CALL NZ,nn
	If Flag Z reset, CALL nn
	- - - -
*/
void Z80::callnz() {
	if (registers.F &flags.Z) {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
	else {
		calln();
		cpuClockCycles = 24;
	}
}


/**
	CALL Z,nn
	If Flag Z set, CALL nn
	- - - -
*/
void Z80::callz() {
	if (registers.F &flags.Z) {
		calln();
		cpuClockCycles = 24;
	}
	else {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
}


/**
	CALL NC,nn
	If Flag C reset, CALL nn
	- - - -
*/
void Z80::callnc() {
	if (registers.F &flags.C) {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
	else {
		calln();
		cpuClockCycles = 24;
	}
}


/**
	CALL C,nn
	If Flag C set, CALL nn
	- - - -
*/
void Z80::callc() {
	if (registers.F &flags.C) {
		calln();
		cpuClockCycles = 24;
	}
	else {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
}


/**
	JP nn
	Jump to address nn
	- - - -
*/
void Z80::jpn() {
	BIT8 i = pMemory->readByte(registers.PC++);
	BIT8 j = pMemory->readByte(registers.PC);
	registers.PC = ((j << 8) + i);
	cpuClockCycles = 12;
}
void Z80::jphl() {
	registers.PC = getHL();
	cpuClockCycles = 4;
}


/**
	JP NZ,nn
	If Flag Z reset, JP nn
	- - - -
*/
void Z80::jpnz() {
	if (registers.F &flags.Z) {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
	else {
		jpn();
		cpuClockCycles = 16;
	}
}


/**
	JP Z,nn
	If Flag Z set, JP nn
	- - - -
*/
void Z80::jpz() {
	if (registers.F &flags.Z) {
		jpn();
		cpuClockCycles = 16;
	}
	else {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
}


/**
	JP NC,nn
	If Flag C reset, JP nn
	- - - -
*/
void Z80::jpnc() {
	if (registers.F &flags.C) {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
	else {
		jpn();
		cpuClockCycles = 16;
	}
}


/**
	JP C,nn
	If Flag C set, JP nn
	- - - -
*/
void Z80::jpc() {
	if (registers.F &flags.C) {
		jpn();
		cpuClockCycles = 16;
	}
	else {
		registers.PC += 2;
		cpuClockCycles = 12;
	}
}


/**
	RET
	Pop 2 bytes from stack and jump to that address
	- - - -
*/
void Z80::ret() {
	BIT8 i = pMemory->readByte(registers.SP++);
	BIT8 j = pMemory->readByte(registers.SP++);
	registers.PC = ((j << 8) + i);
	cpuClockCycles = 8;
}


/**
	RET NZ
	If Flag Z reset, RET
	- - - -
*/
void Z80::retnz() {
	if (registers.F &flags.Z) {
		cpuClockCycles = 8;
	}
	else {
		ret();
		cpuClockCycles = 20;
	}
}


/**
	RET Z
	If Flag Z set, RET
	- - - -
*/
void Z80::retz() {
	if (registers.F &flags.Z) {
		ret();
		cpuClockCycles = 20;
	}
	else {
		cpuClockCycles = 8;
	}
}


/**
	RET NC
	If Flag C reset, RET
	- - - -
*/
void Z80::retnc() {
	if (registers.F &flags.C) {
		cpuClockCycles = 8;
	}
	else {
		ret();
		cpuClockCycles = 20;
	}
}


/**
	RET C
	If Flag C set, RET
	- - - -
*/
void Z80::retc() {
	if (registers.F &flags.C) {
		ret();
		cpuClockCycles = 20;
	}
	else {
		cpuClockCycles = 8;
	}
}


/**
	RETI
	RET and enable interrupts
	- - - -
*/
void Z80::reti() {
	setIme(true);
	ret();
}


/**
	LD (nn),r2
	Put value at address nn into r1
	- - - -
*/
void Z80::ldw(BIT8 X, BIT8 Y, BIT8 Z) {
	pMemory->writeByte((X << 8) + Y, Z);
	cpuClockCycles = 8;
}


/**
	LD (HL),n
	Put value at address nn into r1
	- - - -
*/
void Z80::ldhl() {
	pMemory->writeByte(getHL(), pMemory->readByte(registers.PC++));
	cpuClockCycles = 12;
}


/**
	LD (r1),r2
	Put value r2 into r1
	- - - -
*/
void Z80::ldr(BIT8* X, BIT8 Y, BIT8 Z) {
	*X = pMemory->readByte((BIT16)((Y << 8) + Z));
	cpuClockCycles = 8;
}


/**
	LD nn,n
	Put value n into nn
	- - - -
*/
void Z80::ldr(BIT8* X) {
	*X = pMemory->readByte(registers.PC++);
	cpuClockCycles = 8;
}


/**
	LD r1,r2
	Put value r2 into r1
	- - - -
*/
void Z80::ld(BIT8* X, BIT8 Y) {
	*X = Y;
	cpuClockCycles = 4;
}


/**
	LDH (n),A
	Put A into memory address 0xFF00+n
	- - - -
*/
void Z80::ldhw() {
	pMemory->writeByte(0xFF00 + pMemory->readByte(registers.PC++), registers.A);
	cpuClockCycles = 12;
}


/**
	LDH A,(n)
	Put memory address 0xFF00+n into A
	- - - -
*/
void Z80::ldhr() {
	registers.A = pMemory->readByte((BIT16)(0xFF00 + pMemory->readByte(registers.PC++)));
	cpuClockCycles = 12;
}


/**
	LD (nn), A
	Put register A into memory address nn.
	- - - -
*/
void Z80::ldna() {
	BIT8 i = pMemory->readByte(registers.PC++);
	BIT8 j = pMemory->readByte(registers.PC++);
	pMemory->writeByte((BIT16)((j << 8) + i), registers.A);
	cpuClockCycles = 16;
}


/**
	LD A,(nn)
	Put value at address nn into A.
	- - - -
*/
void Z80::ldan() {
	BIT8 i = pMemory->readByte(registers.PC++);
	BIT8 j = pMemory->readByte(registers.PC++);
	registers.A = pMemory->readByte((j << 8) + i);
	cpuClockCycles = 16;
}


/**
	LD (HL+),A
	Put value A into address HL. Increment HL.
	- - - -
*/
void Z80::ldhlpa() {
	pMemory->writeByte(getHL(), registers.A);
	registers.L = (registers.L + 1) &0xFF;
	if (!registers.L)
		registers.H = (registers.H + 1) &0xFF;
	cpuClockCycles = 8;
}


/**
	LD A,(HL+)
	Put value at address HL into A. Increment HL.
	- - - -
*/
void Z80::ldahlp() {
	registers.A = pMemory->readByte(getHL());
	registers.L = (registers.L + 1) &0xFF;
	if (!registers.L)
		registers.H = (registers.H + 1) &0xFF;
	cpuClockCycles = 8;
}


/**
	LD (HL-), A
	Put value A into address HL. Decrement HL.
	- - - -
*/
void Z80::ldhlma() {
	pMemory->writeByte(getHL(), registers.A);
	
	if (registers.L == 0) {
		registers.L = 0xFF;
		registers.H--;
	}
	else {
		registers.L--;
	}

	cpuClockCycles = 8;
}


/**
	LD A,(HL-)
	Put value at address HL into A. Decrement HL.
	- - - -
*/
void Z80::ldahlm() {
	registers.A = pMemory->readByte(getHL());
	
	if (registers.L == 0) {
		registers.L = 0xFF;
		registers.H--;
	}
	else {
		registers.L--;
	}

	cpuClockCycles = 8;
}


/**
	LD n,nn
	Put value nn into n.
	- - - -
*/
void Z80::ld16(BIT8* X, BIT8* Y) {
	*Y = pMemory->readByte(registers.PC++);
	*X = pMemory->readByte(registers.PC++);
	cpuClockCycles = 12;
}
void Z80::ldsp16() {
	BIT8 i = pMemory->readByte(registers.PC++);
	BIT8 j = pMemory->readByte(registers.PC++);
	registers.SP = (j << 8) + i;
	cpuClockCycles = 12;
}


/**
	LD (nn),SP
	Put Stack Pointer at address nn.
	- - - -
*/
void Z80::ld16sp() {
	BIT8 i = pMemory->readByte(registers.PC++);
	BIT8 j = pMemory->readByte(registers.PC++);
	BIT16 k = (j << 8) + i;
	pMemory->writeByte(k++, (registers.SP & 0xFF));
	pMemory->writeByte(k, (registers.SP >> 8));
	cpuClockCycles = 20;
}


/**
	POP nn
	Pop two bytes off stack int register pair nn. Increment SP by 2.
	- - - -
*/
void Z80::pop(BIT8* X, BIT8* Y) {
	*Y = pMemory->readByte(registers.SP++);
	*X = pMemory->readByte(registers.SP++);
	cpuClockCycles = 12;
}


/**
	PUSH nn
	Push registers pair nn onto stack.
	- - - -
*/
void Z80::push(BIT8 X, BIT8 Y) {
	pMemory->writeByte(--registers.SP, X);
	pMemory->writeByte(--registers.SP, Y);
	cpuClockCycles = 16;
}


/**
	LD HL,SP+n
	Put SP+n address into HL.
	0 0 H C
*/
void Z80::ldhlspn() {
	signed char i = pMemory->readByte(registers.PC++);
	BIT16 j = registers.SP + i;
	registers.F &= ~flags.Z;										// Set Z(0)
	registers.F &= ~flags.N;										// Set N(0)
	registers.F |= (registers.SP ^ i ^ j) & 0x10 ? flags.H : 0;		// Set H
	registers.F |= (registers.SP ^ i ^ j) & 0x100 ? flags.C : 0;	// Set C
	registers.H = (BIT8)(j >> 8);
	registers.L = j & 0xFF;
	cpuClockCycles = 12;
}


/**
	LD SP,HL
	Put HL into Stack Pointer
	- - - -
*/
void Z80::ldsphl() {
	registers.SP = getHL();
	cpuClockCycles = 8;
}


/*	
	INC n
	Increment n
	Z 0 H -
*/
void Z80::inc(BIT8* X) {
	*X = (*X + 1) & 0xFF;
	registers.F |= ((*X == 0) ? flags.Z : 0);			// Set Z
	registers.F &= ~flags.N;							// Set N (0)
	registers.F |= ((*X & 0x0F) == 0x0F ? flags.H : 0);	// Set H
	cpuClockCycles = 4;
}
void Z80::inchl() {
	BIT8 i = (pMemory->readByte(getHL()) + 1) &0xFF;
	pMemory->writeByte(getHL(), i);
	registers.F |= ((i == 0) ? flags.Z : 0);			// Set Z
	registers.F &= ~flags.N;							// Set N (0)
	registers.F |= ((i & 0x0F) == 0x0F ? flags.H : 0);	// Set H
	cpuClockCycles = 12;
}


/**
	DEC n
	Decrement n
	Z 1 H -
*/
void Z80::dec(BIT8* X) {
	*X = (*X - 1) & 0xFF;
	registers.F |= ((*X == 0) ? flags.Z : 0);			// Set Z
	registers.F |= flags.N;								// Set N (1)
	registers.F |= ((*X & 0x0F) == 0x0F ? flags.H : 0);	// Set H
	cpuClockCycles = 4;
}
void Z80::dechl() {
	BIT8 i = pMemory->readByte(getHL()) - 1;
	pMemory->writeByte(getHL(), i);
	registers.F |= ((i == 0) ? flags.Z : 0);			// Set Z
	registers.F |= flags.N;								// Set N (1)
	registers.F |= ((i & 0x0F) == 0x0F ? flags.H : 0);	// Set H
	cpuClockCycles = 12;
}


/**
	DAA
	Decimal Adjust A.
	Z - 0 C
*/
void Z80::daa() {
	BIT16 i = registers.A & 0xFFFF;
	BIT8 correction = 0;
	if ((registers.F &flags.H) || (!(registers.F &flags.N) && (i & 0xf) > 9)) {
		correction |= 0x6;
	}

	if ((registers.F &flags.C) || (!(registers.F &flags.N) && i > 0x99)) {
		correction |= 0x60;
		registers.F |= flags.C;
	}

	i += (registers.F &flags.N) ? -correction : correction;
	i &= 0xFF;
	
	registers.F |= (i == 0) ? flags.Z : 0;
	registers.F &= ~(flags.H);

	registers.A = i;
	cpuClockCycles = 4;
}


/**
	CPL
	Complement A register.  (Flip all bits)
	- 1 1 -
*/
void Z80::cpl() {
	registers.A ^= 0xFF;
	registers.F |= flags.N;	// Set N (1)
	registers.F |= flags.H;	// Set H (1)
	cpuClockCycles = 4;
}


/**
	SCF
	Set Carry Flag.
	- 0 0 1
*/
void Z80::scf() {
	registers.F &= ~flags.N;	// Set N (0)
	registers.F &= ~flags.H;	// Set H (0)
	registers.F |= flags.C;		// Set C (1)
	cpuClockCycles = 4;
}


/**
	CCF
	Complement carry flag. If C set, reset. If C reset, set.
	- 0 0 C
*/
void Z80::ccf() {
	registers.F &= ~flags.N;	// Set N(0)
	registers.F &= ~flags.H;	// Set H(0)
	registers.F ^= flags.C;		// Set C
	cpuClockCycles = 4;
}


/**
	ADD A,n
	Add n to A
	Z 0 H C
*/
void Z80::add(BIT8 X) {
	BIT16 i = registers.A + X;
	registers.F |= (i & 0xFF == 0) ? flags.Z : 0;							// Set Z
	registers.F &= ~flags.N;												// Set N (0)
	registers.F |= ((X &0x0F) + (registers.A &0x0F) > 0x0F) ? flags.H : 0;	// Set H
	registers.F |= (i > 0xFF) ? flags.C : 0;								// Set C
	registers.A = (i &0xFF);
	cpuClockCycles = 4;
}
void Z80::addhl() {
	add(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::addn() {
	add(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	ADC A,n
	Add (n + CarryFlag) to A
	Z 0 H C
*/
void Z80::adc(BIT8 X) {
	BIT16 i = registers.A + X + (registers.F&flags.C ? 1 : 0);
	registers.F |= (i & 0xFF) == 0 ? flags.Z : 0;						// Set Z
	registers.F &= ~flags.N;											// Set N(0)
	registers.F |= ((registers.A) ^ (X) ^ (i&0xFF)) ? flags.H : 0;		// Set H
	registers.F |= (i > 0xFF) ? flags.C : 0;							// Set C
	registers.A = (i & 0xFF);
	cpuClockCycles = 4;
}
void Z80::adchl() {
	adc(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::adcn() {
	adc(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	SUB n
	Subtract n from A
	Z 1 H C
*/
void Z80::sub(BIT8 X) {
	BIT16 i = registers.A - X;
	registers.F |= ((i & 0xFF) & 0xFF == 0) ? flags.Z : 0;			// Set Z
	registers.F |= flags.N;											// Set N(1)
	registers.F |= ((X &0x0F) > (registers.A &0x0F)) ? flags.H : 0;	// Set H
	registers.F |= (X > registers.A) ? flags.C : 0;					// Set C
	registers.A = (i & 0xFF);
	cpuClockCycles = 4;
}
void Z80::subhl() {
	sub(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::subn() {
	sub(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	SBC A,n
	Subtract (n + CarryFlag) from A
	Z 1 H C
*/
void Z80::sbc(BIT8 X) {
	int i = registers.A - X - (registers.F&flags.C ? 1 : 0);
	registers.F |= (i & 0xFF) == 0 ? flags.Z : 0;										// Set Z
	registers.F &= ~flags.N;															// Set N(0)
	registers.F |= (((registers.A ^ X ^ (i & 0xff)) & (1 << 4)) != 0) ? flags.H : 0;	// Set H
	registers.F |= (i < 0) ? flags.C : 0;												// Set C
	registers.A = (i & 0xFF);
	cpuClockCycles = 4;
}
void Z80::sbchl() {
	sbc(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::sbcn() {
	sbc(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	AND n
	Logically AND n with A, result in A
	Z 0 1 0
*/
void Z80::_and(BIT8 X) {
	registers.A &= X;
	registers.F |= (registers.A == 0) ? flags.Z : 0;	// Set Z
	registers.F &= ~flags.N;							// Set N(0)
	registers.F |= flags.H;								// Set H(1)
	registers.F &= ~flags.C;							// Set C(0)
	cpuClockCycles = 4;
}
void Z80::andhl() {
	_and(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::andn() {
	_and(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	XOR n
	Logically XOR n with A, result in A
	Z 0 0 0
*/
void Z80::_xor(BIT8 X) {
	registers.A ^= X;
	registers.F |= (registers.A == 0) ? flags.Z : 0;	// Set Z
	registers.F &= ~flags.N;							// Set N(0)
	registers.F &= ~flags.H;							// Set H(0)
	registers.F &= ~flags.C;							// Set C(0)
	cpuClockCycles = 4;
}
void Z80::xorhl() {
	_xor(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::xorn() {
	_xor(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	OR n
	Logically OR n with A, result in A
	Z 0 0 0
*/
void Z80::_or(BIT8 X) {
	registers.A |= X;
	registers.F |= (registers.A == 0) ? flags.Z : 0;	// Set Z
	registers.F &= ~flags.N;							// Set N(0)
	registers.F &= ~flags.H;							// Set H(0)
	registers.F &= ~flags.C;							// Set C(0)
	cpuClockCycles = 4;
}
void Z80::orhl() {
	_or(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::orn() {
	_or(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	CP n
	Compare A with n. Same as SUB n, but don't save results in register A.
	Z 1 H C
*/
void Z80::cp(BIT8 X) {
	BIT16 i = registers.A - X;
	registers.F |= ((i & 0xFF) & 0xFF == 0) ? flags.Z : 0;				// Set Z
	registers.F |= flags.N;												// Set N(1)
	registers.F |= ((X & 0x0F) > (registers.A & 0x0F)) ? flags.H : 0;	// Set H
	registers.F |= (X > registers.A) ? flags.C : 0;						// Set C
	cpuClockCycles = 4;
}
void Z80::cphl() {
	cp(pMemory->readByte(getHL()));
	cpuClockCycles = 8;
}
void Z80::cpn() {
	cp(pMemory->readByte(registers.PC++));
	cpuClockCycles = 8;
}


/**
	INC nn
	Increment register nn
	- - - -
*/
void Z80::inc16(BIT8* X, BIT8* Y) {
	*Y = (*Y + 1) &0xFF;
	if (*Y==0)
		*X = (*X + 1) &0xFF;
	cpuClockCycles = 8;
}
void Z80::incsp() {
	registers.SP++;
	cpuClockCycles = 8;
}


/**
	DEC nn
	Decrement register nn
	- - - -
*/
void Z80::dec16(BIT8* X, BIT8* Y) {
	
	if(*Y == 0)
		*X = (*X - 1) & 0xFF;
	else
		*Y = (*Y - 1) & 0xFF;
	cpuClockCycles = 8;
}
void Z80::decsp() {
	registers.SP--;
	cpuClockCycles = 8;
}


/**
	Add HL,n
	Add n to HL
	- 0 H C
*/
void Z80::add16(BIT16 X) {
	BIT16 HL = getHL();
	BIT16 i = (HL + X) &0xFFFF;
	registers.F &= ~flags.N;												// Set N (0)
	registers.F |= ((HL & 0x0FFF) + (X & 0x0FFF) > 0x0FFF) ? flags.H : 0;	// Set H
	registers.F |= (i & 0xFFFF) ? flags.C : 0;								// Set C
	registers.H = (BIT8)(i >> 8);
	registers.L = (BIT8)(i & 0xFF);
	cpuClockCycles = 8;
}


/**
	ADD SP,n
	Add address n to SP
	0 0 H C
*/
void Z80::addspnn() {
	signed char i = pMemory->readByte(registers.PC++);
	BIT16 j = i + registers.SP;
	registers.F &= ~flags.Z;														// Set Z (0)
	registers.F &= ~flags.N;														// Set N (0)
	registers.F |= (((i &0x0F) + (registers.SP &0x0F)) &0x10) != 0 ? flags.H : 0;	// Set H
	registers.F |= (((i &0xFF) + (registers.SP &0xFF)) &0x100) != 0 ? flags.C : 0;	// Set C
	registers.SP = (j &0xFFFF);
}


/**
	RLC n
	Rotate n Left. Old bit 7 to Carry flag.
	Z 0 0 C
*/
void Z80::rlc(BIT8* X) {
	*X = (*X << 1) | (*X >> 7);
	registers.F |= (*X == 0) ? flags.Z : 0;				// Set Z
	registers.F &= ~flags.N;							// Set N(0)
	registers.F &= ~flags.H;							// Set H(0)
	registers.F |= ((*X & 0x01) == 1) ? flags.C : 0;	// Set C
	cpuClockCycles = 8;
}
void Z80::rlca() {
	// 0 0 0 C
	registers.F |= (registers.A & 0x01) ? flags.C : 0;	// Set C	Set before operation because we will lose the bit.
	registers.A = (registers.A << 1) | (registers.A >> 7);
	registers.F &= ~flags.Z;	// Set Z(0)
	registers.F &= ~flags.N;	// Set N(0)
	registers.F &= ~flags.H;	// Set H(0)
	cpuClockCycles = 4;
}
void Z80::rlchl() {
	BIT8 i = pMemory->readByte(getHL());
	registers.F |= ((i & 0x01) == 1) ? flags.C : 0;		// Set C	Set before operation, we are going to lose the bit.
	i = (i << 1) | (i >> 7);
	registers.F |= (i == 0) ? flags.Z : 0;				// Set Z
	registers.F &= ~flags.N;							// Set N(0)
	registers.F &= ~flags.H;							// Set H(0)
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/**
	RRC n
	Rotate n right. Old bit 0 to Carry Flag
	Z 0 0 C
*/
void Z80::rrc(BIT8* X) {
	*X = (*X >> 1) | (*X << 7);
	registers.F |= (*X == 0) ? flags.Z : 0;				// Set Z
	registers.F &= ~flags.N;							// Set N(0)
	registers.F &= ~flags.H;							// Set H(0)
	registers.F |= (*X & 0x80) ? flags.C : 0;			// Set C
	cpuClockCycles = 8;
}
void Z80::rrca() {
	// 0 0 0 C
	registers.F |= (registers.A & 0x01) ? flags.C : 0;	// Set C	Set before operation because we will lose the bit.
	registers.A = (registers.A >> 1) | (registers.A << 7);
	registers.F &= ~flags.Z;	// Set Z(0)
	registers.F &= ~flags.N;	// Set N(0)
	registers.F &= ~flags.H;	// Set H(0)
	cpuClockCycles = 4;
}
void Z80::rrchl() {
	BIT8 i = pMemory->readByte(getHL());
	registers.F |= ((i & 0x01) == 1) ? flags.C : 0;		// Set C
	i = (i >> 1) | (i << 7);
	registers.F |= (i == 0) ? flags.Z : 0;				// Set Z
	registers.F &= ~flags.N;							// Set N(0)
	registers.F &= ~flags.H;							// Set H(0)
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/**
	RL n
	Rotate n left through Carry Flag.
	Z 0 0 C
*/
void Z80::rl(BIT8* X) {
	registers.F |= (*X & 0x80) ? flags.C : 0;	// Set C (Setting before operation because we will lose the bit)
	*X = ((*X << 1) | ((registers.F &flags.C) ? 1 : 0)) & 0xFF;
	registers.F |= (*X == 0) ? flags.Z : 0;		// Set Z
	registers.F &= ~flags.N;					// Set N(0)
	registers.F &= ~flags.H;					// Set H(0)
	cpuClockCycles = 8;
}
void Z80::rla() {
	// 0 0 0 C
	registers.F |= (registers.A & 0x80) ? flags.C : 0;	// Set C (Setting before operation because we will lose the bit)
	registers.A = ((registers.A << 1) | ((registers.F & flags.C) ? 1 : 0)) &0xFF;
	registers.F &= ~flags.Z;	// Set Z(0)
	registers.F &= ~flags.N;	// Set N(0)
	registers.F &= ~flags.H;	// Set H(0)
	cpuClockCycles = 4;
}
void Z80::rlhl() {
	BIT8 i = pMemory->readByte(getHL());
	registers.F |= (i & 0x80) ? flags.C : 0;		// Set C (Setting before operation because we will lose the bit)
	i = ((i << 1) | ((registers.F & flags.C) ? 1 : 0)) &0xFF;
	registers.F |= (i == 0) ? flags.Z : 0;			// Set Z
	registers.F &= ~flags.N;						// Set N(0)
	registers.F &= ~flags.H;						// Set H(0)
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/*
	RR n
	Rotate n right through Carry Flag.
	Z 0 0 C
*/
void Z80::rr(BIT8* X) {
	registers.F |= (*X & 0x01) ? flags.C : 0;		// Set C (Setting before operation because we will lose the bit)
	*X = ((*X >> 1) | ((registers.F &flags.C) ? 0x80 : 0)) & 0xFF;
	registers.F |= (*X == 0) ? flags.Z : 0;			// Set Z
	registers.F &= ~flags.N;						// Set N(0)
	registers.F &= ~flags.H;						// Set H(0)
	cpuClockCycles = 8;
}
void Z80::rra() {
	// 0 0 0 C
	registers.F |= (registers.A & 0x01) ? flags.C : 0;		// Set C (Setting before operation because we will lose the bit)
	registers.A = ((registers.A >> 1) | ((registers.F & flags.C) ? 0x80 : 0)) &0xFF;
	registers.F &= ~flags.Z;								// Set Z
	registers.F &= ~flags.N;								// Set N(0)
	registers.F &= ~flags.H;								// Set H(0)
	cpuClockCycles = 4;
}
void Z80::rrhl() {
	BIT8 i = pMemory->readByte(getHL());
	registers.F |= (registers.A & 0x01) ? flags.C : 0;		// Set C (Setting before operation because we will lose the bit)
	i = ((i >> 1) | ((registers.F & flags.C) ? 0x80 : 0)) &0xFF;
	registers.F |= (i == 0) ? flags.Z : 0;					// Set Z
	registers.F &= ~flags.N;								// Set N(0)
	registers.F &= ~flags.H;								// Set H(0)
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/**
	SLA n
	Shift register n left into Carry. LSB of n set to 0.
	Z 0 0 C
*/
void Z80::sla(BIT8* X) {
	registers.F |= (*X & 0x80) ? flags.C : 0;		// Set C (Setting before operation because we will lose the bit)
	*X <<= 1;
	registers.F |= ((*X == 0) ? flags.Z : 0);		// Set Z
	registers.F &= ~flags.N;						// Set N
	registers.F &= ~flags.H;						// Set H
	cpuClockCycles = 8;
}
void Z80::slahl() {
	BIT8 i = pMemory->readByte(getHL());
	registers.F |= (i & 0x80) ? flags.C : 0;		// Set C (Setting before operation because we will lose the bit)
	i <<= 1;
	registers.F |= ((i == 0) ? flags.Z : 0);		// Set Z
	registers.F &= ~flags.N;						// Set N
	registers.F &= ~flags.H;						// Set H
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/**
	SRA n
	Shift register n right into Carry. MSB doesn't change.
	Z 0 0 0
*/
void Z80::sra(BIT8* X) {
	*X = (*X >> 1) | (*X & 0x80);
	registers.F |= (*X == 0) ? flags.Z : 0;	// Set Z
	registers.F &= ~flags.N;				// Set N(0)
	registers.F &= ~flags.H;				// Set H(0)
	registers.F &= ~flags.C;				// Set C(0)
	cpuClockCycles = 8;
}
void Z80::srahl() {
	BIT8 i = pMemory->readByte(getHL());
	i = ((i >> 1) | (i & 0x80));
	registers.F |= (i == 0) ? flags.Z : 0;	// Set Z
	registers.F &= ~flags.N;				// Set N(0)
	registers.F &= ~flags.H;				// Set H(0)
	registers.F &= ~flags.C;				// Set C(0)
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/**
	SWAP n
	Swap upper & lower bits of n
	Z 0 0 0
*/
void Z80::swap(BIT8* X) {
	*X = ((*X & 0xF0) >> 4) | ((*X & 0x0F) << 4);
	registers.F |= ((*X == 0) ? flags.Z : 0);	// Set Z
	registers.F &= ~flags.N;					// Set N(0)
	registers.F &= ~flags.H;					// Set H(0)
	registers.F &= ~flags.C;					// Set C(0)
	cpuClockCycles = 8;
}
void Z80::swaphl() {
	BIT8 X = pMemory->readByte(getHL());
	X = ((X & 0xF0) >> 4) | ((X & 0x0F) << 4);
	pMemory->writeByte(getHL(), X);
	registers.F |= ((X == 0) ? flags.Z : 0);	// Set Z
	registers.F &= ~flags.N;					// Set N(0)
	registers.F &= ~flags.H;					// Set H(0)
	registers.F &= ~flags.C;					// Set C(0)
	cpuClockCycles = 16;
}


/**
	SRL n
	Shift register n right into Carry. MSB set to 0.
	Z 0 0 C
*/
void Z80::srl(BIT8* X) {
	registers.F |= (*X & 0x01 ? flags.C : 0);	// Set C (Setting before operation because we will lose the bit)
	*X = (*X >> 1) &0x7F;
	registers.F |= (*X == 0) ? flags.Z : 0;		// Set Z
	registers.F &= ~flags.N;					// Set N(0)
	registers.F &= ~flags.H;					// Set H(0)
	cpuClockCycles = 8;
}
void Z80::srlhl() {
	BIT8 i = pMemory->readByte(getHL());
	registers.F |= (i & 0x01 ? flags.C : 0);	// Set C (Setting before operation because we will lose the bit)
	i = ((i >> 1) &0x7F);
	registers.F |= (i == 0) ? flags.Z : 0;		// Set Z
	registers.F &= ~flags.N;					// Set N(0)
	registers.F &= ~flags.H;					// Set H(0)
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/**
	BIT X,n
	Test bit n in register X
	Z 0 1 -
*/
void Z80::bit(BIT8 X, int n) {
	registers.F |= (X & (1 << n)) ? 0 : flags.Z;	// Set Z
	registers.F &= ~flags.N;						// Set N(0)
	registers.F |= flags.H;							// Set H(1)
	cpuClockCycles = 8;
}
void Z80::bithl(int n) {
	bit(pMemory->readByte(getHL()), n);
	cpuClockCycles = 16;
}


/**
	RES X,n
	Reset bit n in register X
	- - - -
*/
void Z80::res(BIT8* X, int n) {
	*X &= ~(1 << n);
	cpuClockCycles = 8;
}
void Z80::reshl(int n) {
	BIT8 i = pMemory->readByte(getHL());
	i &= ~(1 << n);
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}


/**
	SET X,n
	Set bit n in register X
	- - - -
*/
void Z80::set(BIT8* X, int n) {
	*X |= (1 << n);
	cpuClockCycles = 8;
}
void Z80::sethl(int n) {
	BIT8 i = pMemory->readByte(getHL());
	i |= (1 << n);
	pMemory->writeByte(getHL(), i);
	cpuClockCycles = 16;
}





//**********************************************************************************
//	INSTRUCTIONS
//**********************************************************************************
#pragma region InstructionSet
void Z80::i00() { nop(); }
void Z80::i01() { ld16(&registers.B, &registers.C); }
void Z80::i02() { ldw(registers.B, registers.C, registers.A); }
void Z80::i03() { inc16(&registers.B, &registers.C); }
void Z80::i04() { inc(&registers.B); }
void Z80::i05() { dec(&registers.B); }
void Z80::i06() { ldr(&registers.B); }
void Z80::i07() { rlca(); }
void Z80::i08() { ld16sp(); }
void Z80::i09() { add16(getBC()); }
void Z80::i0A() { ldr(&registers.A, registers.B, registers.C); }
void Z80::i0B() { dec16(&registers.B, &registers.C); }
void Z80::i0C() { inc(&registers.C); }
void Z80::i0D() { dec(&registers.C); }
void Z80::i0E() { ldr(&registers.C); }
void Z80::i0F() { rrca(); }

void Z80::i10() { stop(); }
void Z80::i11() { ld16(&registers.D, &registers.E); }
void Z80::i12() { ldw(registers.D, registers.E, registers.A); }
void Z80::i13() { inc16(&registers.D, &registers.E); }
void Z80::i14() { inc(&registers.D); }
void Z80::i15() { dec(&registers.D); }
void Z80::i16() { ldr(&registers.D); }
void Z80::i17() { rla(); }
void Z80::i18() { jrn(); }
void Z80::i19() { add16(getDE()); }
void Z80::i1A() { ldr(&registers.A, registers.D, registers.E); }
void Z80::i1B() { dec16(&registers.D, &registers.E); }
void Z80::i1C() { inc(&registers.E); }
void Z80::i1D() { dec(&registers.E); }
void Z80::i1E() { ldr(&registers.E); }
void Z80::i1F() { rra(); }

void Z80::i20() { jrnz(); }
void Z80::i21() { ld16(&registers.H, &registers.L); }
void Z80::i22() { ldhlpa(); }
void Z80::i23() { inc16(&registers.H, &registers.L); }
void Z80::i24() { inc(&registers.H); }
void Z80::i25() { dec(&registers.H); }
void Z80::i26() { ldr(&registers.H); }
void Z80::i27() { daa(); }
void Z80::i28() { jrz(); }
void Z80::i29() { add16(getHL()); }
void Z80::i2A() { ldahlp(); }
void Z80::i2B() { dec16(&registers.H, &registers.L); }
void Z80::i2C() { inc(&registers.L); }
void Z80::i2D() { dec(&registers.L); }
void Z80::i2E() { ldr(&registers.L); }
void Z80::i2F() { cpl(); }

void Z80::i30() { jrnc(); }
void Z80::i31() { ldsp16(); }
void Z80::i32() { ldhlma(); }
void Z80::i33() { incsp(); }
void Z80::i34() { inchl(); }
void Z80::i35() { dechl(); }
void Z80::i36() { ldhl(); }
void Z80::i37() { scf(); }
void Z80::i38() { jrc(); }
void Z80::i39() { add16(registers.SP); }
void Z80::i3A() { ldahlm(); }
void Z80::i3B() { decsp(); }
void Z80::i3C() { inc(&registers.A); }
void Z80::i3D() { dec(&registers.A); }
void Z80::i3E() { ldr(&registers.A); }
void Z80::i3F() { ccf(); }

void Z80::i40() { ld(&registers.B, registers.B); }
void Z80::i41() { ld(&registers.B, registers.C); }
void Z80::i42() { ld(&registers.B, registers.D); }
void Z80::i43() { ld(&registers.B, registers.E); }
void Z80::i44() { ld(&registers.B, registers.H); }
void Z80::i45() { ld(&registers.B, registers.L); }
void Z80::i46() { ldr(&registers.B, registers.H, registers.L); }
void Z80::i47() { ld(&registers.B, registers.A); }
void Z80::i48() { ld(&registers.C, registers.B); }
void Z80::i49() { ld(&registers.C, registers.C); }
void Z80::i4A() { ld(&registers.C, registers.D); }
void Z80::i4B() { ld(&registers.C, registers.E); }
void Z80::i4C() { ld(&registers.C, registers.H); }
void Z80::i4D() { ld(&registers.C, registers.L); }
void Z80::i4E() { ldr(&registers.C, registers.H, registers.L); }
void Z80::i4F() { ld(&registers.C, registers.A); }

void Z80::i50() { ld(&registers.D, registers.B); }
void Z80::i51() { ld(&registers.D, registers.C); }
void Z80::i52() { ld(&registers.D, registers.D); }
void Z80::i53() { ld(&registers.D, registers.E); }
void Z80::i54() { ld(&registers.D, registers.H); }
void Z80::i55() { ld(&registers.D, registers.L); }
void Z80::i56() { ldr(&registers.D, registers.H, registers.L); }
void Z80::i57() { ld(&registers.D, registers.A); }
void Z80::i58() { ld(&registers.E, registers.B); }
void Z80::i59() { ld(&registers.E, registers.C); }
void Z80::i5A() { ld(&registers.E, registers.D); }
void Z80::i5B() { ld(&registers.E, registers.E); }
void Z80::i5C() { ld(&registers.E, registers.H); }
void Z80::i5D() { ld(&registers.E, registers.L); }
void Z80::i5E() { ldr(&registers.E, registers.H, registers.L); }
void Z80::i5F() { ld(&registers.E, registers.A); }

void Z80::i60() { ld(&registers.H, registers.B); }
void Z80::i61() { ld(&registers.H, registers.C); }
void Z80::i62() { ld(&registers.H, registers.D); }
void Z80::i63() { ld(&registers.H, registers.E); }
void Z80::i64() { ld(&registers.H, registers.H); }
void Z80::i65() { ld(&registers.H, registers.L); }
void Z80::i66() { ldr(&registers.H, registers.H, registers.L); }
void Z80::i67() { ld(&registers.H, registers.A); }
void Z80::i68() { ld(&registers.L, registers.B); }
void Z80::i69() { ld(&registers.L, registers.C); }
void Z80::i6A() { ld(&registers.L, registers.D); }
void Z80::i6B() { ld(&registers.L, registers.E); }
void Z80::i6C() { ld(&registers.L, registers.H); }
void Z80::i6D() { ld(&registers.L, registers.L); }
void Z80::i6E() { ldr(&registers.L, registers.H, registers.L); }
void Z80::i6F() { ld(&registers.L, registers.A); }

void Z80::i70() { ldw(registers.H, registers.L, registers.B); }
void Z80::i71() { ldw(registers.H, registers.L, registers.C); }
void Z80::i72() { ldw(registers.H, registers.L, registers.D); }
void Z80::i73() { ldw(registers.H, registers.L, registers.E); }
void Z80::i74() { ldw(registers.H, registers.L, registers.H); }
void Z80::i75() { ldw(registers.H, registers.L, registers.L); }
void Z80::i76() { halt(); }
void Z80::i77() { ldw(registers.H, registers.L, registers.A); }
void Z80::i78() { ld(&registers.A, registers.B); }
void Z80::i79() { ld(&registers.A, registers.C); }
void Z80::i7A() { ld(&registers.A, registers.D); }
void Z80::i7B() { ld(&registers.A, registers.E); }
void Z80::i7C() { ld(&registers.A, registers.H); }
void Z80::i7D() { ld(&registers.A, registers.L); }
void Z80::i7E() { ldr(&registers.A, registers.H, registers.L); }
void Z80::i7F() { ld(&registers.A, registers.A); }

void Z80::i80() { add(registers.B); }
void Z80::i81() { add(registers.C); }
void Z80::i82() { add(registers.D); }
void Z80::i83() { add(registers.E); }
void Z80::i84() { add(registers.H); }
void Z80::i85() { add(registers.L); }
void Z80::i86() { addhl(); }
void Z80::i87() { add(registers.A); }
void Z80::i88() { adc(registers.B); }
void Z80::i89() { adc(registers.C); }
void Z80::i8A() { adc(registers.D); }
void Z80::i8B() { adc(registers.E); }
void Z80::i8C() { adc(registers.H); }
void Z80::i8D() { adc(registers.L); }
void Z80::i8E() { adchl(); }
void Z80::i8F() { adc(registers.A); }

void Z80::i90() { sub(registers.B); }
void Z80::i91() { sub(registers.C); }
void Z80::i92() { sub(registers.D); }
void Z80::i93() { sub(registers.E); }
void Z80::i94() { sub(registers.H); }
void Z80::i95() { sub(registers.L); }
void Z80::i96() { subhl(); }
void Z80::i97() { sub(registers.A); }
void Z80::i98() { sbc(registers.B); }
void Z80::i99() { sbc(registers.C); }
void Z80::i9A() { sbc(registers.D); }
void Z80::i9B() { sbc(registers.E); }
void Z80::i9C() { sbc(registers.H); }
void Z80::i9D() { sbc(registers.L); }
void Z80::i9E() { sbchl(); }
void Z80::i9F() { sbc(registers.A); }

void Z80::iA0() { _and(registers.B); }
void Z80::iA1() { _and(registers.C); }
void Z80::iA2() { _and(registers.D); }
void Z80::iA3() { _and(registers.E); }
void Z80::iA4() { _and(registers.H); }
void Z80::iA5() { _and(registers.L); }
void Z80::iA6() { andhl(); }
void Z80::iA7() { _and(registers.A); }
void Z80::iA8() { _xor(registers.B); }
void Z80::iA9() { _xor(registers.C); }
void Z80::iAA() { _xor(registers.D); }
void Z80::iAB() { _xor(registers.E); }
void Z80::iAC() { _xor(registers.H); }
void Z80::iAD() { _xor(registers.L); }
void Z80::iAE() { xorhl(); }
void Z80::iAF() { _xor(registers.A); }

void Z80::iB0() { _or(registers.B); }
void Z80::iB1() { _or(registers.C); }
void Z80::iB2() { _or(registers.D); }
void Z80::iB3() { _or(registers.E); }
void Z80::iB4() { _or(registers.H); }
void Z80::iB5() { _or(registers.L); }
void Z80::iB6() { orhl(); }
void Z80::iB7() { _or(registers.A); }
void Z80::iB8() { cp(registers.B); }
void Z80::iB9() { cp(registers.C); }
void Z80::iBA() { cp(registers.D); }
void Z80::iBB() { cp(registers.E); }
void Z80::iBC() { cp(registers.H); }
void Z80::iBD() { cp(registers.L); }
void Z80::iBE() { cphl(); }
void Z80::iBF() { cp(registers.A); }

void Z80::iC0() { retnz(); }
void Z80::iC1() { pop(&registers.B, &registers.C); }
void Z80::iC2() { jpnz(); }
void Z80::iC3() { jpn(); }
void Z80::iC4() { callnz(); }
void Z80::iC5() { push(registers.B, registers.C); }
void Z80::iC6() { addn(); }
void Z80::iC7() { rst((BIT8)(0x00)); }
void Z80::iC8() { retz(); }
void Z80::iC9() { ret(); }
void Z80::iCA() { jpz(); }
void Z80::iCB() { runCB(); }
void Z80::iCC() { callz(); }
void Z80::iCD() { calln(); }
void Z80::iCE() { adcn(); }
void Z80::iCF() { rst((BIT8)(0x08)); }

void Z80::iD0() { retnc(); }
void Z80::iD1() { pop(&registers.D, &registers.E); }
void Z80::iD2() { jpnc(); }
void Z80::iD3() { xx(); }
void Z80::iD4() { callnc(); }
void Z80::iD5() { push(registers.D, registers.E); }
void Z80::iD6() { subn(); }
void Z80::iD7() { rst((BIT8)(0x10)); }
void Z80::iD8() { retc(); }
void Z80::iD9() { reti(); }
void Z80::iDA() { jpc(); }
void Z80::iDB() { xx(); }
void Z80::iDC() { callc(); }
void Z80::iDD() { xx(); }
void Z80::iDE() { sbcn(); }
void Z80::iDF() { rst((BIT8)(0x18)); }

void Z80::iE0() { ldhw(); }
void Z80::iE1() { pop(&registers.H, &registers.L); }
void Z80::iE2() { ldw((BIT8)(0xFF), registers.C, registers.A); }
void Z80::iE3() { xx(); }
void Z80::iE4() { xx(); }
void Z80::iE5() { push(registers.H, registers.L); }
void Z80::iE6() { andn(); }
void Z80::iE7() { rst((BIT8)(0x20)); }
void Z80::iE8() { addspnn(); }
void Z80::iE9() { jphl(); }
void Z80::iEA() { ldna(); }
void Z80::iEB() { xx(); }
void Z80::iEC() { xx(); }
void Z80::iED() { xx(); }
void Z80::iEE() { xorn(); }
void Z80::iEF() { rst((BIT8)(0x28)); }

void Z80::iF0() { ldhr(); }
void Z80::iF1() { pop(&registers.A, &registers.F); }
void Z80::iF2() { ldr(&registers.A, (BIT8)(0xFF), registers.C); }
void Z80::iF3() { diei(false); }
void Z80::iF4() { xx(); }
void Z80::iF5() { push(registers.A, registers.F); }
void Z80::iF6() { orn(); }
void Z80::iF7() { rst((BIT8)(0x30)); }
void Z80::iF8() { ldhlspn(); }
void Z80::iF9() { ldsphl(); }
void Z80::iFA() { ldan(); }
void Z80::iFB() { diei(true); }
void Z80::iFC() { xx(); }
void Z80::iFD() { xx(); }
void Z80::iFE() { cpn(); }
void Z80::iFF() { rst((BIT8)(0x38)); }
#pragma endregion




//**********************************************************************************
//	PREFIX CB INSTRUCTIONS
//**********************************************************************************
#pragma region CBInstructionSet
void Z80::iCB00() { rlc(&registers.B); }
void Z80::iCB01() { rlc(&registers.C); }
void Z80::iCB02() { rlc(&registers.D); }
void Z80::iCB03() { rlc(&registers.E); }
void Z80::iCB04() { rlc(&registers.H); }
void Z80::iCB05() { rlc(&registers.L); }
void Z80::iCB06() { rlchl(); }
void Z80::iCB07() { rlc(&registers.A); }
void Z80::iCB08() { rrc(&registers.B); }
void Z80::iCB09() { rrc(&registers.C); }
void Z80::iCB0A() { rrc(&registers.D); }
void Z80::iCB0B() { rrc(&registers.E); }
void Z80::iCB0C() { rrc(&registers.H); }
void Z80::iCB0D() { rrc(&registers.L); }
void Z80::iCB0E() { rrchl(); }
void Z80::iCB0F() { rrc(&registers.A); }

void Z80::iCB10() { rl(&registers.B); }
void Z80::iCB11() { rl(&registers.C); }
void Z80::iCB12() { rl(&registers.D); }
void Z80::iCB13() { rl(&registers.E); }
void Z80::iCB14() { rl(&registers.H); }
void Z80::iCB15() { rl(&registers.L); }
void Z80::iCB16() { rlhl(); }
void Z80::iCB17() { rl(&registers.A); }
void Z80::iCB18() { rr(&registers.B); }
void Z80::iCB19() { rr(&registers.C); }
void Z80::iCB1A() { rr(&registers.D); }
void Z80::iCB1B() { rr(&registers.E); }
void Z80::iCB1C() { rr(&registers.H); }
void Z80::iCB1D() { rr(&registers.L); }
void Z80::iCB1E() { rrhl(); }
void Z80::iCB1F() { rr(&registers.A); }

void Z80::iCB20() { sla(&registers.B); }
void Z80::iCB21() { sla(&registers.C); }
void Z80::iCB22() { sla(&registers.D); }
void Z80::iCB23() { sla(&registers.E); }
void Z80::iCB24() { sla(&registers.H); }
void Z80::iCB25() { sla(&registers.L); }
void Z80::iCB26() { slahl(); }
void Z80::iCB27() { sla(&registers.A); }
void Z80::iCB28() { sra(&registers.B); }
void Z80::iCB29() { sra(&registers.C); }
void Z80::iCB2A() { sra(&registers.D); }
void Z80::iCB2B() { sra(&registers.E); }
void Z80::iCB2C() { sra(&registers.H); }
void Z80::iCB2D() { sra(&registers.L); }
void Z80::iCB2E() { srahl(); }
void Z80::iCB2F() { sra(&registers.A); }

void Z80::iCB30() { swap(&registers.B); }
void Z80::iCB31() { swap(&registers.C); }
void Z80::iCB32() { swap(&registers.D); }
void Z80::iCB33() { swap(&registers.E); }
void Z80::iCB34() { swap(&registers.H); }
void Z80::iCB35() { swap(&registers.L); }
void Z80::iCB36() { swaphl(); }
void Z80::iCB37() { swap(&registers.A); }
void Z80::iCB38() { srl(&registers.B); }
void Z80::iCB39() { srl(&registers.C); }
void Z80::iCB3A() { srl(&registers.D); }
void Z80::iCB3B() { srl(&registers.E); }
void Z80::iCB3C() { srl(&registers.H); }
void Z80::iCB3D() { srl(&registers.L); }
void Z80::iCB3E() { srlhl(); }
void Z80::iCB3F() { srl(&registers.A); }

void Z80::iCB40() { bit(registers.B, 0); }
void Z80::iCB41() { bit(registers.C, 0); }
void Z80::iCB42() { bit(registers.D, 0); }
void Z80::iCB43() { bit(registers.E, 0); }
void Z80::iCB44() { bit(registers.H, 0); }
void Z80::iCB45() { bit(registers.L, 0); }
void Z80::iCB46() { bithl(0); }
void Z80::iCB47() { bit(registers.A, 0); }
void Z80::iCB48() { bit(registers.B, 1); }
void Z80::iCB49() { bit(registers.C, 1); }
void Z80::iCB4A() { bit(registers.D, 1); }
void Z80::iCB4B() { bit(registers.E, 1); }
void Z80::iCB4C() { bit(registers.H, 1); }
void Z80::iCB4D() { bit(registers.L, 1); }
void Z80::iCB4E() { bithl(1); }
void Z80::iCB4F() { bit(registers.A, 1); }

void Z80::iCB50() { bit(registers.B, 2); }
void Z80::iCB51() { bit(registers.C, 2); }
void Z80::iCB52() { bit(registers.D, 2); }
void Z80::iCB53() { bit(registers.E, 2); }
void Z80::iCB54() { bit(registers.H, 2); }
void Z80::iCB55() { bit(registers.L, 2); }
void Z80::iCB56() { bithl(2); }
void Z80::iCB57() { bit(registers.A, 2); }
void Z80::iCB58() { bit(registers.B, 3); }
void Z80::iCB59() { bit(registers.C, 3); }
void Z80::iCB5A() { bit(registers.D, 3); }
void Z80::iCB5B() { bit(registers.E, 3); }
void Z80::iCB5C() { bit(registers.H, 3); }
void Z80::iCB5D() { bit(registers.L, 3); }
void Z80::iCB5E() { bithl(3); }
void Z80::iCB5F() { bit(registers.A, 3); }

void Z80::iCB60() { bit(registers.B, 4); }
void Z80::iCB61() { bit(registers.C, 4); }
void Z80::iCB62() { bit(registers.D, 4); }
void Z80::iCB63() { bit(registers.E, 4); }
void Z80::iCB64() { bit(registers.H, 4); }
void Z80::iCB65() { bit(registers.L, 4); }
void Z80::iCB66() { bithl(4); }
void Z80::iCB67() { bit(registers.A, 4); }
void Z80::iCB68() { bit(registers.B, 5); }
void Z80::iCB69() { bit(registers.C, 5); }
void Z80::iCB6A() { bit(registers.D, 5); }
void Z80::iCB6B() { bit(registers.E, 5); }
void Z80::iCB6C() { bit(registers.H, 5); }
void Z80::iCB6D() { bit(registers.L, 5); }
void Z80::iCB6E() { bithl(5); }
void Z80::iCB6F() { bit(registers.A, 5); }

void Z80::iCB70() { bit(registers.B, 6); }
void Z80::iCB71() { bit(registers.C, 6); }
void Z80::iCB72() { bit(registers.D, 6); }
void Z80::iCB73() { bit(registers.E, 6); }
void Z80::iCB74() { bit(registers.H, 6); }
void Z80::iCB75() { bit(registers.L, 6); }
void Z80::iCB76() { bithl(6); }
void Z80::iCB77() { bit(registers.A, 6); }
void Z80::iCB78() { bit(registers.B, 7); }
void Z80::iCB79() { bit(registers.C, 7); }
void Z80::iCB7A() { bit(registers.D, 7); }
void Z80::iCB7B() { bit(registers.E, 7); }
void Z80::iCB7C() { bit(registers.H, 7); }
void Z80::iCB7D() { bit(registers.L, 7); }
void Z80::iCB7E() { bithl(7); }
void Z80::iCB7F() { bit(registers.A, 7); }

void Z80::iCB80() { res(&registers.B, 0); }
void Z80::iCB81() { res(&registers.C, 0); }
void Z80::iCB82() { res(&registers.D, 0); }
void Z80::iCB83() { res(&registers.E, 0); }
void Z80::iCB84() { res(&registers.H, 0); }
void Z80::iCB85() { res(&registers.L, 0); }
void Z80::iCB86() { reshl(0); }
void Z80::iCB87() { res(&registers.A, 0); }
void Z80::iCB88() { res(&registers.B, 1); }
void Z80::iCB89() { res(&registers.C, 1); }
void Z80::iCB8A() { res(&registers.D, 1); }
void Z80::iCB8B() { res(&registers.E, 1); }
void Z80::iCB8C() { res(&registers.H, 1); }
void Z80::iCB8D() { res(&registers.L, 1); }
void Z80::iCB8E() { reshl(1); }
void Z80::iCB8F() { res(&registers.A, 1); }

void Z80::iCB90() { res(&registers.B, 2); }
void Z80::iCB91() { res(&registers.C, 2); }
void Z80::iCB92() { res(&registers.D, 2); }
void Z80::iCB93() { res(&registers.E, 2); }
void Z80::iCB94() { res(&registers.H, 2); }
void Z80::iCB95() { res(&registers.L, 2); }
void Z80::iCB96() { reshl(2); }
void Z80::iCB97() { res(&registers.A, 2); }
void Z80::iCB98() { res(&registers.B, 3); }
void Z80::iCB99() { res(&registers.C, 3); }
void Z80::iCB9A() { res(&registers.D, 3); }
void Z80::iCB9B() { res(&registers.E, 3); }
void Z80::iCB9C() { res(&registers.H, 3); }
void Z80::iCB9D() { res(&registers.L, 3); }
void Z80::iCB9E() { reshl(3); }
void Z80::iCB9F() { res(&registers.A, 3); }

void Z80::iCBA0() { res(&registers.B, 4); }
void Z80::iCBA1() { res(&registers.C, 4); }
void Z80::iCBA2() { res(&registers.D, 4); }
void Z80::iCBA3() { res(&registers.E, 4); }
void Z80::iCBA4() { res(&registers.H, 4); }
void Z80::iCBA5() { res(&registers.L, 4); }
void Z80::iCBA6() { reshl(4); }
void Z80::iCBA7() { res(&registers.A, 4); }
void Z80::iCBA8() { res(&registers.B, 5); }
void Z80::iCBA9() { res(&registers.C, 5); }
void Z80::iCBAA() { res(&registers.D, 5); }
void Z80::iCBAB() { res(&registers.E, 5); }
void Z80::iCBAC() { res(&registers.H, 5); }
void Z80::iCBAD() { res(&registers.L, 5); }
void Z80::iCBAE() { reshl(5); }
void Z80::iCBAF() { res(&registers.A, 5); }

void Z80::iCBB0() { res(&registers.B, 6); }
void Z80::iCBB1() { res(&registers.C, 6); }
void Z80::iCBB2() { res(&registers.D, 6); }
void Z80::iCBB3() { res(&registers.E, 6); }
void Z80::iCBB4() { res(&registers.H, 6); }
void Z80::iCBB5() { res(&registers.L, 6); }
void Z80::iCBB6() { reshl(6); }
void Z80::iCBB7() { res(&registers.A, 6); }
void Z80::iCBB8() { res(&registers.B, 7); }
void Z80::iCBB9() { res(&registers.C, 7); }
void Z80::iCBBA() { res(&registers.D, 7); }
void Z80::iCBBB() { res(&registers.E, 7); }
void Z80::iCBBC() { res(&registers.H, 7); }
void Z80::iCBBD() { res(&registers.L, 7); }
void Z80::iCBBE() { reshl(7); }
void Z80::iCBBF() { res(&registers.A, 7); }

void Z80::iCBC0() { set(&registers.B, 0); }
void Z80::iCBC1() { set(&registers.C, 0); }
void Z80::iCBC2() { set(&registers.D, 0); }
void Z80::iCBC3() { set(&registers.E, 0); }
void Z80::iCBC4() { set(&registers.H, 0); }
void Z80::iCBC5() { set(&registers.L, 0); }
void Z80::iCBC6() { sethl(0); }
void Z80::iCBC7() { set(&registers.A, 0); }
void Z80::iCBC8() { set(&registers.B, 1); }
void Z80::iCBC9() { set(&registers.C, 1); }
void Z80::iCBCA() { set(&registers.D, 1); }
void Z80::iCBCB() { set(&registers.E, 1); }
void Z80::iCBCC() { set(&registers.H, 1); }
void Z80::iCBCD() { set(&registers.L, 1); }
void Z80::iCBCE() { sethl(1); }
void Z80::iCBCF() { set(&registers.A, 1); }

void Z80::iCBD0() { set(&registers.B, 2); }
void Z80::iCBD1() { set(&registers.C, 2); }
void Z80::iCBD2() { set(&registers.D, 2); }
void Z80::iCBD3() { set(&registers.E, 2); }
void Z80::iCBD4() { set(&registers.H, 2); }
void Z80::iCBD5() { set(&registers.L, 2); }
void Z80::iCBD6() { sethl(2); }
void Z80::iCBD7() { set(&registers.A, 2); }
void Z80::iCBD8() { set(&registers.B, 3); }
void Z80::iCBD9() { set(&registers.C, 3); }
void Z80::iCBDA() { set(&registers.D, 3); }
void Z80::iCBDB() { set(&registers.E, 3); }
void Z80::iCBDC() { set(&registers.H, 3); }
void Z80::iCBDD() { set(&registers.L, 3); }
void Z80::iCBDE() { sethl(3); }
void Z80::iCBDF() { set(&registers.A, 3); }

void Z80::iCBE0() { set(&registers.B, 4); }
void Z80::iCBE1() { set(&registers.C, 4); }
void Z80::iCBE2() { set(&registers.D, 4); }
void Z80::iCBE3() { set(&registers.E, 4); }
void Z80::iCBE4() { set(&registers.H, 4); }
void Z80::iCBE5() { set(&registers.L, 4); }
void Z80::iCBE6() { sethl(4); }
void Z80::iCBE7() { set(&registers.A, 4); }
void Z80::iCBE8() { set(&registers.B, 5); }
void Z80::iCBE9() { set(&registers.C, 5); }
void Z80::iCBEA() { set(&registers.D, 5); }
void Z80::iCBEB() { set(&registers.E, 5); }
void Z80::iCBEC() { set(&registers.H, 5); }
void Z80::iCBED() { set(&registers.L, 5); }
void Z80::iCBEE() { sethl(5); }
void Z80::iCBEF() { set(&registers.A, 5); }

void Z80::iCBF0() { set(&registers.B, 6); }
void Z80::iCBF1() { set(&registers.C, 6); }
void Z80::iCBF2() { set(&registers.D, 6); }
void Z80::iCBF3() { set(&registers.E, 6); }
void Z80::iCBF4() { set(&registers.H, 6); }
void Z80::iCBF5() { set(&registers.L, 6); }
void Z80::iCBF6() { sethl(6); }
void Z80::iCBF7() { set(&registers.A, 6); }
void Z80::iCBF8() { set(&registers.B, 7); }
void Z80::iCBF9() { set(&registers.C, 7); }
void Z80::iCBFA() { set(&registers.D, 7); }
void Z80::iCBFB() { set(&registers.E, 7); }
void Z80::iCBFC() { set(&registers.H, 7); }
void Z80::iCBFD() { set(&registers.L, 7); }
void Z80::iCBFE() { sethl(7); }
void Z80::iCBFF() { set(&registers.A, 7); }
#pragma endregion
