#include "Memory.h"
#include <iostream>
#include <sstream>

// Constructor
Memory::Memory(){}


// Destructor
Memory::~Memory() {
	delete[] BOOT_ROM;
	delete[] ROM;
}


// Set initial registers
void Memory::setInitRegisters() {
	ROM[0xFF05] = 0x00;	// TIMA
	ROM[0xFF06] = 0x00;	// TMA
	ROM[0xFF07] = 0x00;	// TAC
	ROM[0xFF10] = 0x80;	// NR10
	ROM[0xFF11] = 0xBF;	// NR11
	ROM[0xFF12] = 0xF3;	// NR12
	ROM[0xFF14] = 0xBF;	// NR14
	ROM[0xFF16] = 0x3F;	// NR21
	ROM[0xFF17] = 0x00;	// NR22
	ROM[0xFF19] = 0xBF;	// NR24
	ROM[0xFF1A] = 0x7F;	// NR30
	ROM[0xFF1B] = 0xFF;	// NR31
	ROM[0xFF1C] = 0x9F;	// NR32
	ROM[0xFF1E] = 0xBF;	// NR33
	ROM[0xFF20] = 0xFF;	// NR41
	ROM[0xFF21] = 0x00;	// NR42
	ROM[0xFF22] = 0x00;	// NR43
	ROM[0xFF23] = 0xBF;	// NR30
	ROM[0xFF24] = 0x77;	// NR50
	ROM[0xFF25] = 0xF3;	// NR51
	ROM[0xFF26] = 0xF1;	// NR52
	ROM[0xFF40] = 0x91;	// LCDC
	ROM[0xFF42] = 0x00;	// SCY
	ROM[0xFF43] = 0x00;	// SCX
	ROM[0xFF45] = 0x00;	// LYC
	ROM[0xFF47] = 0xFC;	// BGP
	ROM[0xFF48] = 0xFF;	// OBP0
	ROM[0xFF49] = 0xFF;	// OBP1
	ROM[0xFF4A] = 0x00;	// WY
	ROM[0xFF4B] = 0x00;	// WX
	ROM[0xFFFF] = 0x00;	// IE
}


// Load the ROM into memory.
void Memory::loadROM(const char* filePath)
{
	FILE *file = NULL;

	// TODO: Should I use fopen_s instead?        For now, using _CRT_SECURE_NO_WARNINGS
	if ((file = fopen(filePath, "rb")) == NULL) {
		printf("Cannot open file.\n");
	}

	// Clear Memory
	delete[] BOOT_ROM;
	delete[] ROM;

	// Read in the ROM
	long lCurPos = ftell(file);
	fseek(file, 0, 2);
	long lEndPos = ftell(file);
	fseek(file, lCurPos, 0);
	long fileSize = lEndPos;
	BIT8* temp = new BIT8[fileSize];
	fread(temp, fileSize, 1, file);
	fclose(file);

	// Load Boot Rom
	BOOT_ROM = new BIT8[256]{
		0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
		0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
		0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
		0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
		0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
		0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
		0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
		0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
		0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
		0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
		0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
		0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
		0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
		0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
		0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x00, 0x00, 0x23, 0x7D, 0xFE, 0x34, 0x20,
		0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x00, 0x00, 0x3E, 0x01, 0xE0, 0x50
	};

	// Create ROM File		328 ROM Size
	int romSize = 32768 * std::pow(2, temp[328]);
	printf("ROM Size: %i\n", romSize);
	// FileSize special cases- they dont match the power of 2
	if (temp[328] == 0x52) romSize = 32768 * 72;
	else if (temp[328] == 0x53) romSize = 32768 * 80;
	else if (temp[328] == 0x54) romSize = 32768 * 96;
	ROM = new BIT8[romSize];
	for (int i = 0; i < fileSize; i++) {
		ROM[i] = temp[i];
	}
	delete[] temp;

	// Setup Initial Registers
	setInitRegisters();

	// Use Boot ROM
	printf("BOOT ROM: %i  (1)Disabled, (0)Enabled", ROM[0xFF50]);

	// 308-323	Title
	std::stringstream ss;
	for (int i = 308; i < 324; i++) {
		ss << std::hex << ROM[i];
	}
	//gameTitle = ss.str(); TOOD: Save gametitle somehow.. ?

	// 326	SGB Flag
	if (ROM[326] == 0x03)
		printf("This game supports SGB, but not implemented yet...\n");

	// 327	Cartridge Type
	printf("Cartridge Type: %i\n", ROM[327]);
	switch (ROM[327]) {
		// ROM ONLY
		case 0x00:
			break;
		// MBC1
		case 0x01:
			break;
		// MBC1 + RAM
		case 0x02:
			break;
		// MBC1 + RAM + BATTERY
		case 0x03:
			break;
		// MBC2
		case 0x05:
			break;
		// MBC2 + BATTERY
		case 0x06:
			break;
		// ROM + RAM
		case 0x08:
			break;
		// ROM + RAM + BATTERY
		case 0x09:
			break;
		// MMM01
		case 0x0B:
			break;
		// MMM01 + RAM
		case 0x0C:
			break;
		// MMM01 + RAM + BATTERY
		case 0x0D:
			break;
		// MBC3 + TIMER + BATTERY
		case 0x0F:
			break;
		// MBC3 + TIMER + RAM + BATTERY
		case 0x10:
			break;
		// MBC3
		case 0x11:
			break;
		// MBC3 + RAM
		case 0x12:
			break;
		// MBC3 + RAM + BATTERY
		case 0x13:
			break;
		// MBC4
		case 0x15:
			break;
		// MBC4 + RAM
		case 0x16:
			break;
		// MBC4 + RAM + BATTERY
		case 0x17:
			break;
		// MBC5
		case 0x19:
			break;
		// MBC5 + RAM
		case 0x1A:
			break;
		// MBC5 + RAM + BATTERY
		case 0x1B:
			break;
		// MBC5 + RUMBLE
		case 0x1C:
			break;
		// MBC5 + RUMBLE + RAM
		case 0x1D:
			break;
		// MBC5 + RUMBLE + RAM + BATTERY
		case 0x1E:
			break;
		// POCKET CAMERA
		case 0xFC:
			break;
		// BANDAI TAMA5
		case 0xFD:
			break;
		// HuC3
		case 0xFE:
			break;
		// HuC1 + RAM + BATTERY
		case 0xFF:
			break;
		default:
			printf("Bad Cartridge Type!\n");
			break;
	}

	// 329	RAM Size		TODO: What do i do with this?
	printf("RAM Size: %i\n", ROM[329]);
	switch (ROM[329]) {
		case 0x00:	// None
			break;
		case 0x01:	// 2KB
			break;
		case 0x02:	// 8 KB
			break;
		case 0x03:	// 32 KB (4 banks, 8KB each)
			break;
		default:
			break;
	}
	
	return;
}


/*
	R	0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
	R	4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
	R/W	8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
	R/W	A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
	?	C000-CFFF   4KB Work RAM Bank 0 (WRAM)
	?	D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
	?	E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
	?	FE00-FE9F   Sprite Attribute Table (OAM)
	?	FEA0-FEFF   Not Usable
	?	FF00-FF7F   I/O Ports
	?	FF80-FFFE   High RAM (HRAM)
	?	FFFF        Interrupt Enable Register
*/


// Read a single byte from Memory
BIT8 Memory::readByte(BIT16 location)
{
	printf("Memory.readByte(%02X)\n", location);
	return ROM[location];
}


// Write a single byte to memory.
void Memory::writeByte(BIT16 location, BIT8 value)
{
	if (location < 0x8000)
		printf("Bad Write. Shouldn't be able to write to %04X\n", location);

	// Write to memory array
	ROM[location] = value;
		
	// Important Methods
	if (location == 0xFF44 || location == 0xFF45)
		lyc();
	else if (location == 0xFF46)
		dmaTransfer();
		
	// Echo of 8kB Internal RAM:  If you write a byte to E000, it will appear at C000 as well.	(E000-FE00 and C000-DE00 are copies of each other.)
	if (location >= 0xC000 && location < 0xDE00)
		ROM[location + 8192] = value;
	else if (location >= 0xE000 && location < 0xFE00)
		ROM[location - 8192] = value;
}


void Memory::lyc()
{
	if (ROM[0xFF44] == ROM[0xFF45]) {
		printf("Z80::lyc() Setting coincident bit.\n");
		writeByte(0xFF41, (readByte(0xFF41) | (1UL<<6)) );
	}
}


void Memory::dmaTransfer()
{
	printf("\nDMA Transfer Request.\n");
}
