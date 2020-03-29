#include "pch.h"
#include "CppUnitTest.h"
#include "../Gameboy/Z80.h"
#include "../Gameboy/Z80.cpp"
#include "../Gameboy/Memory.h"
#include "../Gameboy/Memory.cpp"
#include "../Gameboy/Globals.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			// Needed this for some assert statements
			template<> inline std::wstring ToString<BIT16>(const BIT16& t) { RETURN_WIDE_STRING(t); }
		}
	}
}

#define UNCHANGED ' '

namespace UnitTests
{
	// Testing our CPU (Z80) Instruction Sets
	TEST_CLASS(CPU)
	{
	public:
		// Mem & CPU objects for testing.
		Memory memory = Memory();
		Z80 cpu = Z80(&memory);


		// TODO:       03 04 05 07 08 09 0B 0C 0D 0F
		// TODO: 10    13 14 15 17 18 19 1B 1C 1D 1F
		// TODO: 20    23 24 25 27 28 29 2B 2C 2D 2F
		// TODO: 30 31 33 34 35 37 38 39 3B 3C 3D 3F
		// TODO: 76
		// TODO: 80-DF
		// TODO: E1 E5 E6 E7 E8 E9 EE EF
		// TODO: F1 F3 F5 F6 F7 F8 F9 FB FE FF

		// TODO: cb00 - cbFF


		#pragma region 00 - 0F
		
		// 00 - No Operation.
		TEST_METHOD(i00)
		{
			memory.writeByte(0x0100, 0x00);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0101, UNCHANGED);
		}

		// 01 - LD BC,d16
		TEST_METHOD(i01)
		{
			memory.writeByte(0x0100, 0x01);
			memory.writeByte(0x0101, 0x54);
			memory.writeByte(0x0102, 0x67);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, 0x67, 0x54, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 02 - LD (BC),A
		TEST_METHOD(i02)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x0E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x02);
			cpu.execute(); // LD B,n
			cpu.execute(); // LD C,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (BC),A
			assertRegisters(0x23, UNCHANGED, 0x03, 0x13, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0107, UNCHANGED);
			assertMemory(0x0313, 0x23);
		}

		// 06 - LD B,n
		TEST_METHOD(i06)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0102, UNCHANGED);
		}

		// 0A - LD A,(BC)
		TEST_METHOD(i0A)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x0E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x0A);
			memory.writeByte(0x0313, 0x23);
			cpu.execute(); // LD B,n
			cpu.execute(); // LD C,n
			cpu.execute(); // LD A,(BC)
			assertRegisters(0x23, UNCHANGED, 0x03, 0x13, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0105, UNCHANGED);
		}

		// 0E - LD C,n
		TEST_METHOD(i0E)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x2E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0102, UNCHANGED);
		}
		#pragma endregion


		#pragma region 10 - 1F
		
		// 11 - LD DE,d16
		TEST_METHOD(i11)
		{
			memory.writeByte(0x0100, 0x11);
			memory.writeByte(0x0101, 0x96);
			memory.writeByte(0x0102, 0xAB);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xAB, 0x96, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 12 - LD (DE),A
		TEST_METHOD(i12)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x1E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x12);
			cpu.execute(); // LD D,n
			cpu.execute(); // LD E,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (BC),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, 0x03, 0x13, UNCHANGED, UNCHANGED, 0x0107, UNCHANGED);
			assertMemory(0x0313, 0x23);
		}

		// 16 - LD D,n
		TEST_METHOD(i16)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x3E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0102, UNCHANGED);
		}

		// 1A - LD A,(DE)
		TEST_METHOD(i1A)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x1E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x1A);
			memory.writeByte(0x0313, 0x23);
			cpu.execute(); // LD D,n
			cpu.execute(); // LD E,n
			cpu.execute(); // LD A,(BC)
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, 0x03, 0x13, UNCHANGED, UNCHANGED, 0x0105, UNCHANGED);
		}

		// 1E - LD E,n
		TEST_METHOD(i1E)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x4E, UNCHANGED, UNCHANGED, 0x0102, UNCHANGED);
		}
		#pragma endregion


		#pragma region 20 - 2F
		
		// 21 - LD HL,d16
		TEST_METHOD(i21)
		{
			memory.writeByte(0x0100, 0x21);
			memory.writeByte(0x0101, 0xCD);
			memory.writeByte(0x0102, 0x64);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x64, 0xCD, 0x0103, UNCHANGED);
		}

		// 22 - LD (HL+),A
		TEST_METHOD(i22_1)	// Standard Use Case
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x22);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (HL+),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x03, 0x14, 0x0107, UNCHANGED);
			assertMemory(0x0313, 0x23);
		}
		TEST_METHOD(i22_2) // L is 0xFF and should go to 0x00 while H should increment.
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0xFF);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x22);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (HL+),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x04, 0x00, 0x0107, UNCHANGED);
			assertMemory(0x03FF, 0x23);
		}
		TEST_METHOD(i22_3) // L&H both 0xFF, we should end up with both at 0x00
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0xFF);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0xFF);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x22);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (HL+),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x00, 0x00, 0x0107, UNCHANGED);
			assertMemory(0xFFFF, 0x23);
		}

		// 26 - LD H,n
		TEST_METHOD(i26)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, 0x0102, UNCHANGED);
		}

		// 2A - LD A,(HL+)
		TEST_METHOD(i2A_1)	// Standard Use Case
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x2A);
			memory.writeByte(0x0313, 0x23);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL+),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x03, 0x14, 0x0105, UNCHANGED);
		}
		TEST_METHOD(i2A_2) // L is 0xFF and should go to 0x00 while H should increment.
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0xFF);
			memory.writeByte(0x0104, 0x2A);
			memory.writeByte(0x03FF, 0x23);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL+),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x04, 0x00, 0x0105, UNCHANGED);
		}
		TEST_METHOD(i2A_3) // L&H both 0xFF, we should end up with both at 0x00
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0xFF);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0xFF);
			memory.writeByte(0x0104, 0x2A);
			memory.writeByte(0xFFFF, 0x23);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL+),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x00, 0x00, 0x0105, UNCHANGED);
		}

		// 2E - LD L,n
		TEST_METHOD(i2E)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			cpu.execute();
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, 0x0102, UNCHANGED);
		}
		#pragma endregion

		
		#pragma region 30 - 3F

		// 32 - LD (HL-),A
		TEST_METHOD(i32_1)	// Standard Use Case
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x32);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (HL-),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x03, 0x12, 0x0107, UNCHANGED);
			assertMemory(0x0313, 0x23);
		}
		TEST_METHOD(i32_2) // L is 0x00 and should go to 0xFF while H should decrement.
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x00);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x32);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (HL-),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x02, 0xFF, 0x0107, UNCHANGED);
			assertMemory(0x0300, 0x23);
		}
		TEST_METHOD(i32_3) // L&H both 0x00, we should end up with both at 0xFF
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x00);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x00);
			memory.writeByte(0x0104, 0x3E);
			memory.writeByte(0x0105, 0x23);
			memory.writeByte(0x0106, 0x32);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,n
			cpu.execute(); // LD (HL-),A
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xFF, 0xFF, 0x0107, UNCHANGED);
			assertMemory(0x0000, 0x23);
		}

		// 36 - LD (HL),n
		TEST_METHOD(i36)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x04);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x14);
			memory.writeByte(0x0104, 0x36);
			memory.writeByte(0x0105, 0x24);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),n
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x04, 0x14, 0x0106, UNCHANGED);
			assertMemory(0x0414, 0x24);
		}

		// 3A - LD A,(HL-)
		TEST_METHOD(i3A_1)	// Standard Use Case
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x13);
			memory.writeByte(0x0104, 0x3A);
			memory.writeByte(0x0313, 0x23);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,(HL-)
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x03, 0x12, 0x0105, UNCHANGED);
		}
		TEST_METHOD(i3A_2) // L is 0x00 and should go to 0xFF while H should decrement.
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x03);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x00);
			memory.writeByte(0x0104, 0x3A);
			memory.writeByte(0x0300, 0x23);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,(HL-)
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x02, 0xFF, 0x0105, UNCHANGED);
		}
		TEST_METHOD(i3A_3) // L&H both 0x00, we should end up with both at 0xFF
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x00);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x00);
			memory.writeByte(0x0104, 0x3A);
			memory.writeByte(0x0000, 0x23);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,(HL-)
			assertRegisters(0x23, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xFF, 0xFF, 0x0105, UNCHANGED);
		}

		// 3E - LD A,n
		TEST_METHOD(i3E)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			cpu.execute();
			assertRegisters(0x7E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0102, UNCHANGED);
		}
		#pragma endregion


		#pragma region 40 - 4F

		// 40 - LD B,B
		TEST_METHOD(i40)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			memory.writeByte(0x0102, 0x40);
			cpu.execute();	// LD B,n
			cpu.execute();	// LD B,B
			assertRegisters(UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 41 - LD B,C
		TEST_METHOD(i41)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			memory.writeByte(0x0102, 0x41);
			cpu.execute();	// LD C,n
			cpu.execute();	// LD B,C
			assertRegisters(UNCHANGED, UNCHANGED, 0x2E, 0x2E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 42 - LD B,D
		TEST_METHOD(i42)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			memory.writeByte(0x0102, 0x42);
			cpu.execute();	// LD D,n
			cpu.execute();	// LD B,D
			assertRegisters(UNCHANGED, UNCHANGED, 0x3E, UNCHANGED, 0x3E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 43 - LD B,E
		TEST_METHOD(i43)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			memory.writeByte(0x0102, 0x43);
			cpu.execute();	// LD E,n
			cpu.execute();	// LD B,E
			assertRegisters(UNCHANGED, UNCHANGED, 0x4E, UNCHANGED, UNCHANGED, 0x4E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 44 - LD B,H
		TEST_METHOD(i44)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			memory.writeByte(0x0102, 0x44);
			cpu.execute();	// LD H,n
			cpu.execute();	// LD B,H
			assertRegisters(UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 45 - LD B,L
		TEST_METHOD(i45)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			memory.writeByte(0x0102, 0x45);
			cpu.execute();	// LD L,n
			cpu.execute();	// LD B,L
			assertRegisters(UNCHANGED, UNCHANGED, 0x6E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, 0x0103, UNCHANGED);
		}

		// 46 - LD B,(HL)
		TEST_METHOD(i46)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x8E);
			memory.writeByte(0x0104, 0x46);
			memory.writeByte(0x7E8E, 0x9E);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD B,(HL)
			assertRegisters(UNCHANGED, UNCHANGED, 0x9E, UNCHANGED, UNCHANGED, UNCHANGED, 0x7E, 0x8E, 0x0105, UNCHANGED);
		}

		// 47 - LD B,A
		TEST_METHOD(i47)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x47);
			cpu.execute();	// LD A,n
			cpu.execute();	// LD B,A
			assertRegisters(0x7E, UNCHANGED, 0x7E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 48 - LD C,B
		TEST_METHOD(i48)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			memory.writeByte(0x0102, 0x48);
			cpu.execute();	// LD B,n
			cpu.execute();	// LD C,B
			assertRegisters(UNCHANGED, UNCHANGED, 0x1E, 0x1E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 49 - LD C,C
		TEST_METHOD(i49)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			memory.writeByte(0x0102, 0x49);
			cpu.execute();	// LD C,n
			cpu.execute();	// LD C,C
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x2E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 4A - LD C,D
		TEST_METHOD(i4A)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			memory.writeByte(0x0102, 0x4A);
			cpu.execute();	// LD D,n
			cpu.execute();	// LD C,D
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x3E, 0x3E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 4B - LD C,E
		TEST_METHOD(i4B)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			memory.writeByte(0x0102, 0x4B);
			cpu.execute();	// LD E,n
			cpu.execute();	// LD C,E
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x4E, UNCHANGED, 0x4E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 4C - LD C,H
		TEST_METHOD(i4C)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			memory.writeByte(0x0102, 0x4C);
			cpu.execute();	// LD H,n
			cpu.execute();	// LD C,H
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 4D - LD C,L
		TEST_METHOD(i4D)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			memory.writeByte(0x0102, 0x4D);
			cpu.execute();	// LD L,n
			cpu.execute();	// LD C,L
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, 0x0103, UNCHANGED);
		}

		// 4E - LD C,(HL)
		TEST_METHOD(i4E)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x8E);
			memory.writeByte(0x0104, 0x4E);
			memory.writeByte(0x7E8E, 0x9E);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD C,(HL)
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x9E, UNCHANGED, UNCHANGED, 0x7E, 0x8E, 0x0105, UNCHANGED);
		}

		// 4F - LD C,A
		TEST_METHOD(i4F)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x4F);
			cpu.execute();	// LD A,n
			cpu.execute();	// LD C,A
			assertRegisters(0x7E, UNCHANGED, UNCHANGED, 0x7E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}
		#pragma endregion


		#pragma region 50 - 5F

		// 50 - LD D,B
		TEST_METHOD(i50)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			memory.writeByte(0x0102, 0x50);
			cpu.execute();	// LD B,n
			cpu.execute();	// LD D,B
			assertRegisters(UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 51 - LD D,C
		TEST_METHOD(i51)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			memory.writeByte(0x0102, 0x51);
			cpu.execute();	// LD C,n
			cpu.execute();	// LD D,C
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x2E, 0x2E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 52 - LD D,D
		TEST_METHOD(i52)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			memory.writeByte(0x0102, 0x52);
			cpu.execute();	// LD D,n
			cpu.execute();	// LD D,D
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x3E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 53 - LD D,E
		TEST_METHOD(i53)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			memory.writeByte(0x0102, 0x53);
			cpu.execute();	// LD E,n
			cpu.execute();	// LD D,E
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x4E, 0x4E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 54 - LD D,H
		TEST_METHOD(i54)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			memory.writeByte(0x0102, 0x54);
			cpu.execute();	// LD H,n
			cpu.execute();	// LD D,H
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, 0x5E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 55 - LD D,L
		TEST_METHOD(i55)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			memory.writeByte(0x0102, 0x55);
			cpu.execute();	// LD L,n
			cpu.execute();	// LD D,L
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, UNCHANGED, UNCHANGED, 0x6E, 0x0103, UNCHANGED);
		}

		// 56 - LD D,(HL)
		TEST_METHOD(i56)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x8E);
			memory.writeByte(0x0104, 0x56);
			memory.writeByte(0x7E8E, 0x9E);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD D,(HL)
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x9E, UNCHANGED, 0x7E, 0x8E, 0x0105, UNCHANGED);
		}

		// 57 - LD D,A
		TEST_METHOD(i57)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x57);
			cpu.execute();	// LD A,n
			cpu.execute();	// LD D,A
			assertRegisters(0x7E, UNCHANGED, UNCHANGED, UNCHANGED, 0x7E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 58 - LD E,B
		TEST_METHOD(i58)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			memory.writeByte(0x0102, 0x58);
			cpu.execute();	// LD B,n
			cpu.execute();	// LD E,B
			assertRegisters(UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 59 - LD E,C
		TEST_METHOD(i59)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			memory.writeByte(0x0102, 0x59);
			cpu.execute();	// LD C,n
			cpu.execute();	// LD E,C
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x2E, UNCHANGED, 0x2E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 5A - LD E,D
		TEST_METHOD(i5A)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			memory.writeByte(0x0102, 0x5A);
			cpu.execute();	// LD D,n
			cpu.execute();	// LD E,D
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x3E, 0x3E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 5B - LD E,E
		TEST_METHOD(i5B)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			memory.writeByte(0x0102, 0x5B);
			cpu.execute();	// LD E,n
			cpu.execute();	// LD E,E
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x4E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 5C - LD E,H
		TEST_METHOD(i5C)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			memory.writeByte(0x0102, 0x5C);
			cpu.execute();	// LD H,n
			cpu.execute();	// LD E,H
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, 0x5E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 5D - LD E,L
		TEST_METHOD(i5D)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			memory.writeByte(0x0102, 0x5D);
			cpu.execute();	// LD L,n
			cpu.execute();	// LD E,L
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, UNCHANGED, 0x6E, 0x0103, UNCHANGED);
		}

		// 5E - LD E,(HL)
		TEST_METHOD(i5E)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x8E);
			memory.writeByte(0x0104, 0x5E);
			memory.writeByte(0x7E8E, 0x9E);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD E,(HL)
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x9E, 0x7E, 0x8E, 0x0105, UNCHANGED);
		}

		// 5F - LD E,A
		TEST_METHOD(i5F)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x5F);
			cpu.execute();	// LD A,n
			cpu.execute();	// LD E,A
			assertRegisters(0x7E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x7E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		#pragma endregion

		
		#pragma region 60 - 6F

		// 60 - LD H,B
		TEST_METHOD(i60)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			memory.writeByte(0x0102, 0x60);
			cpu.execute();	// LD B,n
			cpu.execute();	// LD H,B
			assertRegisters(UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 61 - LD H,C
		TEST_METHOD(i61)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			memory.writeByte(0x0102, 0x61);
			cpu.execute();	// LD C,n
			cpu.execute();	// LD H,C
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x2E, UNCHANGED, UNCHANGED, 0x2E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 62 - LD H,D
		TEST_METHOD(i62)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			memory.writeByte(0x0102, 0x62);
			cpu.execute();	// LD D,n
			cpu.execute();	// LD H,D
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x3E, UNCHANGED, 0x3E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 63 - LD H,E
		TEST_METHOD(i63)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			memory.writeByte(0x0102, 0x63);
			cpu.execute();	// LD E,n
			cpu.execute();	// LD H,E
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x4E, 0x4E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 64 - LD H,H
		TEST_METHOD(i64)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			memory.writeByte(0x0102, 0x64);
			cpu.execute();	// LD H,n
			cpu.execute();	// LD H,H
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 65 - LD H,L
		TEST_METHOD(i65)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			memory.writeByte(0x0102, 0x65);
			cpu.execute();	// LD L,n
			cpu.execute();	// LD H,L
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, 0x6E, 0x0103, UNCHANGED);
		}

		// 66 - LD H,(HL)
		TEST_METHOD(i66)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x8E);
			memory.writeByte(0x0104, 0x66);
			memory.writeByte(0x7E8E, 0x9E);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD B,(HL)
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x9E, 0x8E, 0x0105, UNCHANGED);
		}

		// 67 - LD H,A
		TEST_METHOD(i67)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x67);
			cpu.execute();	// LD A,n
			cpu.execute();	// LD H,A
			assertRegisters(0x7E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x7E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 68 - LD L,B
		TEST_METHOD(i68)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			memory.writeByte(0x0102, 0x68);
			cpu.execute();	// LD B,n
			cpu.execute();	// LD B,B
			assertRegisters(UNCHANGED, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x1E, 0x0103, UNCHANGED);
		}

		// 69 - LD L,C
		TEST_METHOD(i69)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			memory.writeByte(0x0102, 0x69);
			cpu.execute();	// LD C,n
			cpu.execute();	// LD L,C
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0x2E, UNCHANGED, UNCHANGED, UNCHANGED, 0x2E, 0x0103, UNCHANGED);
		}

		// 6A - LD L,D
		TEST_METHOD(i6A)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			memory.writeByte(0x0102, 0x6A);
			cpu.execute();	// LD D,n
			cpu.execute();	// LD L,D
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x3E, UNCHANGED, UNCHANGED, 0x3E, 0x0103, UNCHANGED);
		}

		// 6B - LD L,E
		TEST_METHOD(i6B)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			memory.writeByte(0x0102, 0x6B);
			cpu.execute();	// LD E,n
			cpu.execute();	// LD L,E
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x4E, UNCHANGED, 0x4E, 0x0103, UNCHANGED);
		}

		// 6C - LD L,H
		TEST_METHOD(i6C)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			memory.writeByte(0x0102, 0x6C);
			cpu.execute();	// LD H,n
			cpu.execute();	// LD L,H
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, 0x5E, 0x0103, UNCHANGED);

		}
		// 6D - LD L,L
		TEST_METHOD(i6D)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			memory.writeByte(0x0102, 0x6D);
			cpu.execute();	// LD L,n
			cpu.execute();	// LD L,L
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, 0x0103, UNCHANGED);
		}

		// 6E - LD L,(HL)
		TEST_METHOD(i6E)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x8E);
			memory.writeByte(0x0104, 0x6E);
			memory.writeByte(0x7E8E, 0x9E);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD L,(HL)
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x7E, 0x9E, 0x0105, UNCHANGED);
		}

		// 6F - LD L,A
		TEST_METHOD(i6F)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x6F);
			cpu.execute();	// LD A,n
			cpu.execute();	// LD L,A
			assertRegisters(0x7E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x7E, 0x0103, UNCHANGED);
		}

		#pragma endregion

		
		#pragma region 70 - 7F
		
		// 70 - LD (HL),B
		TEST_METHOD(i70)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0xAE);
			memory.writeByte(0x0102, 0x26);
			memory.writeByte(0x0103, 0xBE);
			memory.writeByte(0x0104, 0x2E);
			memory.writeByte(0x0105, 0xCE);
			memory.writeByte(0x0106, 0x70);
			cpu.execute(); // LD B,n
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),B
			assertRegisters(UNCHANGED, UNCHANGED, 0xAE, UNCHANGED, UNCHANGED, UNCHANGED, 0xBE, 0xCE, 0x0107, UNCHANGED);
			assertMemory(0xBECE, 0xAE);
		}

		// 71 - LD (HL),C
		TEST_METHOD(i71)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0xAE);
			memory.writeByte(0x0102, 0x26);
			memory.writeByte(0x0103, 0xBE);
			memory.writeByte(0x0104, 0x2E);
			memory.writeByte(0x0105, 0xCE);
			memory.writeByte(0x0106, 0x71);
			cpu.execute(); // LD C,n
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),C
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, 0xAE, UNCHANGED, UNCHANGED, 0xBE, 0xCE, 0x0107, UNCHANGED);
			assertMemory(0xBECE, 0xAE);
		}

		// 72 - LD (HL),D
		TEST_METHOD(i72)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0xAE);
			memory.writeByte(0x0102, 0x26);
			memory.writeByte(0x0103, 0xBE);
			memory.writeByte(0x0104, 0x2E);
			memory.writeByte(0x0105, 0xCE);
			memory.writeByte(0x0106, 0x72);
			cpu.execute(); // LD D,n
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),D
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xAE, UNCHANGED, 0xBE, 0xCE, 0x0107, UNCHANGED);
			assertMemory(0xBECE, 0xAE);
		}

		// 73 - LD (HL),E
		TEST_METHOD(i73)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0xAE);
			memory.writeByte(0x0102, 0x26);
			memory.writeByte(0x0103, 0xBE);
			memory.writeByte(0x0104, 0x2E);
			memory.writeByte(0x0105, 0xCE);
			memory.writeByte(0x0106, 0x73);
			cpu.execute(); // LD E,n
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),E
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xAE, 0xBE, 0xCE, 0x0107, UNCHANGED);
			assertMemory(0xBECE, 0xAE);
		}

		// 74 - LD (HL),H
		TEST_METHOD(i74)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0xBE);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0xCE);
			memory.writeByte(0x0104, 0x74);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),H
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xBE, 0xCE, 0x0105, UNCHANGED);
			assertMemory(0xBECE, 0xBE);
		}

		// 75 - LD (HL),L
		TEST_METHOD(i75)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0xBE);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0xCE);
			memory.writeByte(0x0104, 0x75);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),L
			assertRegisters(UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xBE, 0xCE, 0x0105, UNCHANGED);
			assertMemory(0xBECE, 0xCE);
		}

		// 77 - LD (HL),A
		TEST_METHOD(i77)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0xAE);
			memory.writeByte(0x0102, 0x26);
			memory.writeByte(0x0103, 0xBE);
			memory.writeByte(0x0104, 0x2E);
			memory.writeByte(0x0105, 0xCE);
			memory.writeByte(0x0106, 0x77);
			cpu.execute(); // LD E,n
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD (HL),A
			assertRegisters(0xAE, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0xBE, 0xCE, 0x0107, UNCHANGED);
			assertMemory(0xBECE, 0xAE);
		}

		// 78 - LD A,B
		TEST_METHOD(i78)
		{
			memory.writeByte(0x0100, 0x06);
			memory.writeByte(0x0101, 0x1E);
			memory.writeByte(0x0102, 0x78);
			cpu.execute();	// LD B,n
			cpu.execute();	// LD A,B
			assertRegisters(0x1E, UNCHANGED, 0x1E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 79 - LD A,C
		TEST_METHOD(i79)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x2E);
			memory.writeByte(0x0102, 0x79);
			cpu.execute();	// LD C,n
			cpu.execute();	// LD A,C
			assertRegisters(0x2E, UNCHANGED, UNCHANGED, 0x2E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 7A - LD A,D
		TEST_METHOD(i7A)
		{
			memory.writeByte(0x0100, 0x16);
			memory.writeByte(0x0101, 0x3E);
			memory.writeByte(0x0102, 0x7A);
			cpu.execute();	// LD D,n
			cpu.execute();	// LD A,D
			assertRegisters(0x3E, UNCHANGED, UNCHANGED, UNCHANGED, 0x3E, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 7B - LD A,E
		TEST_METHOD(i7B)
		{
			memory.writeByte(0x0100, 0x1E);
			memory.writeByte(0x0101, 0x4E);
			memory.writeByte(0x0102, 0x7B);
			cpu.execute();	// LD E,n
			cpu.execute();	// LD A,E
			assertRegisters(0x4E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x4E, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 7C - LD A,H
		TEST_METHOD(i7C)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x5E);
			memory.writeByte(0x0102, 0x7C);
			cpu.execute();	// LD H,n
			cpu.execute();	// LD A,H
			assertRegisters(0x5E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x5E, UNCHANGED, 0x0103, UNCHANGED);
		}

		// 7D - LD A,L
		TEST_METHOD(i7D)
		{
			memory.writeByte(0x0100, 0x2E);
			memory.writeByte(0x0101, 0x6E);
			memory.writeByte(0x0102, 0x7D);
			cpu.execute();	// LD L,n
			cpu.execute();	// LD A,L
			assertRegisters(0x6E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x6E, 0x0103, UNCHANGED);
		}

		// 7E - LD A,(HL)
		TEST_METHOD(i7E)
		{
			memory.writeByte(0x0100, 0x26);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x2E);
			memory.writeByte(0x0103, 0x8E);
			memory.writeByte(0x0104, 0x7E);
			memory.writeByte(0x7E8E, 0x9E);
			cpu.execute(); // LD H,n
			cpu.execute(); // LD L,n
			cpu.execute(); // LD A,(HL)
			assertRegisters(0x9E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x7E, 0x8E, 0x0105, UNCHANGED);
		}

		// 7F - LD A,A
		TEST_METHOD(i7F)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x7E);
			memory.writeByte(0x0102, 0x7F);
			cpu.execute();	// LD A,n
			cpu.execute();	// LD A,A
			assertRegisters(0x7E, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		#pragma endregion

		
		#pragma region E0 - EF

		// E0 - LDH n,A
		TEST_METHOD(iE0)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x07);
			memory.writeByte(0x0102, 0xE0);
			memory.writeByte(0x0103, 0x17);
			cpu.execute(); // LD A,n
			cpu.execute(); // LD n,A
			assertRegisters(0x07, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0104, UNCHANGED);
			assertMemory(0xFF17, 0x07);
		}
		
		// E2 - LD (C),A
		TEST_METHOD(iE2)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x07);
			memory.writeByte(0x0102, 0x0E);
			memory.writeByte(0x0103, 0x17);
			memory.writeByte(0x0104, 0xE2);
			cpu.execute(); // LD A,n
			cpu.execute(); // LD C,n
			cpu.execute(); // LD (C),A
			assertRegisters(0x07, UNCHANGED, UNCHANGED, 0x17, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0105, UNCHANGED);
			assertMemory(0xFF17, 0x07);
		}

		// EA - LD nn,A
		TEST_METHOD(iEA)
		{
			memory.writeByte(0x0100, 0x3E);
			memory.writeByte(0x0101, 0x07);
			memory.writeByte(0x0102, 0xEA);
			memory.writeByte(0x0103, 0x17);
			memory.writeByte(0x0104, 0x27);
			cpu.execute(); // LD A,n
			cpu.execute(); // LD n,A
			assertRegisters(0x07, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0105, UNCHANGED);
			assertMemory(0x2717, 0x07);
		}

		#pragma endregion


		#pragma region F0 - FF

		// F0 - LDH A,n
		TEST_METHOD(iF0)
		{
			memory.writeByte(0x0100, 0xF0);
			memory.writeByte(0x0101, 0x07);
			memory.writeByte(0xFF07, 0x17);
			cpu.execute(); // LD A,n
			assertRegisters(0x17, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0102, UNCHANGED);
		}

		// F2 - LD A,(C)
		TEST_METHOD(iF2)
		{
			memory.writeByte(0x0100, 0x0E);
			memory.writeByte(0x0101, 0x07);
			memory.writeByte(0x0102, 0xF2);
			memory.writeByte(0xFF07, 0x17);
			cpu.execute(); // LD C,n
			cpu.execute(); // LD A,(C)
			assertRegisters(0x17, UNCHANGED, UNCHANGED, 0x07, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		// FA - LD A,nn
		TEST_METHOD(iFA)
		{
			memory.writeByte(0x0100, 0xFA);
			memory.writeByte(0x0101, 0x07);
			memory.writeByte(0x0102, 0x17);
			memory.writeByte(0x1707, 0x27);
			cpu.execute(); // LD A,n
			assertRegisters(0x27, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, UNCHANGED, 0x0103, UNCHANGED);
		}

		#pragma endregion
		

	private:
		// Allows us to check all registers at once.
		// Allows UNCHANGED to be passed if the register is expected to be unchanged.
		void assertRegisters(	BIT8 expectedA, BIT8 expectedF, BIT8 expectedB, BIT8 expectedC,
								BIT8 expectedD, BIT8 expectedE, BIT8 expectedH, BIT8 expectedL,
								BIT16 expectedPC, BIT16 expectedSP)
		{
			if(expectedA!=UNCHANGED)
				Assert::AreEqual(expectedA, cpu.getA(), L"A modified. Contains incorrect value.", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0x01, cpu.getA(), L"A modified. Expected default value.", LINE_INFO());

			if (expectedF != UNCHANGED)
				Assert::AreEqual(expectedF, cpu.getF(), L"Incorrect value in Register F", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0xB0, cpu.getF(), L"F modified. Expected default value.", LINE_INFO());

			if (expectedB != UNCHANGED)
				Assert::AreEqual(expectedB, cpu.getB(), L"Incorrect value in Register B", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0x00, cpu.getB(), L"B modified. Expected default value.", LINE_INFO());

			if (expectedC != UNCHANGED)
				Assert::AreEqual(expectedC, cpu.getC(), L"Incorrect value in Register C", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0x13, cpu.getC(), L"C modified. Expected default value.", LINE_INFO());

			if (expectedD != UNCHANGED)
				Assert::AreEqual(expectedD, cpu.getD(), L"Incorrect value in Register D", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0x00, cpu.getD(), L"D modified. Expected default value.", LINE_INFO());

			if (expectedE != UNCHANGED)
				Assert::AreEqual(expectedE, cpu.getE(), L"Incorrect value in Register E", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0xD8, cpu.getE(), L"E modified. Expected default value.", LINE_INFO());

			if (expectedH != UNCHANGED)
				Assert::AreEqual(expectedH, cpu.getH(), L"Incorrect value in Register H", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0x01, cpu.getH(), L"H modified. Expected default value.", LINE_INFO());

			if (expectedL != UNCHANGED)
				Assert::AreEqual(expectedL, cpu.getL(), L"Incorrect value in Register L", LINE_INFO());
			else
				Assert::AreEqual((BIT8)0x4D, cpu.getL(), L"L modified. Expected default value.", LINE_INFO());

			if (expectedPC != UNCHANGED)
				Assert::AreEqual(expectedPC, cpu.getPC(), L"Incorrect value in Register PC", LINE_INFO());
			else
				Assert::AreEqual((BIT16)0x0100, cpu.getPC(), L"PC modified. Expected default value.", LINE_INFO());

			if (expectedSP != UNCHANGED)
				Assert::AreEqual(expectedSP, cpu.getSP(), L"Incorrect value in Register SP", LINE_INFO());
			else
				Assert::AreEqual((BIT16)0xFFFE, cpu.getSP(), L"SP modified. Expected default value.", LINE_INFO());
		}

		// Allows us to check a value at a specific memory address.
		void assertMemory(BIT16 location, BIT8 value)
		{
			Assert::AreEqual(value, memory.readByte(location), L"Memory address incorrect.", LINE_INFO());
		}
	};
}
