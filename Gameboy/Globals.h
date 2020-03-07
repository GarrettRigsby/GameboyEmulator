#pragma once

typedef unsigned char BIT8;
typedef unsigned short int BIT16;










/******
NOTES:
https://github.com/jgilchrist/gbemu

http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
http://problemkaputt.de/pandocs.htm#videodisplay
http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics
*******/





/*****
Sound Overview

There are two sound channels connected to the output terminals SO1 and SO2. There is also a input terminal Vin connected to the cartridge. It can be routed to either of both output terminals. GameBoy circuitry allows producing sound in four different ways:

   Quadrangular wave patterns with sweep and envelope functions.
   Quadrangular wave patterns with envelope functions.
   Voluntary wave patterns from wave RAM.
   White noise with an envelope function.

These four sounds can be controlled independantly and then mixed separately for each of the output terminals.

Sound registers may be set at all times while producing sound.

(Sounds will have a 2.4% higher frequency on Super GB.)


Sound Channel 1 - Tone & Sweep

FF10 - NR10 - Channel 1 Sweep register (R/W)
  Bit 6-4 - Sweep Time
  Bit 3   - Sweep Increase/Decrease
			 0: Addition    (frequency increases)
			 1: Subtraction (frequency decreases)
  Bit 2-0 - Number of sweep shift (n: 0-7)
Sweep Time:
  000: sweep off - no freq change
  001: 7.8 ms  (1/128Hz)
  010: 15.6 ms (2/128Hz)
  011: 23.4 ms (3/128Hz)
  100: 31.3 ms (4/128Hz)
  101: 39.1 ms (5/128Hz)
  110: 46.9 ms (6/128Hz)
  111: 54.7 ms (7/128Hz)

The change of frequency (NR13,NR14) at each shift is calculated by the following formula where X(0) is initial freq & X(t-1) is last freq:
  X(t) = X(t-1) +/- X(t-1)/2^n

FF11 - NR11 - Channel 1 Sound length/Wave pattern duty (R/W)
  Bit 7-6 - Wave Pattern Duty (Read/Write)
  Bit 5-0 - Sound length data (Write Only) (t1: 0-63)
Wave Duty:
  00: 12.5% ( _-------_-------_------- )
  01: 25%   ( __------__------__------ )
  10: 50%   ( ____----____----____---- ) (normal)
  11: 75%   ( ______--______--______-- )
Sound Length = (64-t1)*(1/256) seconds
The Length value is used only if Bit 6 in NR14 is set.

FF12 - NR12 - Channel 1 Volume Envelope (R/W)
  Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
  Bit 3   - Envelope Direction (0=Decrease, 1=Increase)
  Bit 2-0 - Number of envelope sweep (n: 0-7)
			(If zero, stop envelope operation.)
Length of 1 step = n*(1/64) seconds

FF13 - NR13 - Channel 1 Frequency lo (Write Only)

Lower 8 bits of 11 bit frequency (x).
Next 3 bit are in NR14 ($FF14)

FF14 - NR14 - Channel 1 Frequency hi (R/W)
  Bit 7   - Initial (1=Restart Sound)     (Write Only)
  Bit 6   - Counter/consecutive selection (Read/Write)
			(1=Stop output when length in NR11 expires)
  Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
Frequency = 131072/(2048-x) Hz


 Sound Channel 2 - Tone

This sound channel works exactly as channel 1, except that it doesn't have a Tone Envelope/Sweep Register.

FF16 - NR21 - Channel 2 Sound Length/Wave Pattern Duty (R/W)
  Bit 7-6 - Wave Pattern Duty (Read/Write)
  Bit 5-0 - Sound length data (Write Only) (t1: 0-63)
Wave Duty:
  00: 12.5% ( _-------_-------_------- )
  01: 25%   ( __------__------__------ )
  10: 50%   ( ____----____----____---- ) (normal)
  11: 75%   ( ______--______--______-- )
Sound Length = (64-t1)*(1/256) seconds
The Length value is used only if Bit 6 in NR24 is set.

FF17 - NR22 - Channel 2 Volume Envelope (R/W)
  Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
  Bit 3   - Envelope Direction (0=Decrease, 1=Increase)
  Bit 2-0 - Number of envelope sweep (n: 0-7)
			(If zero, stop envelope operation.)
Length of 1 step = n*(1/64) seconds

FF18 - NR23 - Channel 2 Frequency lo data (W)
Frequency's lower 8 bits of 11 bit data (x).
Next 3 bits are in NR24 ($FF19).

FF19 - NR24 - Channel 2 Frequency hi data (R/W)
  Bit 7   - Initial (1=Restart Sound)     (Write Only)
  Bit 6   - Counter/consecutive selection (Read/Write)
			(1=Stop output when length in NR21 expires)
  Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
Frequency = 131072/(2048-x) Hz


Sound Channel 3 - Wave Output

This channel can be used to output digital sound, the length of the sample buffer (Wave RAM) is limited to 32 digits. This sound channel can be also used to output normal tones when initializing the Wave RAM by a square wave. This channel doesn't have a volume envelope register.

FF1A - NR30 - Channel 3 Sound on/off (R/W)
  Bit 7 - Sound Channel 3 Off  (0=Stop, 1=Playback)  (Read/Write)

FF1B - NR31 - Channel 3 Sound Length
  Bit 7-0 - Sound length (t1: 0 - 255)
Sound Length = (256-t1)*(1/256) seconds
This value is used only if Bit 6 in NR34 is set.

FF1C - NR32 - Channel 3 Select output level (R/W)
  Bit 6-5 - Select output level (Read/Write)
Possible Output levels are:
  0: Mute (No sound)
  1: 100% Volume (Produce Wave Pattern RAM Data as it is)
  2:  50% Volume (Produce Wave Pattern RAM data shifted once to the right)
  3:  25% Volume (Produce Wave Pattern RAM data shifted twice to the right)

FF1D - NR33 - Channel 3 Frequency's lower data (W)
Lower 8 bits of an 11 bit frequency (x).

FF1E - NR34 - Channel 3 Frequency's higher data (R/W)
  Bit 7   - Initial (1=Restart Sound)     (Write Only)
  Bit 6   - Counter/consecutive selection (Read/Write)
			(1=Stop output when length in NR31 expires)
  Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
Frequency = 4194304/(64*(2048-x)) Hz = 65536/(2048-x) Hz

FF30-FF3F - Wave Pattern RAM
Contents - Waveform storage for arbitrary sound data

This storage area holds 32 4-bit samples that are played back upper 4 bits first.


Sound Channel 4 - Noise

This channel is used to output white noise. This is done by randomly switching the amplitude between high and low at a given frequency. Depending on the frequency the noise will appear 'harder' or 'softer'.

It is also possible to influence the function of the random generator, so the that the output becomes more regular, resulting in a limited ability to output Tone instead of Noise.

FF20 - NR41 - Channel 4 Sound Length (R/W)
  Bit 5-0 - Sound length data (t1: 0-63)
Sound Length = (64-t1)*(1/256) seconds
The Length value is used only if Bit 6 in NR44 is set.

FF21 - NR42 - Channel 4 Volume Envelope (R/W)
  Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
  Bit 3   - Envelope Direction (0=Decrease, 1=Increase)
  Bit 2-0 - Number of envelope sweep (n: 0-7)
			(If zero, stop envelope operation.)
Length of 1 step = n*(1/64) seconds

FF22 - NR43 - Channel 4 Polynomial Counter (R/W)
The amplitude is randomly switched between high and low at the given frequency. A higher frequency will make the noise to appear 'softer'.
When Bit 3 is set, the output will become more regular, and some frequencies will sound more like Tone than Noise.
  Bit 7-4 - Shift Clock Frequency (s)
  Bit 3   - Counter Step/Width (0=15 bits, 1=7 bits)
  Bit 2-0 - Dividing Ratio of Frequencies (r)
Frequency = 524288 Hz / r / 2^(s+1) ;For r=0 assume r=0.5 instead

FF23 - NR44 - Channel 4 Counter/consecutive; Inital (R/W)
  Bit 7   - Initial (1=Restart Sound)     (Write Only)
  Bit 6   - Counter/consecutive selection (Read/Write)
			(1=Stop output when length in NR41 expires)



Sound Control Registers

FF24 - NR50 - Channel control / ON-OFF / Volume (R/W)
The volume bits specify the "Master Volume" for Left/Right sound output.
  Bit 7   - Output Vin to SO2 terminal (1=Enable)
  Bit 6-4 - SO2 output level (volume)  (0-7)
  Bit 3   - Output Vin to SO1 terminal (1=Enable)
  Bit 2-0 - SO1 output level (volume)  (0-7)
The Vin signal is received from the game cartridge bus, allowing external hardware in the cartridge to supply a fifth sound channel, additionally to the gameboys internal four channels. As far as I know this feature isn't used by any existing games.

FF25 - NR51 - Selection of Sound output terminal (R/W)
  Bit 7 - Output sound 4 to SO2 terminal
  Bit 6 - Output sound 3 to SO2 terminal
  Bit 5 - Output sound 2 to SO2 terminal
  Bit 4 - Output sound 1 to SO2 terminal
  Bit 3 - Output sound 4 to SO1 terminal
  Bit 2 - Output sound 3 to SO1 terminal
  Bit 1 - Output sound 2 to SO1 terminal
  Bit 0 - Output sound 1 to SO1 terminal

FF26 - NR52 - Sound on/off
If your GB programs don't use sound then write 00h to this register to save 16% or more on GB power consumption. Disabeling the sound controller by clearing Bit 7 destroys the contents of all sound registers. Also, it is not possible to access any sound registers (execpt FF26) while the sound controller is disabled.
  Bit 7 - All sound on/off  (0: stop all sound circuits) (Read/Write)
  Bit 3 - Sound 4 ON flag (Read Only)
  Bit 2 - Sound 3 ON flag (Read Only)
  Bit 1 - Sound 2 ON flag (Read Only)
  Bit 0 - Sound 1 ON flag (Read Only)
Bits 0-3 of this register are read only status bits, writing to these bits does NOT enable/disable sound. The flags get set when sound output is restarted by setting the Initial flag (Bit 7 in NR14-NR44), the flag remains set until the sound length has expired (if enabled). A volume envelopes which has decreased to zero volume will NOT cause the sound flag to go off.
******/





/*****
 Joypad Input

FF00 - P1/JOYP - Joypad (R/W)
The eight gameboy buttons/direction keys are arranged in form of a 2x4 matrix. Select either button or direction keys by writing to this register, then read-out bit 0-3.
  Bit 7 - Not used
  Bit 6 - Not used
  Bit 5 - P15 Select Button Keys      (0=Select)
  Bit 4 - P14 Select Direction Keys   (0=Select)
  Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
  Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
  Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
  Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
Note: Most programs are repeatedly reading from this port several times (the first reads used as short delay, allowing the inputs to stabilize, and only the value from the last read actually used).

Usage in SGB software
Beside for normal joypad input, SGB games mis-use the joypad register to output SGB command packets to the SNES, also, SGB programs may read out gamepad states from up to four different joypads which can be connected to the SNES.
See SGB description for details.

INT 60 - Joypad Interrupt
Joypad interrupt is requested when any of the above Input lines changes from High to Low. Generally this should happen when a key becomes pressed (provided that the button/direction key is enabled by above Bit4/5), however, because of switch bounce, one or more High to Low transitions are usually produced both when pressing or releasing a key.

Using the Joypad Interrupt
It's more or less useless for programmers, even when selecting both buttons and direction keys simultaneously it still cannot recognize all keystrokes, because in that case a bit might be already held low by a button key, and pressing the corresponding direction key would thus cause no difference. The only meaningful purpose of the keystroke interrupt would be to terminate STOP (low power) standby state.
Also, the joypad interrupt does not appear to work with CGB and GBA hardware (the STOP function can be still terminated by joypad keystrokes though).
*****/





/*****
Serial Data Transfer (Link Cable)

FF01 - SB - Serial transfer data (R/W)
8 Bits of data to be read/written

FF02 - SC - Serial Transfer Control (R/W)
  Bit 7 - Transfer Start Flag (0=No Transfer, 1=Start)
  Bit 1 - Clock Speed (0=Normal, 1=Fast) ** CGB Mode Only **
  Bit 0 - Shift Clock (0=External Clock, 1=Internal Clock)
The clock signal specifies the rate at which the eight data bits in SB (FF01) are transferred. When the gameboy is communicating with another gameboy (or other computer) then either one must supply internal clock, and the other one must use external clock.

Internal Clock
In Non-CGB Mode the gameboy supplies an internal clock of 8192Hz only (allowing to transfer about 1 KByte per second). In CGB Mode four internal clock rates are available, depending on Bit 1 of the SC register, and on whether the CGB Double Speed Mode is used:
	8192Hz -  1KB/s - Bit 1 cleared, Normal
   16384Hz -  2KB/s - Bit 1 cleared, Double Speed Mode
  262144Hz - 32KB/s - Bit 1 set,     Normal
  524288Hz - 64KB/s - Bit 1 set,     Double Speed Mode

External Clock
The external clock is typically supplied by another gameboy, but might be supplied by another computer (for example if connected to a PCs parallel port), in that case the external clock may have any speed. Even the old/monochrome gameboy is reported to recognizes external clocks of up to 500KHz. And there is no limitiation into the other direction - even when suppling an external clock speed of "1 bit per month", then the gameboy will still eagerly wait for the next bit(s) to be transferred. It isn't required that the clock pulses are sent at an regular interval either.

Timeouts
When using external clock then the transfer will not complete until the last bit is received. In case that the second gameboy isn't supplying a clock signal, if it gets turned off, or if there is no second gameboy connected at all) then transfer will never complete. For this reason the transfer procedure should use a timeout counter, and abort the communication if no response has been received during the timeout interval.

Delays and Synchronization
The gameboy that is using internal clock should always execute a small delay between each transfer, in order to ensure that the opponent gameboy has enough time to prepare itself for the next transfer, ie. the gameboy with external clock must have set its transfer start bit before the gameboy with internal clock starts the transfer. Alternately, the two gameboys could switch between internal and external clock for each transferred byte to ensure synchronization.

Transfer is initiated by setting the Transfer Start Flag. This bit is automatically set to 0 at the end of Transfer. Reading this bit can be used to determine if the transfer is still active.

INT 58 - Serial Interrupt
When the transfer has completed (ie. after sending/receiving 8 bits, if any) then an interrupt is requested by setting Bit 3 of the IF Register (FF0F). When that interrupt is enabled, then the Serial Interrupt vector at 0058 is called.

XXXXXX...

Transmitting and receiving serial data is done simultaneously. The received data is automatically stored in SB.

The serial I/O port on the Gameboy is a very simple setup and is crude compared to standard RS-232 (IBM-PC) or RS-485 (Macintosh) serial ports. There are no start or stop bits.

During a transfer, a byte is shifted in at the same time that a byte is shifted out. The rate of the shift is determined by whether the clock source is internal or external.
The most significant bit is shifted in and out first.

When the internal clock is selected, it drives the clock pin on the game link port and it stays high when not used. During a transfer it will go low eight times to clock in/out each bit.

The state of the last bit shifted out determines the state of the output line until another transfer takes place.

If a serial transfer with internal clock is performed and no external GameBoy is present, a value of $FF will be received in the transfer.

The following code causes $75 to be shifted out the serial port and a byte to be shifted into $FF01:

	ld   a,$75
	ld  ($FF01),a
	ld   a,$81
	ld  ($FF02),a

******/





/*****
Timer and Divider Registers

FF04 - DIV - Divider Register (R/W)
This register is incremented at rate of 16384Hz (~16779Hz on SGB). In CGB Double Speed Mode it is incremented twice as fast, ie. at 32768Hz. Writing any value to this register resets it to 00h.

FF05 - TIMA - Timer counter (R/W)
This timer is incremented by a clock frequency specified by the TAC register ($FF07). When the value overflows (gets bigger than FFh) then it will be reset to the value specified in TMA (FF06), and an interrupt will be requested, as described below.

FF06 - TMA - Timer Modulo (R/W)
When the TIMA overflows, this data will be loaded.

FF07 - TAC - Timer Control (R/W)
  Bit 2    - Timer Stop  (0=Stop, 1=Start)
  Bits 1-0 - Input Clock Select
			 00:   4096 Hz    (~4194 Hz SGB)
			 01: 262144 Hz  (~268400 Hz SGB)
			 10:  65536 Hz   (~67110 Hz SGB)
			 11:  16384 Hz   (~16780 Hz SGB)

INT 50 - Timer Interrupt
Each time when the timer overflows (ie. when TIMA gets bigger than FFh), then an interrupt is requested by setting Bit 2 in the IF Register (FF0F). When that interrupt is enabled, then the CPU will execute it by calling the timer interrupt vector at 0050h.

Note
The above described Timer is the built-in timer in the gameboy. It has nothing to do with the MBC3s battery buffered Real Time Clock - that's a completely different thing, described in the chapter about Memory Banking Controllers.
*****/





/*****
Memory Bank Controllers

As the gameboys 16 bit address bus offers only limited space for ROM and RAM addressing, many games are using Memory Bank Controllers (MBCs) to expand the
available address space by bank switching. These MBC chips are located in the game cartridge (ie. not in the gameboy itself), several different MBC types are available:
	None (32KByte ROM only)
	MBC1 (max 2MByte ROM and/or 32KByte RAM)
	MBC2 (max 256KByte ROM and 512x4 bits RAM)
	MBC3 (max 2MByte ROM and/or 32KByte RAM and Timer)
	HuC1 (MBC with Infrared Controller)

MBC Timing Issues
In each cartridge, the required (or preferred) MBC type should be specified in byte at 0147h of the ROM. (As described in the chapter about The Cartridge Header.)

None (32KByte ROM only)
	Small games of not more than 32KBytes ROM do not require a MBC chip for ROM banking. The ROM is directly mapped to memory at 0000-7FFFh.
	Optionally up to 8KByte of RAM could be connected at A000-BFFF, even though that could require a tiny MBC-like circuit, but no real MBC chip.


MBC1 (max 2MByte ROM and/or 32KByte RAM)
	This is the first MBC chip for the gameboy. Any newer MBC chips are working similiar, so that is relative easy to upgrade a program from one MBC chip to another
	- or even to make it compatible to several different types of MBCs.

	Note that the memory in range 0000-7FFF is used for both reading from ROM, and for writing to the MBCs Control Registers.

	0000-3FFF - ROM Bank 00 (Read Only)
		This area always contains the first 16KBytes of the cartridge ROM.

	4000-7FFF - ROM Bank 01-7F (Read Only)
		This area may contain any of the further 16KByte banks of the ROM, allowing to address up to 125 ROM Banks (almost 2MByte).
		As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.

	A000-BFFF - RAM Bank 00-03, if any (Read/Write)
		This area is used to address external RAM in the cartridge (if any). External RAM is often battery buffered, allowing to store game positions or high score tables,
		even if the gameboy is turned off, or if the cartridge is removed from the gameboy. Available RAM sizes are: 2KByte (at A000-A7FF),
		8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).

	0000-1FFF - RAM Enable (Write Only)
		Before external RAM can be read or written, it must be enabled by writing to this address space. It is recommended to disable external RAM after accessing it,
		in order to protect its contents from damage during power down of the gameboy. Usually the following values are used:
			00h  Disable RAM (default)
			0Ah  Enable RAM
		Practically any value with 0Ah in the lower 4 bits enables RAM, and any other value disables RAM.

	2000-3FFF - ROM Bank Number (Write Only)
		Writing to this address space selects the lower 5 bits of the ROM Bank Number (in range 01-1Fh). When 00h is written, the MBC translates that to bank 01h also.
		That doesn't harm so far, because ROM Bank 00h can be always directly accessed by reading from 0000-3FFF. But (when using the register below to specify the upper ROM Bank bits),
		the same happens for Bank 20h, 40h, and 60h. Any attempt to address these ROM Banks will select Bank 21h, 41h, and 61h instead.

	4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
		This 2bit register can be used to select a RAM Bank in range from 00-03h, or to specify the upper two bits (Bit 5-6) of the ROM Bank number,
		depending on the current ROM/RAM Mode. (See below.)

	6000-7FFF - ROM/RAM Mode Select (Write Only)
		This 1bit Register selects whether the two bits of the above register should be used as upper two bits of the ROM Bank, or as RAM Bank Number.
			00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)
			01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)
		The program may freely switch between both modes, the only limitiation is that only RAM Bank 00h can be used during Mode 0, and only ROM Banks 00-1Fh can be used during Mode 1.


MBC2 (max 256KByte ROM and 512x4 bits RAM)

	0000-3FFF - ROM Bank 00 (Read Only)
		Same as for MBC1.

	4000-7FFF - ROM Bank 01-0F (Read Only)
		Same as for MBC1, but only a total of 16 ROM banks is supported.

	A000-A1FF - 512x4bits RAM, built-in into the MBC2 chip (Read/Write)
		The MBC2 doesn't support external RAM, instead it includes 512x4 bits of built-in RAM (in the MBC2 chip itself). It still requires an external battery to save data during power-off though.
		As the data consists of 4bit values, only the lower 4 bits of the "bytes" in this memory area are used.

	0000-1FFF - RAM Enable (Write Only)
		The least significant bit of the upper address byte must be zero to enable/disable cart RAM. For example the following addresses can be used to
		enable/disable cart RAM: 0000-00FF, 0200-02FF, 0400-04FF, ..., 1E00-1EFF. The suggested address range to use for MBC2 ram enable/disable is 0000-00FF.

	2000-3FFF - ROM Bank Number (Write Only)
		Writing a value (XXXXBBBB - X = Don't cares, B = bank select bits) into 2000-3FFF area will select an appropriate ROM bank at 4000-7FFF.

	The least significant bit of the upper address byte must be one to select a ROM bank. For example the following addresses can be used to select a ROM bank: 2100-21FF, 2300-23FF, 2500-25FF, ..., 3F00-3FFF.
	The suggested address range to use for MBC2 rom bank selection is 2100-21FF.


MBC3 (max 2MByte ROM and/or 32KByte RAM and Timer)

	Beside for the ability to access up to 2MB ROM (128 banks), and 32KB RAM (4 banks), the MBC3 also includes a built-in Real Time Clock (RTC). The RTC requires an external 32.768 kHz Quartz Oscillator, and an external battery (if it should continue to tick when the gameboy is turned off).

	0000-3FFF - ROM Bank 00 (Read Only)
		Same as for MBC1.

	4000-7FFF - ROM Bank 01-7F (Read Only)
		Same as for MBC1, except that accessing banks 20h, 40h, and 60h is supported now.

	A000-BFFF - RAM Bank 00-03, if any (Read/Write)
	A000-BFFF - RTC Register 08-0C (Read/Write)
		Depending on the current Bank Number/RTC Register selection (see below), this memory space is used to access an 8KByte external RAM Bank, or a single RTC Register.

	0000-1FFF - RAM and Timer Enable (Write Only)
		Mostly the same as for MBC1, a value of 0Ah will enable reading and writing to external RAM - and to the RTC Registers! A value of 00h will disable either.

	2000-3FFF - ROM Bank Number (Write Only)
		Same as for MBC1, except that the whole 7 bits of the RAM Bank Number are written directly to this address. As for the MBC1, writing a value of 00h, will select Bank 01h instead.
		All other values 01-7Fh select the corresponding ROM Banks.

	4000-5FFF - RAM Bank Number - or - RTC Register Select (Write Only)
		As for the MBC1s RAM Banking Mode, writing a value in range for 00h-03h maps the corresponding external RAM Bank (if any) into memory at A000-BFFF.
		When writing a value of 08h-0Ch, this will map the corresponding RTC register into memory at A000-BFFF. That register could then be read/written by
		accessing any address in that area, typically that is done by using address A000.

	6000-7FFF - Latch Clock Data (Write Only)
		When writing 00h, and then 01h to this register, the current time becomes latched into the RTC registers. The latched data will not change until it becomes latched again, by repeating the write 00h->01h procedure.
		This is supposed for <reading> from the RTC registers. It is proof to read the latched (frozen) time from the RTC registers, while the clock itself continues to tick in background.

	The Clock Counter Registers
		08h  RTC S   Seconds   0-59 (0-3Bh)
		09h  RTC M   Minutes   0-59 (0-3Bh)
		0Ah  RTC H   Hours     0-23 (0-17h)
		0Bh  RTC DL  Lower 8 bits of Day Counter (0-FFh)
		0Ch  RTC DH  Upper 1 bit of Day Counter, Carry Bit, Halt Flag
			Bit 0  Most significant bit of Day Counter (Bit 8)
			Bit 6  Halt (0=Active, 1=Stop Timer)						The Halt Flag is supposed to be set before <writing> to the RTC Registers.
			Bit 7  Day Counter Carry Bit (1=Counter Overflow)

	The Day Counter
		The total 9 bits of the Day Counter allow to count days in range from 0-511 (0-1FFh). The Day Counter Carry Bit becomes set when this value overflows.
		In that case the Carry Bit remains set until the program does reset it.
		Note that you can store an offset to the Day Counter in battery RAM. For example, every time you read a non-zero Day Counter,
		add this Counter to the offset in RAM, and reset the Counter to zero. This method allows to count any number of days, making your program Year-10000-Proof,
		provided that the cartridge gets used at least every 511 days.

	Delays
		When accessing the RTC Registers it is recommended to execute a 4ms delay (4 Cycles in Normal Speed Mode) between the separate accesses.


HuC1 (MBC with Infrared Controller)
	This controller (made by Hudson Soft) appears to be very similar to an MBC1 with the main difference being that it supports infrared LED input / output.
	(Similiar to the infrared port that has been later invented in CGBs.) The Japanese cart "Fighting Phoenix" (internal cart name: SUPER B DAMAN) is known to contain this chip.


MBC Timing Issues
	Using MBCs with CGB Double Speed Mode. The MBC5 has been designed to support CGB Double Speed Mode.
	There have been rumours that older MBCs (like MBC1-3) wouldn't be fast enough in that mode. If so, it might be nethertheless possible to use
	Double Speed during periods which use only code and data which is located in internal RAM.
	However, despite of the above, my own good old selfmade MBC1-EPROM card appears to work stable and fine even in Double Speed Mode though.
******/





/*****
Gamegenie/Shark Cheats

Game Shark and Gamegenie are external cartridge adapters that can be plugged between the gameboy and the actual game cartridge. Hexadecimal codes can be then entered for specific games, typically providing things like Infinite Sex, 255 Cigarettes, or Starting directly in Wonderland Level PRO, etc.

Gamegenie (ROM patches)
Gamegenie codes consist of nine-digit hex numbers, formatted as ABC-DEF-GHI, the meaning of the separate digits is:
  AB    New data
  FCDE  Memory address, XORed by 0F000h
  GI    Old data, XORed by 0BAh and rotated left by two
  H     Don't know, maybe checksum and/or else
The address should be located in ROM area 0000h-7FFFh, the adapter permanently compares address/old data with address/data being read by the game, and replaces that data by new data if necessary. That method (more or less) prohibits unwanted patching of wrong memory banks. Eventually it is also possible to patch external RAM ?
Newer devices reportedly allow to specify only the first six digits (optionally). As far as I rememeber, around three or four codes can be used simultaneously.

Game Shark (RAM patches)
Game Shark codes consist of eight-digit hex numbers, formatted as ABCDEFGH, the meaning of the separate digits is:
  AB    External RAM bank number
  CD    New Data
  GHEF  Memory Address (internal or external RAM, A000-DFFF)
As far as I understand, patching is implement by hooking the original VBlank interrupt handler, and re-writing RAM values each frame. The downside is that this method steals some CPU time, also, it cannot be used to patch program code in ROM.
As far as I rememeber, somewhat 10-25 codes can be used simultaneously.
******/