#pragma once
#include "Globals.h"
#include "Memory.h"


class Z80
{
public:
	// Constructor & Destructor
	Z80(Memory* memory);
	~Z80();

	void execute();
	int getCpuClockCycles();
	void setProgramCounter(BIT16 val);
	void putPCOnStack();

	void printRegisters();

	// Interrupt Master Enable
	void setIme(bool interrupt);
	bool getIme();

	/*
		void setHalt(bool halt);
		bool isHalted();
	*/

private:
	typedef void (Z80::*Function)(void);

	Memory* pMemory;					// Pointer to Memory class so we can read/write it.
	Function* instructionSet;			// Array of function pointers for instruction set
	Function* cbInstructionSet;			// Array of function pointers for CB prefixed instructions 
	int cpuClockCycles;					// Current number of clock cycles until next instruction can be run
	bool bIME;							// Interrupt Master Enable.
	bool bHalt;							// Is emulator currently halted?
	bool bStop;							// Is emulator currently stopped?

	// All of the Gameboy Register Values
	struct {
		BIT8 A;		// Accumulator
		BIT8 F;		// Flags
		BIT8 B;		// BC 15.8
		BIT8 C;		// BC 7..0
		BIT8 D;		// DE 15.8
		BIT8 E;		// DE 7..0
		BIT8 H;		// HL 15.8
		BIT8 L;		// HL 7..0
		BIT16 PC;	// Program Counter
		BIT16 SP;	// Stack Pointer
	} registers;

	// Get BIT16 Register Values from the BIT8 versions.
	BIT16 getHL();
	BIT16 getAF();
	BIT16 getBC();
	BIT16 getDE();

	// Define Flags to use
	const struct {
		BIT8 Z = 0x80;
		BIT8 N = 0x40;
		BIT8 H = 0x20;
		BIT8 C = 0x10;
	} flags;

	// Misc
	void xx();
	void nop();
	void stop();
	void halt();
	void runCB();
	void diei(bool interrupt);

	// Jump/Call
	void jrn();
	void jrnz();
	void jrz();
	void jrnc();
	void jrc();
	void rst(BIT8 X);
	void calln();
	void callnz();
	void callz();
	void callnc();
	void callc();
	void jpn();
	void jpnz();
	void jpz();
	void jpnc();
	void jpc();
	void jphl();
	void ret();
	void retnz();
	void retz();
	void retnc();
	void retc();
	void reti();

	// 8bit Load
	void ldw(BIT8 X, BIT8 Y, BIT8 Z);
	void ldhl();
	void ldr(BIT8* X, BIT8 Y, BIT8 Z);
	void ldr(BIT8* X);
	void ld(BIT8* X, BIT8 Y);
	void ldhw();
	void ldhr();
	void ldna();
	void ldan();
	void ldhlpa();
	void ldahlp();
	void ldhlma();
	void ldahlm();

	// 16bit Load
	void ld16(BIT8* X, BIT8* Y);
	void ldsp16();
	void ld16sp();
	void pop(BIT8* X, BIT8* Y);
	void push(BIT8 X, BIT8 Y);
	void ldhlspn();
	void ldsphl();

	// 8bit Arithmetic
	void inc(BIT8* X);
	void inchl();
	void dec(BIT8* X);
	void dechl();
	void daa();
	void cpl();
	void scf();
	void ccf();
	void add(BIT8 X);
	void addhl();
	void addn();
	void adc(BIT8 X);
	void adchl();
	void adcn();
	void sub(BIT8 X);
	void subhl();
	void subn();
	void sbc(BIT8 X);
	void sbchl();
	void sbcn();
	void _and(BIT8 X);
	void andhl();
	void andn();
	void _xor(BIT8 X);
	void xorhl();
	void xorn();
	void _or(BIT8 X);
	void orhl();
	void orn();
	void cp(BIT8 X);
	void cphl();
	void cpn();

	// 16bit arithmetic
	void inc16(BIT8* X, BIT8* Y);
	void incsp();
	void dec16(BIT8* X, BIT8* Y);
	void decsp();
	void add16(BIT16 X);
	void addspnn();

	// 8bit rotation/shifts
	void rlc(BIT8* X);
	void rlca();
	void rlchl();
	void rrc(BIT8* X);
	void rrca();
	void rrchl();
	void rl(BIT8* X);
	void rla();
	void rlhl();
	void rr(BIT8* X);
	void rra();
	void rrhl();
	void sla(BIT8* X);
	void slahl();
	void sra(BIT8* X);
	void srahl();
	void swap(BIT8* X);
	void swaphl();
	void srl(BIT8* X);
	void srlhl();
	void bit(BIT8 X, int n);
	void bithl(int n);
	void res(BIT8* X, int n);
	void reshl(int n);
	void set(BIT8* X, int n);
	void sethl(int n);

	// Instruction Set Methods
	void i00(), i01(), i02(), i03(), i04(), i05(), i06(), i07(), i08(), i09(), i0A(), i0B(), i0C(), i0D(), i0E(), i0F();
	void i10(), i11(), i12(), i13(), i14(), i15(), i16(), i17(), i18(), i19(), i1A(), i1B(), i1C(), i1D(), i1E(), i1F();
	void i20(), i21(), i22(), i23(), i24(), i25(), i26(), i27(), i28(), i29(), i2A(), i2B(), i2C(), i2D(), i2E(), i2F();
	void i30(), i31(), i32(), i33(), i34(), i35(), i36(), i37(), i38(), i39(), i3A(), i3B(), i3C(), i3D(), i3E(), i3F();
	void i40(), i41(), i42(), i43(), i44(), i45(), i46(), i47(), i48(), i49(), i4A(), i4B(), i4C(), i4D(), i4E(), i4F();
	void i50(), i51(), i52(), i53(), i54(), i55(), i56(), i57(), i58(), i59(), i5A(), i5B(), i5C(), i5D(), i5E(), i5F();
	void i60(), i61(), i62(), i63(), i64(), i65(), i66(), i67(), i68(), i69(), i6A(), i6B(), i6C(), i6D(), i6E(), i6F();
	void i70(), i71(), i72(), i73(), i74(), i75(), i76(), i77(), i78(), i79(), i7A(), i7B(), i7C(), i7D(), i7E(), i7F();
	void i80(), i81(), i82(), i83(), i84(), i85(), i86(), i87(), i88(), i89(), i8A(), i8B(), i8C(), i8D(), i8E(), i8F();
	void i90(), i91(), i92(), i93(), i94(), i95(), i96(), i97(), i98(), i99(), i9A(), i9B(), i9C(), i9D(), i9E(), i9F();
	void iA0(), iA1(), iA2(), iA3(), iA4(), iA5(), iA6(), iA7(), iA8(), iA9(), iAA(), iAB(), iAC(), iAD(), iAE(), iAF();
	void iB0(), iB1(), iB2(), iB3(), iB4(), iB5(), iB6(), iB7(), iB8(), iB9(), iBA(), iBB(), iBC(), iBD(), iBE(), iBF();
	void iC0(), iC1(), iC2(), iC3(), iC4(), iC5(), iC6(), iC7(), iC8(), iC9(), iCA(), iCB(), iCC(), iCD(), iCE(), iCF();
	void iD0(), iD1(), iD2(), iD3(), iD4(), iD5(), iD6(), iD7(), iD8(), iD9(), iDA(), iDB(), iDC(), iDD(), iDE(), iDF();
	void iE0(), iE1(), iE2(), iE3(), iE4(), iE5(), iE6(), iE7(), iE8(), iE9(), iEA(), iEB(), iEC(), iED(), iEE(), iEF();
	void iF0(), iF1(), iF2(), iF3(), iF4(), iF5(), iF6(), iF7(), iF8(), iF9(), iFA(), iFB(), iFC(), iFD(), iFE(), iFF();

	// CB Methods
	void iCB00(), iCB01(), iCB02(), iCB03(), iCB04(), iCB05(), iCB06(), iCB07(), iCB08(), iCB09(), iCB0A(), iCB0B(), iCB0C(), iCB0D(), iCB0E(), iCB0F();
	void iCB10(), iCB11(), iCB12(), iCB13(), iCB14(), iCB15(), iCB16(), iCB17(), iCB18(), iCB19(), iCB1A(), iCB1B(), iCB1C(), iCB1D(), iCB1E(), iCB1F();
	void iCB20(), iCB21(), iCB22(), iCB23(), iCB24(), iCB25(), iCB26(), iCB27(), iCB28(), iCB29(), iCB2A(), iCB2B(), iCB2C(), iCB2D(), iCB2E(), iCB2F();
	void iCB30(), iCB31(), iCB32(), iCB33(), iCB34(), iCB35(), iCB36(), iCB37(), iCB38(), iCB39(), iCB3A(), iCB3B(), iCB3C(), iCB3D(), iCB3E(), iCB3F();
	void iCB40(), iCB41(), iCB42(), iCB43(), iCB44(), iCB45(), iCB46(), iCB47(), iCB48(), iCB49(), iCB4A(), iCB4B(), iCB4C(), iCB4D(), iCB4E(), iCB4F();
	void iCB50(), iCB51(), iCB52(), iCB53(), iCB54(), iCB55(), iCB56(), iCB57(), iCB58(), iCB59(), iCB5A(), iCB5B(), iCB5C(), iCB5D(), iCB5E(), iCB5F();
	void iCB60(), iCB61(), iCB62(), iCB63(), iCB64(), iCB65(), iCB66(), iCB67(), iCB68(), iCB69(), iCB6A(), iCB6B(), iCB6C(), iCB6D(), iCB6E(), iCB6F();
	void iCB70(), iCB71(), iCB72(), iCB73(), iCB74(), iCB75(), iCB76(), iCB77(), iCB78(), iCB79(), iCB7A(), iCB7B(), iCB7C(), iCB7D(), iCB7E(), iCB7F();
	void iCB80(), iCB81(), iCB82(), iCB83(), iCB84(), iCB85(), iCB86(), iCB87(), iCB88(), iCB89(), iCB8A(), iCB8B(), iCB8C(), iCB8D(), iCB8E(), iCB8F();
	void iCB90(), iCB91(), iCB92(), iCB93(), iCB94(), iCB95(), iCB96(), iCB97(), iCB98(), iCB99(), iCB9A(), iCB9B(), iCB9C(), iCB9D(), iCB9E(), iCB9F();
	void iCBA0(), iCBA1(), iCBA2(), iCBA3(), iCBA4(), iCBA5(), iCBA6(), iCBA7(), iCBA8(), iCBA9(), iCBAA(), iCBAB(), iCBAC(), iCBAD(), iCBAE(), iCBAF();
	void iCBB0(), iCBB1(), iCBB2(), iCBB3(), iCBB4(), iCBB5(), iCBB6(), iCBB7(), iCBB8(), iCBB9(), iCBBA(), iCBBB(), iCBBC(), iCBBD(), iCBBE(), iCBBF();
	void iCBC0(), iCBC1(), iCBC2(), iCBC3(), iCBC4(), iCBC5(), iCBC6(), iCBC7(), iCBC8(), iCBC9(), iCBCA(), iCBCB(), iCBCC(), iCBCD(), iCBCE(), iCBCF();
	void iCBD0(), iCBD1(), iCBD2(), iCBD3(), iCBD4(), iCBD5(), iCBD6(), iCBD7(), iCBD8(), iCBD9(), iCBDA(), iCBDB(), iCBDC(), iCBDD(), iCBDE(), iCBDF();
	void iCBE0(), iCBE1(), iCBE2(), iCBE3(), iCBE4(), iCBE5(), iCBE6(), iCBE7(), iCBE8(), iCBE9(), iCBEA(), iCBEB(), iCBEC(), iCBED(), iCBEE(), iCBEF();
	void iCBF0(), iCBF1(), iCBF2(), iCBF3(), iCBF4(), iCBF5(), iCBF6(), iCBF7(), iCBF8(), iCBF9(), iCBFA(), iCBFB(), iCBFC(), iCBFD(), iCBFE(), iCBFF();
};