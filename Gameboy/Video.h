#pragma once
#include <SFML/Graphics.hpp>
#include "Memory.h"

class Video : public sf::RenderWindow
{
public:
	// Constructors & Destructors
	Video(Memory* memory, unsigned int width, unsigned int height, sf::String title);
	~Video();

	// Clear Frame
	void reset();
	void renderFrame();

	/*
	void renderScanline(BIT8 lineNumber);
	*/

private:
	// Store Memory Pointer so Video can access
	Memory* pMemory;

	void drawBackground(BIT16 tileMap, BIT16 tileData);
	void drawWindow(BIT16 tileMap, BIT16 tileData);
	void drawSprites(bool largeSprites);

	int backgroundSize = 256 * 256;
	int	windowSize = 256 * 256;		// TODO: is this size correct? I'm surprised since you always display the top left corner of it
	int	spritesSize = 160 * 144;	// TODO: is this size correct?
	sf::Color layerBackground[256*256];		// Layer: Background
	sf::Color layerWindow[256*256];			// Layer: Window
	sf::Color layerSprites[160*144];	// Layer: Sprites

	int gpuClockCycles;

	sf::Color pTransparent = sf::Color(1, 1, 1, 0);
	sf::Color const pWhite = sf::Color(255, 255, 255, 255);
	sf::Color const pLightGray = sf::Color(192, 192, 192, 255);
	sf::Color const pDarkGray = sf::Color(96, 96, 96, 255);
	sf::Color const pBlack = sf::Color(0, 0, 0, 255);
	
	sf::Color const bgPaletteMap[4] = { pWhite, pLightGray, pDarkGray, pBlack };
	sf::Color const obPaletteMap[4] = { pTransparent, pLightGray, pDarkGray, pBlack };
	sf::Color bgPalette[4];		//0xFF47
	sf::Color obPalette0[4];	//0xFF48
	sf::Color obPalette1[4];	//0xFF49
	void loadPalettes();

	/*	
	bool sprPriorities[160 * 144];		// Does this sprite have priority over the bg ?
	
	struct Sprite {
		BIT16 addr;
		BIT8 xPos;
		BIT8 yPos;
		BIT8 tile;
		bool priority;
		bool xFlip;
		bool yFlip;
		int palette;
	};
	Sprite spriteObjects[40];			// Sprite Objects

	void drawBackground(BIT16 tileMapAddr, BIT16 tileDataAddr, BIT8 lineNumber);
	void drawWindow(BIT16 windowMapAddr, BIT16 tileDataAddr, BIT8 lineNumber);
	void drawSprites(BIT16 spriteHeight, BIT8 lineNumber);
	void loadSprites(BIT8 lcd);
	bool spritePriority(Sprite lhs, Sprite rhs);
	*/
};