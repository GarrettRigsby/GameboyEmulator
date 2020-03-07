#include<thread>
#include "Memory.h"
#include "Z80.h"
#include "Video.h"

using namespace std;

// Setup Hardware
Memory gbMem = Memory();
Z80 gbCpu = Z80(&gbMem);
//Video gbGpu(&gbMem, 160*4, 144*4, "Garrett GB Emulator"); // TODO: This * 4 should maybe be global? Its used in Video.cpp too.
Video gbGpu(&gbMem, 256*3, 256*3, "Garrett GB Emulator"); //TODO: Currently testing with full background 256x256

// This method will only run in the while() loop. This is primarily for code-cleanup.
void doStuff()
{
	// TODO: If CPU Halted, should we avoid all of this?
	
	//BIT8 IF = gbMem.readByte(0xFF0F);
	//BIT8 IE = gbMem.readByte(0xFFFF);
	//printf("ime(%s)  IF(%02X)  IE(%02X)\n", (gbCpu.getIme() ? "true" : "false"), IF, IE);

	//	Interrupt Requests
	//	- Any set bits in the IF register are only requesting an interrupt to be executed.
	//	- The actual execution happens only if both the IME flag, and the corresponding bit in the IE register are set,
	//	- otherwise the interrupt waits until both IME and IE allow its execution.
	if (gbCpu.getIme())
	{
		// Reset Variables
		//gbCpu.setHalt(false);	// TODO: Should halt be false now?

		// FF0F - IF - Interrupt Flag (R/W)
		// FFFF - IE - Interrupt Enable (R/W)
		BIT8 interruptFlag = gbMem.readByte(0xFF0F);
		BIT8 needToInterrupt = interruptFlag & gbMem.readByte(0xFFFF);
		
		printf("INTERRUPT: (%02X) (%02X)\n", interruptFlag, needToInterrupt);
		
		if (!needToInterrupt)
			return;

		printf("Interrupt\n");

		//	Interrupt Priorities
		//	- Provided that IME and IE allow the execution of more than one of the requested interrupts, 
		//	- then the interrupt with the highest priority becomes executed first.
		//	- The priorities are ordered as the bits in the IE and IF registers, 
		//	- Bit 0 (V-Blank) having the highest priority, and Bit 4 (Joypad) having the lowest priority.

		// Bit 0: V-Blank  (INT 40h)  (1=Enable)
		if (needToInterrupt & 0x01) {
			gbMem.writeByte(0xFF0F, (interruptFlag & 0xFE));
			gbCpu.putPCOnStack();
			gbCpu.setProgramCounter(0x40);
			gbCpu.setIme(false);
			return;
		}

		// Bit 1: LCD STAT Interrupt Enable  (INT 48h)  (1=Enable)
		if (needToInterrupt & 0x02) {
			gbMem.writeByte(0xFF0F, (needToInterrupt & 0xFD));
			gbCpu.putPCOnStack();
			gbCpu.setProgramCounter(0x48);
			gbCpu.setIme(false);
			return;
		}

		// Bit 2: Timer    Interrupt Enable  (INT 50h)  (1=Enable)
		if (needToInterrupt & 0x04) {
			gbMem.writeByte(0xFF0F, (needToInterrupt & 0xFB));
			gbCpu.putPCOnStack();
			gbCpu.setProgramCounter(0x50);
			gbCpu.setIme(false);
			return;
		}

		// Bit 3: Serial   Interrupt Enable  (INT 58h)  (1=Enable)
		if (needToInterrupt & 0x08) {
			gbMem.writeByte(0xFF0F, (needToInterrupt & 0xF7));
			gbCpu.putPCOnStack();
			gbCpu.setProgramCounter(0x58);
			gbCpu.setIme(false);
			return;
		}

		// Bit 4: Joypad   Interrupt Enable  (INT 60h)  (1=Enable)
		if (needToInterrupt & 0x10) {
			gbMem.writeByte(0xFF0F, (needToInterrupt & 0xEF));
			gbCpu.putPCOnStack();
			gbCpu.setProgramCounter(0x60);
			gbCpu.setIme(false);
			return;
		}
	}
	else {
		// Execute Next Instruction
		gbCpu.execute();
	}
	// timer.increment()   //TODO: When do we compare to cpu clock cycles, and what exactly are cpu clocks use for?
}


int main(int argc, char *argv[])
{
	printf("======= Starting Emulation ========\n\n");

	// Load ROM
	//gbMem.loadROM(argv[1]);


	// TESTING ON LAPTOP
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\Tetris.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\BB.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\bgbtest.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\DuckTales.gb");
	gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\cpu_instrs.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\01-special.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\02-interrupts.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\03-op sp,hl.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\04-op r,imm.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\05-op rp.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\06-ld r,r.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\07-jr,jp,call,ret,rst.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\08-misc instrs.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\09-op r,r.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\10-bit ops.gb");
	//gbMem.loadROM("C:\\Users\\Christy\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\11-op a,(hl).gb");

	// TESTING ON DESKTOP
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\Tetris.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\BB.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\bgbtest.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\DuckTales.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\cpu_instrs.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\01-special.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\02-interrupts.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\03-op sp,hl.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\04-op r,imm.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\05-op rp.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\06-ld r,r.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\07-jr,jp,call,ret,rst.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\08-misc instrs.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\09-op r,r.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\10-bit ops.gb");
	//gbMem.loadROM("C:\\Users\\garre\\Documents\\GitHub\\EmulationApp\\ROMS\\Gameboy\\cpu_instrs\\individual\\11-op a,(hl).gb");





	// Setup before we start running
	auto startTime = chrono::steady_clock::now();
	auto nextFrame = startTime + 16.75041876ms;			// TODO: Is this the correct ms for a gameboy frame? Can we save this in global variable?
	
	printf("\n+++ Pre-Execution +++\n");
	printf("Entry Point (%02X-%02X-%02X-%02X)\n\n", gbMem.readByte(256), gbMem.readByte(257), gbMem.readByte(258), gbMem.readByte(259));

	// Main Game Loop
	while (gbGpu.isOpen())
	{
		sf::Event gpuEvent;
		while (gbGpu.pollEvent(gpuEvent)) {
			//printf("Main.gbGpu event caught: (%s)\n", gpuEvent.text);
			if (gpuEvent.type == sf::Event::Closed)
				gbGpu.close();
		}

		doStuff();
		gbGpu.renderFrame();
		this_thread::sleep_until(nextFrame);
		nextFrame += 16.75041876ms;
	}

	return 0;
}






/*		NOTES

FF0F - IF - Interrupt Flag (R/W)
Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
When an interrupt signal changes from low to high, then the corresponding bit in the IF register becomes set.
For example, Bit 0 becomes set when the LCD controller enters into the V-Blank period.

Interrupt Execution
When an interrupt gets executed, the corresponding bit in the IF register becomes automatically reset by the CPU,
and the IME flag becomes cleared (disabeling any further interrupts until the program re-enables the interrupts, typically by using the RETI instruction),
and the corresponding Interrupt Vector (that are the addresses in range 0040h-0060h, as shown in IE and IF register decriptions above) becomes called.

LCD Interrupts
INT 40 - V-Blank Interrupt
The V-Blank interrupt occurs ca. 59.7 times a second on a regular GB and ca. 61.1 times a second on a Super GB (SGB). This interrupt occurs at the beginning of the V-Blank period (LY=144).
During this period video hardware is not using video ram so it may be freely accessed. This period lasts approximately 1.1 milliseconds.
INT 48 - LCDC Status Interrupt
There are various reasons for this interrupt to occur as described by the STAT register ($FF40).
One very popular reason is to indicate to the user when the video hardware is about to redraw a given LCD line.
This can be useful for dynamically controlling the SCX/SCY registers ($FF43/$FF42) to perform special video effects.

FF41 - STAT - LCDC Status (R/W)
Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
Bit 1-0 - Mode Flag       (Mode 0-3, see below) (Read Only)
0: During H-Blank
1: During V-Blank
2: During Searching OAM-RAM
3: During Transfering Data to LCD Driver

The two lower STAT bits show the current status of the LCD controller.
Mode 0: The LCD controller is in the H-Blank period and
the CPU can access both the display RAM (8000h-9FFFh)
and OAM (FE00h-FE9Fh)

Mode 1: The LCD contoller is in the V-Blank period (or the
display is disabled) and the CPU can access both the
display RAM (8000h-9FFFh) and OAM (FE00h-FE9Fh)

Mode 2: The LCD controller is reading from OAM memory.
The CPU <cannot> access OAM memory (FE00h-FE9Fh)
during this period.

Mode 3: The LCD controller is reading from both OAM and VRAM,
The CPU <cannot> access OAM and VRAM during this period.
CGB Mode: Cannot access Palette Data (FF69,FF6B) either.

The following are typical when the display is enabled:
Mode 2  2_____2_____2_____2_____2_____2___________________2____
Mode 3  _33____33____33____33____33____33__________________3___
Mode 0  ___000___000___000___000___000___000________________000
Mode 1  ____________________________________11111111111111_____

The Mode Flag goes through the values 0, 2, and 3 at a cycle of about 109uS. 0 is present about 48.6uS, 2 about 19uS, and 3 about 41uS. This is interrupted every 16.6ms by the VBlank (1). The mode flag stays set at 1 for about 1.08 ms.

Mode 0 is present between 201-207 clks, 2 about 77-83 clks, and 3 about 169-175 clks. A complete cycle through these states takes 456 clks. VBlank lasts 4560 clks. A complete screen refresh occurs every 70224 clks.)

*/





/*		TODO: The following is source from I don't know where.

std::pair<sf::Image, bool> GameBoy::RenderFrame() {

	bool running = (input.PollEvents())?true:false;

	cpu.frame_clock = cpu.clock + 17556; // Number of cycles/4 for one frame before v-blank

	bool v_blank = false;

	do {
		if (cpu.halt) {
			cpu.clock += 1;
		} else {
			cpu.ExecuteNextInstruction();
		}

		uint8_t if_memory_value = mmu.ReadByte(0xFF0F);
		if (mmu.interrupt_enable and cpu.interrupt_master_enable and if_memory_value) {
			cpu.halt = 0;
			cpu.interrupt_master_enable = 0;
			uint8_t interrupt_fired = mmu.interrupt_enable & if_memory_value;

			if (interrupt_fired & 0x01) {if_memory_value &= 0XFE; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST40();}
			else if (interrupt_fired & 0x02) {if_memory_value &= 0XFD; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST48();}
			else if (interrupt_fired & 0x04) {if_memory_value &= 0XFB; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST50();}
			else if (interrupt_fired & 0x08) {if_memory_value &= 0XF7; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST58();}
			else if (interrupt_fired & 0x10) {if_memory_value &= 0XEF; mmu.WriteByte(0xFF0F, if_memory_value); cpu.RST60();}
			else {cpu.interrupt_master_enable = 1;}

			mmu.WriteByte(0xFF0F, if_memory_value);
		}

		timer.Increment();
	} while(cpu.clock < cpu.frame_clock);




	if (!v_blank) {
		frame = display.RenderFrame();
	}

	return std::make_pair(frame, running);
}

void Timer::Increment() {
	auto cycles = cpu->clock - clock; // Difference in clocks

	divider_clock_tracker += cycles;
	if (divider_clock_tracker >= 64) {
		++divider_clock;
		divider_clock_tracker -= 64;
	}

	// If counter is active, increment based on clock select
	if (mmu->ReadByte(0xFF07)&0x04) {
		unsigned int counter_increment_count;
		if ((mmu->ReadByte(0xFF07)&0x03) == 0x0) {
			counter_increment_count = 256;
		} else if ((mmu->ReadByte(0xFF07)&0x03) == 0x1) {
			counter_increment_count = 4;
		} else if ((mmu->ReadByte(0xFF07)&0x03) == 0x2) {
			counter_increment_count = 16;
		} else {
			counter_increment_count = 64;
		}

		counter_clock_tracker += cycles;
		if (counter_clock_tracker > counter_increment_count) {
			if (counter_clock == 0xFF) {
				// Overflow to occur, set interrupt flag
				mmu->interrupt_flag |= 0x04;
				counter_clock = mmu->ReadByte(0xFF06);
			} else {
				++counter_clock;
			}

			counter_clock_tracker -= counter_increment_count;
		}
	}

	// Also keep track of scanline (0xFF44)
	uint8_t lcd_control = mmu->zram[0xFF40&0xFF];
	if (lcd_control & 0x80) {
		// Only update scanline if lcd is enabled
		scanline_tracker += cycles;
		if (scanline_tracker >= 456/4) { // Check if this is >=  or just >
			++scanline;
			scanline_tracker -= 456/4;
			if (scanline < 144) {
				display->RenderScanline(scanline);
			} else if (scanline > 153) {
				display->RenderScanline(0);
			}
		}
		if (scanline > 153) {
			scanline = 0;
			v_blank_triggered = false;
		}
	} else {
		scanline = 0;
		scanline_tracker = 0;
	}

	// Update LCD Status based on scanline and timing
	uint8_t lcd_status = mmu->zram[0xFF41&0xFF] & 0xF8; // Last 3 bits are updated here
	if (scanline >= 144) {
		// V-Blank
		lcd_status |= 0b01;
		if (!v_blank_triggered) {
			mmu->interrupt_flag |= 0x01;
			v_blank_triggered = true;
		}
	} else if (scanline_tracker > (80+172)/4 ) {
		// H-Blank
		lcd_status |= 0b00;
	} else if (scanline_tracker > 80/4) {
		// Scanline accessing VRAM
		lcd_status |= 0b11;
	} else {
		// Scanline accessing OAM
		lcd_status |= 0b10;
	}

	// Finally, check if 0xFF45 matches scanline and set bit 2 of 0xFF41 and possibly trigger interrupt
	uint8_t ly_compare = mmu->zram[0xFF45&0xFF];
	if (ly_compare == scanline) {
		lcd_status |= 0b100;
		mmu->interrupt_flag |= 0x02;
	}

	// Update timers
	//mmu->zram[0x04] = divider_clock;
	//mmu->zram[0x05] = counter_clock;
	mmu->zram[0xFF41&0xFF] = lcd_status;
	//mmu->zram[0xFF44&0xFF] = scanline;

	clock = cpu->clock;
}

int main(int argc, char* argv[]) {

	window.create(sf::VideoMode(160, 144), "GBS");
	GameBoy gameboy(window);
	gameboy.LoadGame(game_name, save_file);

	bool running = true;
	auto start_time = std::chrono::steady_clock::now();
	auto next_frame = start_time + 16.75041876ms;
	while(running) {
		auto result = gameboy.RenderFrame();
		DrawFrame(result.first);
		running = result.second;
		std::this_thread::sleep_until(next_frame);
		next_frame += 16.75041876ms;
	}
}

void gbEmulate(int ticksToStop)
{
	int clockTicks = 0;
	gbDmaTicks = 0;
	register int opcode = 0;

	while(1)
	{
		if(IFF & 0x80)
		{
			if(register_LCDC & 0x80) {
				clockTicks = gbLcdTicks;
			}
			else
				clockTicks = 100;

			if(gbLcdMode == 1 && (gbLcdLYIncrementTicks < clockTicks))
				clockTicks = gbLcdLYIncrementTicks;

			if(gbTimerOn && (gbTimerTicks < clockTicks))
				clockTicks = gbTimerTicks;

			if(soundTicks && (soundTicks < clockTicks))
				clockTicks = soundTicks;
		}
		else {
			opcode = gbReadOpcode(PC.W++);

			if(IFF & 0x100) {
				IFF &= 0xff;
				PC.W--;
			}

			clockTicks = gbCycles[opcode];

			switch(opcode) {
				case 0xCB:
					// extended opcode
					opcode = gbReadOpcode(PC.W++);
					clockTicks = gbCyclesCB[opcode];
					switch(opcode) {
						#include "gbCodesCB.h"
					}
					break;

				#include "gbCodes.h"
			}
		}

		if(!emulating)
			return;

		if(gbDmaTicks) {
			clockTicks += gbDmaTicks;
			gbDmaTicks = 0;
		}

		if(gbSgbMode) {
			if(gbSgbPacketTimeout) {
				gbSgbPacketTimeout -= clockTicks;

				if(gbSgbPacketTimeout <= 0)
					gbSgbResetPacketState();
			}
		}

		ticksToStop -= clockTicks;

		// DIV register emulation
		gbDivTicks -= clockTicks;
		while(gbDivTicks <= 0) {
			register_DIV++;
			gbDivTicks += GBDIV_CLOCK_TICKS;
		}

		if(register_LCDC & 0x80) {
			// LCD stuff
			gbLcdTicks -= clockTicks;
			if(gbLcdMode == 1) {
				// during V-BLANK,we need to increment LY at the same rate!
				gbLcdLYIncrementTicks -= clockTicks;
				while(gbLcdLYIncrementTicks <= 0) {
					gbLcdLYIncrementTicks += GBLY_INCREMENT_CLOCK_TICKS;

					if(register_LY < 153) {
						register_LY++;
						gbCompareLYToLYC();

						if(register_LY >= 153)
							gbLcdLYIncrementTicks = 6;
					}
					else {
						register_LY = 0x00;
						// reset the window line
						gbWindowLine = -1;
						gbLcdLYIncrementTicks = GBLY_INCREMENT_CLOCK_TICKS * 2;
						gbCompareLYToLYC();
					}
				}
			}

			// our counter is off, see what we need to do
			while(gbLcdTicks <= 0) {
				int framesToSkip = systemFrameSkip;
				if(speedup)
					framesToSkip = 9; // try 6 FPS during speedup

				switch(gbLcdMode) {
					case 0:
						// H-Blank
						register_LY++;
						gbCompareLYToLYC();

						// check if we reached the V-Blank period
						if(register_LY == 144) {
							// Yes, V-Blank
							// set the LY increment counter
							gbLcdLYIncrementTicks = gbLcdTicks + GBLY_INCREMENT_CLOCK_TICKS;
							gbLcdTicks += GBLCD_MODE_1_CLOCK_TICKS;
							gbLcdMode = 1;
							if(register_LCDC & 0x80) {
								gbInterrupt |= 1; // V-Blank interrupt
								gbInterruptWait = 6;
								if(register_STAT & 0x10)
								gbInterrupt |= 2;
							}

							gbFrameCount++;
							systemFrame();

							if((gbFrameCount % 10) == 0)
								system10Frames(60);

							if(gbFrameCount >= 60) {
								u32 currentTime = systemGetClock();
								if(currentTime != gbLastTime)
									systemShowSpeed(100000/(currentTime - gbLastTime));
								else
									systemShowSpeed(0);
								gbLastTime = currentTime;
								gbFrameCount = 0;
							}

							if(systemReadJoypads()) {
								// read joystick
								gbJoymask[0] = systemReadJoypad(-1);
							}

							int newmask = gbJoymask[0] & 255;

							if(gbRom[0x147] == 0x22) {
								systemUpdateMotionSensor();
							}

							if(newmask) {
								gbInterrupt |= 16;
							}

							newmask = (gbJoymask[0] >> 10);
							speedup = (newmask & 1) ? true : false;

							if(gbFrameSkipCount >= framesToSkip) {
								systemDrawScreen();
								gbFrameSkipCount = 0;
							}
							else
								gbFrameSkipCount++;
						}
						else {
							// go the the OAM being accessed mode
							gbLcdTicks += GBLCD_MODE_2_CLOCK_TICKS;
							gbLcdMode = 2;

							// only one LCD interrupt per line. may need to generalize...
							if(!(register_STAT & 0x40) || (register_LY != register_LYC)) {
								if((register_STAT & 0x28) == 0x20)
									gbInterrupt |= 2;
							}
						}
						break;

					case 1:
						// V-Blank
						// next mode is OAM being accessed mode
						gbLcdTicks += GBLCD_MODE_2_CLOCK_TICKS;
						gbLcdMode = 2;
						if(!(register_STAT & 0x40) || (register_LY != register_LYC)) {
							if((register_STAT & 0x28) == 0x20)
								gbInterrupt |= 2;
						}
						break;

					case 2:
						// OAM being accessed mode
						// next mode is OAM and VRAM in use
						gbLcdTicks += GBLCD_MODE_3_CLOCK_TICKS;
						gbLcdMode = 3;
						break;

					case 3:
						// OAM and VRAM in use
						// next mode is H-Blank
						if(register_LY < 144) {
							if(!gbSgbMask) {
								if(gbFrameSkipCount >= framesToSkip) {
									gbRenderLine();
									gbDrawSprites();
								}
							}
						}
						gbLcdTicks += GBLCD_MODE_0_CLOCK_TICKS;
						gbLcdMode = 0;
						// only one LCD interrupt per line. may need to generalize...
						if(!(register_STAT & 0x40) || (register_LY != register_LYC)) {
							if(register_STAT & 0x08)
								gbInterrupt |= 2;
						}

						if(gbHdmaOn) {
							gbDoHdma();
						}
						break;
				}

				// mark the correct lcd mode on STAT register
				register_STAT = (register_STAT & 0xfc) | gbLcdMode;
			}
		}

		// timer emulation
		if(gbTimerOn) {
			gbTimerTicks -= clockTicks;

			while(gbTimerTicks <= 0) {
				register_TIMA++;

				if(register_TIMA == 0) {
					// timer overflow!
					// reload timer modulo
					register_TIMA = register_TMA;

					// flag interrupt
					gbInterrupt |= 4;
				}

				gbTimerTicks += gbTimerClockTicks;
			}
		}

		soundTicks -= clockTicks;
		while(soundTicks < 0) {
			soundTicks += SOUND_CLOCK_TICKS;
			gbSoundTick();
		}

		register_IF = gbInterrupt;

		if(IFF & 0x20) {
			IFF &= 0xdf;
			IFF |= 0x01;
			gbInterruptWait = 0;
		}
		else if(gbInterrupt) {
			if(gbInterruptWait == 0) {

				if(IFF & 0x01) {
					if((gbInterrupt & 1) && (register_IE & 1)) {
						gbVblank_interrupt();
						continue;
					}

					if((gbInterrupt & 2) && (register_IE & 2)) {
						gbLcd_interrupt();
						continue;
					}

					if((gbInterrupt & 4) && (register_IE & 4)) {
						gbTimer_interrupt();
						continue;
					}

					if((gbInterrupt & 8) && (register_IE & 8)) {
						gbSerial_interrupt();
						continue;
					}

					if((gbInterrupt & 16) && (register_IE & 16)) {
						gbJoypad_interrupt();
						continue;
					}
				}
			}
			else {
				gbInterruptWait -= clockTicks;
				if(gbInterruptWait < 0)
					gbInterruptWait = 0;
			}
		}

		if(ticksToStop <= 0) {
			if(!(register_LCDC & 0x80)) {
				if(systemReadJoypads()) {
					// read joystick
					gbJoymask[0] = systemReadJoypad(-1);
				}
			}
			return;
		}
	}
}
*/
