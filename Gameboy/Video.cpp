#include "Video.h"

// Constructor
Video::Video(Memory* memory, unsigned int width, unsigned int height, sf::String title)
	:RenderWindow(sf::VideoMode(width, height), title), pMemory(memory)
{
	reset();
	gpuClockCycles = 0;
}


// Destructor
Video::~Video() {
	// TODO
}


// Reset our Display
void Video::reset()
{
	clear();

	//Reset Sprites
	for (int i = 0; i < spritesSize; i++) {
		layerSprites[i] = pTransparent;
	}

	// Reset Window & Display
	for (int i = 0; i < backgroundSize; i++) {
		layerBackground[i] = pWhite;
		layerWindow[i] = pWhite;
	}
}


// Load our palette arrays to assist with creating bg/window/sprite arrays
void Video::loadPalettes() {
	BIT8 bgp = pMemory->readByte(0xFF47);
	BIT8 obp0 = pMemory->readByte(0xFF48);
	BIT8 obp1 = pMemory->readByte(0xFF49);

	// Get the 4 colors for each palette
	for (int color = 0; color < 4; color++)
	{
		// BGP
		bgPalette[color] = bgPaletteMap[(bgp >> color * 2) & 0x03];

		// OBP0
		obPalette0[color] = obPaletteMap[(obp0 >> color * 2) & 0x03];

		// OBP1
		obPalette1[color] = obPaletteMap[(obp1 >> color * 2) & 0x03];
	}
}


// Draw
void Video::renderFrame()
{
	BIT8 lcd = pMemory->readByte(0xFF40);
	loadPalettes();

	// Entire Display
	bool displayOn = (lcd & 0x80);
	//if (!displayOn)
	//	close();

	// Used for background & window
	BIT16 tileDataAddr = ((lcd & 0x10) == 0) ? 0x8800 : 0x8000;

	// Background
	bool bgDisplay = (lcd & 0x01);
	BIT16 bgTileMapAddr = ((lcd & 0x08) == 0) ? 0x9800 : 0x9C00;
	if (bgDisplay) {
		//printf("Display Background.\n");
		drawBackground(bgTileMapAddr, tileDataAddr);
	}

	// Window
	bool winDisplay = (lcd & 0x20);
	BIT16 winTileMapAddr = ((lcd & 0x40) == 0) ? 0x9800 : 0x9C00;
	if (winDisplay) {
		//printf("Display Window.\n");
		drawWindow(winTileMapAddr, tileDataAddr);
	}

	// Sprites
	bool sprDisplay = (lcd & 0x02);
	bool largeSprites = (lcd & 0x04);
	if (sprDisplay) {
		//printf("Display Sprites.");
		drawSprites(largeSprites);
	}

	BIT8 scy = pMemory->readByte(0xFF42);
	BIT8 scx = pMemory->readByte(0xFF43);
	
	// TODO: Only doing background for now.
	// Draw Display
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			sf::RectangleShape pixel(sf::Vector2f(3, 3));
			pixel.setPosition(x*3, y*3);
			pixel.setFillColor(layerBackground[x+(y*256)]);
			draw(pixel);
		}
	}
	display();
}

void Video::drawBackground(BIT16 tileMap, BIT16 tileData)
{
	BIT8 scrollY = pMemory->readByte(0xFF42);
	BIT8 scrollX = pMemory->readByte(0xFF43);

	BIT8 yPos = scrollY + pMemory->readByte(0xFF44);	// yPos is used to calculate which of 32 vertical tiles the current scanline is drawing
	BIT16 tileRow = ((yPos / 8) * 32);					// which of the 8 vertical pixels of the current tile is the scanline on?

	// 160 Wide By 144 Tall. Drawing a single line
	for (int pixel = 0; pixel < 160; pixel++)
	{
		BIT8 xPos = pixel + scrollX;

		// 8000: 0 to 255 // 8800: -128 to 127
		int offset = 0;
		if (tileData == 0x8800)
			offset = 128;

		// Load Tile
		BIT16 tileCol = (xPos / 8);		// which of the 32 horizontal tiles does this xPos fall within?
		signed char tileNumber = pMemory->readByte(tileMap + (tileRow*32) + tileCol);
		BIT16 tileLocation = tileData + ((tileNumber + offset) * 16);

		// Find Correct Vertical Line in this tile.
		BIT8 line = (yPos % 8)*2;
		BIT8 data1 = pMemory->readByte(tileLocation + line);
		BIT8 data2 = pMemory->readByte(tileLocation + line + 1);
		int colorBit = ((xPos % 8) - 7) * -1;	// This is strange at first glance, but it's correct.

		int bita = (data1 >> colorBit) & 0x01;		// 0 or 1
		int bitb = ((data2 >> colorBit) & 0x01) * 2; // 0 or 2

		// Add to background array.
		int pixelLocation = xPos + (scrollY * 256);
		layerBackground[pixelLocation] = bgPalette[(bita + bitb)];
	}



	/*
	// 8000: 0 to 255 // 8800: -128 to 127
	int offset = 0;
	if (tileData == 0x8800)
		offset = 128;

	// Loop through all tiles
	for (int tile = 0; tile < 1024; tile++) {
		signed char tileNumber = pMemory->readByte(tileMap + tile);
		
		// Load Tile
		BIT16 tileLocation = tileData + ((tileNumber+offset)*16) - 1;

		//printf("LOADING-TILE:\t %i  %04X  %04X\n", tile, tileNumber, tileLocation);

		for (int tileRow = 0; tileRow < 8; tileRow++)
		{
			BIT8 a = pMemory->readByte(++tileLocation);
			BIT8 b = pMemory->readByte(++tileLocation);

			//printf("\t%i: %02X: %02X\n", tileRow, a, b);

			// loop through each bit of the 2 bytes
			int rowPixel = 0;
			for (int bit = 7; bit >= 0; bit--)
			{
				int bita = (a >> bit) & 0x01;		// 0 or 1
				int bitb = ((b >> bit) & 0x01) * 2; // 0 or 2

				int xPos = (tile * 8 + rowPixel) % 256;
				int yPos = (tile / 32) * 8 + tileRow;
				int pixel = xPos + (yPos * 256);
				//printf("Add Pixel: %i(%i)\n", pixel, (bita + bitb));

				layerBackground[pixel] = bgPalette[(bita+bitb)];
				++rowPixel;
			}
		}
	}*/
}

void Video::drawWindow(BIT16 tileMap, BIT16 tileData)
{
	printf("Video::drawWindow()\n");
}

void Video::drawSprites(bool largeSprites)
{
	printf("Video::drawSprites()\n");
}




/* WORKING CODE
float pixelSize = 4;
sf::Color currentColor = pBlack;
for (int y = 0; y < height/pixelSize; y++)
{
if (y % 2 == 0)
currentColor = pBlack;
else
currentColor = pLightGray;

for (int x = 0; x < width/pixelSize; x++)
{
sf::RectangleShape pixel(sf::Vector2f(pixelSize, pixelSize));
pixel.setPosition(x*pixelSize, y*pixelSize);
pixel.setFillColor(currentColor);
//pixels[(y * 64) + x] = pixel;
draw(pixel);
}
}
display();
*/




/*
void Video::renderFrame() {
	BIT8 lcdc = pMemory->readByte(0xFF40);
	BIT8 stat = pMemory->readByte(0xFF41);

	BIT8 scy = pMemory->readByte(0xFF42);
	BIT8 scx = pMemory->readByte(0xFF43);

	BIT8 ly = pMemory->readByte(0xFF44);
	BIT8 lyc = pMemory->readByte(0xFF45);

	BIT8 dma = pMemory->readByte(0xFF46);

	BIT8 bgp = pMemory->readByte(0xFF47);
	BIT8 obp0 = pMemory->readByte(0xFF48);
	BIT8 obp1 = pMemory->readByte(0xFF49);

	BIT8 wy = pMemory->readByte(0xFF4A);
	BIT8 wx = pMemory->readByte(0xFF4B);

	printf("LCDC: %d,  STAT: %d,  SCY: %d,  SCX: %d,  LY: %d,  LYC: %d,  DMA: %d,  BGP: %d,  OBP0: %d,  OBP1: %d,  WY: %d,  WX: %d\n",
		lcdc, stat, scy, scx, ly, lyc, dma, bgp, obp0, obp1, wy, wx);
}

// Updates all of our arrays to draw them in the frame
void Video::renderScanline(BIT8 lineNumber) {
	BIT8 lcd = pMemory->readByte(0xFF40);
	BIT16 windowMapAddr = ((lcd & 0x40) == 0) ? 0x9800 : 0x9C00;
	BIT16 spriteHeight = ((lcd & 0x04) == 0) ? 8 : 16;
	BIT16 tileMapAddr = ((lcd & 0x08) == 0) ? 0x9800 : 0x9C00;
	BIT16 tileDataAddr = ((lcd & 0x10) == 0) ? 0x8800 : 0x8000;

	// TODO: The second table can be used for the background and the window display and its tiles are numbered from -128 to 127.
	// I'm not sure if ill be able to handle the negative numbers here

	// Start by loading the color palettes
	loadPalettes();

	drawBackground(tileMapAddr, tileDataAddr, lineNumber);
	drawWindow(windowMapAddr, tileDataAddr, lineNumber);
	drawSprites(spriteHeight, lineNumber);
}

// Create our array of pixels to draw the background in our frame.
void Video::drawBackground(BIT16 tileMapAddr, BIT16 tileDataAddr, BIT8 lineNumber) {
	// Select through each tile
	BIT16 currAddress = tileMapAddr + lineNumber * 256 - 1;		// Subtract 1 so we can use ++ in the loop below.
	for (int tile = 0; tile < 32; tile++)
	{
		BIT8 tileNumber = pMemory->readByte(currAddress++);
		BIT16 tileAddress = tileDataAddr + tileNumber * 16 - 1;		// 16 bytes per tile, Subtract 1 so we can use ++ in the loop below.

		// Select through each row of the tile
		for (int tileRow = 0; tileRow < 8; tileRow++)
		{
			BIT8 a = pMemory->readByte(tileAddress++);
			BIT8 b = pMemory->readByte(tileAddress++);

			// loop through each bit of the 2 bytes
			int rowPixel = 0;
			for (int bit = 7; bit >= 0; bit--)
			{
				int bita = a & (1 << bit) >> (bit - 1);	// returns 0 or 2
				int bitb = b & (1 << bit) >> (bit);		// returns 0 or 1

				int xPos = (tile * 8 + rowPixel) % 256;
				int pixel = xPos + (lineNumber * 256);	// yPos = lineNumber

				background[pixel] = bgPalette[bita + bitb];
				rowPixel++;
			}
		}
	}
}

// Create our array of pixels to draw the window in our frame.
void Video::drawWindow(BIT16 windowMapAddr, BIT16 tileDataAddr, BIT8 lineNumber) {
	// Select through each tile
	BIT16 currAddress = windowMapAddr + lineNumber * 256 - 1;		// Subtract 1 so we can use ++ in the loop below.
	for (int tile = 0; tile < 32; tile++)
	{
		BIT8 tileNumber = pMemory->readByte(currAddress++);
		BIT16 tileAddress = tileDataAddr + tileNumber * 16 - 1;		// 16 bytes per tile, Subtract 1 so we can use ++ in the loop below.

		// Select through each row of the tile
		for (int tileRow = 0; tileRow < 8; tileRow++)
		{
			BIT8 a = pMemory->readByte(tileAddress++);
			BIT8 b = pMemory->readByte(tileAddress++);

			// loop through each bit of the 2 bytes
			int rowPixel = 0;
			for (int bit = 7; b >= 0; bit--)
			{
				int bita = a & (1 << bit) >> (bit - 1);	// returns 0 or 2
				int bitb = b & (1 << bit) >> (bit);		// returns 0 or 1

				int xPos = (tile * 8 + rowPixel) % 256;
				int pixel = xPos + (lineNumber * 256);	// yPos = lineNumber

				window[pixel] = bgPalette[bita + bitb];
				rowPixel++;
			}
		}
	}
}

//Sprite attributes reside in the Sprite Attribute Table (OAM - Object Attribute Memory) at $FE00-FE9F.
void Video::loadSprites(BIT8 lcd) {
	BIT16 currAddress = 0xFE00 - 1; // Subtract 1 so we can do ++ in loop below.
	for (int i = 0; i < 40; i++)
	{
		BIT8 yPos = pMemory->readByte(currAddress++) - 16; 	//An offscreen value hides the sprite.
		BIT8 xPos = pMemory->readByte(currAddress++) - 8;	//An offscreen value hides the sprite, but the sprite still affects the priority ordering 
		BIT8 tile = pMemory->readByte(currAddress++);
		BIT8 attr = pMemory->readByte(currAddress++);

		bool priority = !(lcd & 0x80);		// True = Obj Above background, False = Behind background (except white)
		bool xFlip = lcd & 0x40;				// True = Flip
		bool yFlip = lcd & 0x20;				// True = Flip
		int palette = (lcd & 0x10) ? 1 : 0;	// 0 = OBP0, 1 = OBP1

		Sprite s = { (BIT16)(currAddress - 3), xPos, yPos, tile, priority, xFlip, yFlip, palette };
		spriteObjects[i] = s;
	}

	// Sort Array so that first objects have the priority to be drawn.
	// std::sort(spriteObjects, spriteObjects+40, spritePriorities);		// TODO
}

//	When sprites with different x coordinate values overlap, the one with the smaller x coordinate (closer to the left)
//		will have priority and appear above any others.
//	When sprites with the same x coordinate values overlap, they have priority according to table ordering.
//		(i.e. $FE00 - highest, $FE04 - next highest, etc.)
bool Video::spritePriority(Sprite lhs, Sprite rhs) {
	if (lhs.xPos == rhs.xPos)
		return lhs.addr < rhs.addr;
	else
		return lhs.xPos < rhs.xPos;
}

// Create our array of pixels to draw the sprites in our frame.
void Video::drawSprites(BIT16 spriteHeight, BIT8 lineNumber) {
	BIT16 spritePatternTable = 0x8000;

	int spritesDrawn = 0;
	for (int i = 0; i < 40; i++)
	{
		Sprite spr = spriteObjects[i];

		if (spritesDrawn > 9)						// Only 10 sprites can be displayed on any one line.
			break;

		if (spr.yPos + spriteHeight < lineNumber)	// If the bottom is above the current line, move on
			continue;

		if (spr.yPos > lineNumber)					// If the top is below the current line, move on
			continue;


		BIT16 tileAddress = spritePatternTable + spr.tile * 16;	// Each tile is 16 bytes, so we skip ahead to the tile we want.
		int tileLine = lineNumber - spr.yPos;					// Which line of the sprite are we trying to draw?
		if (spr.yFlip)
			tileLine = spriteHeight - 1 - tileLine;

		BIT16 row = tileAddress + 2 * tileLine;
		BIT8 a = pMemory->readByte(row);
		BIT8 b = pMemory->readByte(row++);

		// loop through each bit of the 2 bytes
		for (int bit = 7; b >= 0; bit--)
		{
			int bita = a & (1 << bit) >> (bit - 1);				// returns 0 or 2
			int bitb = b & (1 << (bit - 1)) >> (bit - 1);			// returns 0 or 1

			int xPos = spr.xFlip ? (xPos + (7 - bit)) : xPos;
			int pixel = xPos + (lineNumber * 160);				// yPos = lineNumber

			if (spr.palette == 0)
				sprites[pixel] = obPalette0[bita + bitb];
			else
				sprites[pixel] = obPalette1[bita + bitb];

			sprPriorities[pixel] = spr.priority;
		}
		spritesDrawn++;
	}
}
*/





/*  EXTRA OLD
void Video::renderFrame(){
	BIT8 lcd = pMemory->readByte(0xFF40);
	bool bgDisplay = lcd & 0x01;
	bool spriteEnable = lcd & 0x02;
	bool windowEnable = lcd & 0x20;
	bool lcdEnable = lcd & 0x80;

	// Specifies the upper/left positions of the Window area.
	BIT8 winY = pMemory->readByte(0xFF4A);
	BIT8 winX = pMemory->readByte(0xFF4B) - 7;

	// The SCY and SCX allow you to select the origin of the visible 160x144 pixel area within the total 256x256 pixel background map.
	BIT8 scrollY = pMemory->readByte(0xFF42);
	BIT8 scrollX = pMemory->readByte(0xFF43);

	printf("Video::renderFrame() - %d\n", lcdEnable);

	if (lcdEnable) {
		for (int y = 0; y < 144; y++)
		{
			renderScanline(y);

		for (int x = 0; x < 160; x++)
		{
			//for (int y = 0; y < 144; y++)
			//{
				if (bgDisplay) {
					int xPos = x + scrollX;
					int yPos = y + scrollY;

					// If its >=256, it will scroll to the other side, but we need to make sure we stay on the same row/column
					xPos = (xPos >= 256) ? xPos % 256 : xPos;
					yPos = (yPos >= 256) ? yPos % 256 : yPos;

					//frame.setPixel(x, y, background[yPos * 256 + xPos]);

					sf::RectangleShape pixel(sf::Vector2f(1, 1));
					pixel.setPosition(x, y);
					pixel.setFillColor(background[yPos * 256 + xPos]);
					draw(pixel);
				}

				if (windowEnable && x >= winX && y >= winY) {
					//frame.setPixel(x, y, window[(y - winY) * 256 + (x - winX)]);
					sf::RectangleShape pixel(sf::Vector2f(1, 1));
					pixel.setPosition(x, y);
					pixel.setFillColor(window[(y - winY) * 256 + (x - winX)]);
					draw(pixel);
				}

				if (spriteEnable && sprPriorities[y * 160 + x]) {
					//frame.setPixel(x, y, sprites[y * 160 + x]);
					sf::RectangleShape pixel(sf::Vector2f(1, 1));
					pixel.setPosition(x, y);
					pixel.setFillColor(sprites[y * 160 + x]);
					draw(pixel);
				}
			}
		}
		display();
	}
	else {
		reset();
	}
}*/







/**
NOTES / TODO:

0xFF41 - STAT - do we need to use any of these bits?

FF44 - LY - LCDC Y-Coordinate (R)
	The LY indicates the vertical line to which the present data is transferred to the LCD Driver.
	The LY can take on any value between 0 through 153. The values between 144 and 153 indicate the V-Blank period. Writing will reset the counter.

FF45 - LYC - LY Compare (R/W)
	The gameboy permanently compares the value of the LYC and LY registers. When both values are identical,
	the coincident bit in the STAT register becomes set, and (if enabled) a STAT interrupt is requested.

FF46 - DMA - DMA Transfer and Start Address (W)
	Writing to this register launches a DMA transfer from ROM or RAM to OAM memory (sprite attribute table). The written value specifies the transfer source address divided by 100h.
	It takes 160 microseconds until the transfer has completed (80 microseconds in CGB Double Speed Mode), during this time the CPU can access only HRAM (memory at FF80-FFFE).
	For this reason, the programmer must copy a short procedure into HRAM, and use this procedure to start the transfer from inside HRAM, and wait until the transfer has finished.
	Most programs are executing this procedure from inside of their VBlank procedure, but it is possible to execute it during display redraw also,
	allowing to display more than 40 sprites on the screen (ie. for example 40 sprites in upper half, and other 40 sprites in lower half of the screen).

Writing Data to OAM Memory
	The recommened method is to write the data to normal RAM first, and to copy that RAM to OAM by using the DMA transfer function, initiated through DMA register (FF46).
	Beside for that, it is also possible to write data directly to the OAM area by using normal LD commands, this works only during the H-Blank and V-Blank periods.
	The current state of the LCD controller can be read out from the STAT register (FF41).

CAUTION
	When the LCD Controller is drawing the screen it is directly reading from Video Memory (VRAM) and from the Sprite Attribute Table (OAM).
	During these periods the Gameboy CPU may not access the VRAM and OAM. That means, any attempts to write to VRAM/OAM are ignored (the data remains unchanged).
	And any attempts to read from VRAM/OAM will return undefined data (typically a value of FFh).
	For this reason the program should verify if VRAM/OAM is accessable before actually reading or writing to it. This is usually done by reading the Mode Bits from the STAT Register (FF41).
	When doing this (as described in the examples below) you should take care that no interrupts occur between the wait loops and the following memory access
	- the memory is guaranted to be accessable only for a few cycles directly after the wait loops have completed.
		VRAM (memory at 8000h-9FFFh) is accessable during Mode 0-2
			Even if the procedure gets executed at the <end> of Mode 0 or 1, it is still proof to assume that VRAM can be accessed for a few more cycles because in either case the following period is
			Mode 2 which allows access to VRAM either.

		OAM (memory at FE00h-FE9Fh) is accessable during Mode 0-1
			Beside for that, OAM can be accessed at any time by using the DMA Function (FF46).
			The two wait loops ensure that Mode 0 or 1 will last for a few clock cycles after completion of the procedure.
			In V-Blank period it might be recommended to skip the whole procedure - and in most cases using the above mentioned DMA function would be more recommended anyways.

Scanline (accessing OAM)	2	80
Scanline (accessing VRAM)	3	172
Horizontal blank			0	204
One line (scan and blank)		456
Vertical blank				1	4560 (10 lines)
Full frame (scans / vblank)		70224


Sprite RAM Bug
	There is a flaw in the GameBoy hardware that causes trash to be written to OAM RAM if the following commands are used while their 16-bit content is in the	range of $FE00 to $FEFF:
		inc xx (xx = bc,de, or hl)
		dec xx
		ldi a,(hl)
		ldd a,(hl)
		ldi (hl),a
		ldd (hl),a
	Only sprites 1 & 2 ($FE00 & $FE04) are not affected by these instructions.


	FF46 - DMA - DMA Transfer and Start Address (W)
Writing to this register launches a DMA transfer from ROM or RAM to OAM memory (sprite attribute table). The written value specifies the transfer source address divided by 100h, ie. source & destination are:
  Source:      XX00-XX9F   ;XX in range from 00-F1h
  Destination: FE00-FE9F
It takes 160 microseconds until the transfer has completed (80 microseconds in CGB Double Speed Mode), during this time the CPU can access only HRAM (memory at FF80-FFFE). For this reason, the programmer must copy a short procedure into HRAM, and use this procedure to start the transfer from inside HRAM, and wait until the transfer has finished:
   ld  (0FF46h),a ;start DMA transfer, a=start address/100h
   ld  a,28h      ;delay...
  wait:           ;total 5x40 cycles, approx 200ms
   dec a          ;1 cycle
   jr  nz,wait    ;4 cycles
Most programs are executing this procedure from inside of their VBlank procedure, but it is possible to execute it during display redraw also, allowing to display more than 40 sprites on the screen (ie. for example 40 sprites in upper half, and other 40 sprites in lower half of the screen).

*/