/*
 *  plat64.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include <string.h>
#include <conio.h>
#include "../globals.h"
#include "../plat.h"
#include "logo64.h"
#include "../frontend.h"

#define VicBase					(0x4000)
#define Screen					((char*)(VicBase + 0x1c00))
#define Colour					((char*)0xd800)
#define GfxBaseAddress			((char*)(VicBase + 0x2000))
#define CharROM					((char*)0xd000)
#define R6510					((char*)0x01)
#define	GridRowHeight			5
#define	ResolutionX				320
#define ResolutionY				200
#define SnakeStartSpeed			0xfe28
#define SnakeSpeedIncrease		0x08
#define	AppleTicksCycleLength	10

void platInstallRaster();
void platRemoveRaster();
void platIRQMultiColourOn();
void platIRQMultiColourOff();
void platPrepAppleTicksBarColumn(char column);
void platShowTicksToNextAppleColumn(char column, char y1);
void platShowLogo(char row);

#define platIOOff() 			CIA1.cra &= ~1
#define platIOOn() 				CIA1.cra |= 1
#define platCharMemIn() 		platIOOff(); *R6510 &= ~4
#define platCharMemOut() 		*R6510 |= 4; platIOOn()
#define kernelOut() 			//asm("sei"); *R6510 &= ~2
#define kernelIn() 				//*R6510 |= 2; asm("cli")
#define platVICBankGfx()		CIA2.ddra |= 3; CIA2.pra = (CIA2.pra & 0xfc) | 2; VIC.addr = 0x78; *(char*)0x288 = ((int)Screen/256)
#define platVICBankNormal()		CIA2.ddra |= 3; CIA2.pra = (CIA2.pra & 0xfc) | 3; VIC.addr = 0x14; *(char*)0x288 = (1024/256)
#define platGraphicsOn()		VIC.ctrl1 |= (1<<5)
#define platGraphicsOff()		VIC.ctrl1 &= ~(1<<5)
#define platMultiModeOn()		VIC.ctrl2 |= (1<<4)
#define platMultiModeOff()		VIC.ctrl2 &= ~(1<<4)
#define platSetBorderColour(b)	VIC.bordercolor = b
#define platSetBackColour(b)	VIC.bgcolor0 = b

static bool sb_syncActive;
static char sc_lineSplit[GridY];
static char sc_foreColourLine[GridY][2];
static char sc_characterBuffer[64*CharHeight];
static char sc_rasterLineMulti;
static char sc_rasterLineMono;
static char sc_linesScrolled;
static char sc_scrollColourIndex;
static char sc_colorsScrolled;
static char sc_colourCycleIndex ;
static int si_charScreenStart[ScreenY];
static int si_gfxScreenStart[GridY];
static int si_syncSpeed;

const char cc_appleTicksColourCycle[] = {COLOR_YELLOW, COLOR_ORANGE, COLOR_BROWN, COLOR_YELLOW, COLOR_ORANGE};
const char cc_appleTicksCycleSpeed[AppleTicksCycleLength] = {8, 8, 7, 7, 6, 6, 5, 5, 4, 3};
const char cc_timePlotChar[] = {0xff, 0xff, 0xaa, 0xaa, 0x55, 0x55, 0xff, 0xff, 0xaa, 0xaa, 0x55, 0x55};
const char cc_scrollColours[] = {COLOR_GREEN, COLOR_LIGHTGREEN, COLOR_YELLOW, COLOR_GREEN, COLOR_LIGHTGREEN, COLOR_YELLOW};
const char cc_pixels[][GridRowHeight] =
{
	{0x28, 0x28, 0x69, 0xaa, 0xaa},		// gCHAR_SnakeHeadUp
	{0x90, 0x90, 0xaa, 0xaa, 0x90},		// gCHAR_SnakeHeadRight
	{0xaa, 0xaa, 0x69, 0x28, 0x28},		// gCHAR_SnakeHeadDown
	{0x06, 0x06, 0xaa, 0xaa, 0x06},		// gCHAR_SnakeHeadLeft

	{0x15, 0x15, 0x15, 0x15, 0x15},		// gCHAR_SnakeBodyUp
	{0x00, 0x55, 0x55, 0x55, 0x55},		// gCHAR_SnakeBodyRight
	{0x15, 0x15, 0x15, 0x15, 0x15},		// gCHAR_SnakeBodyDown
	{0x00, 0x55, 0x55, 0x55, 0x55},		// gCHAR_SnakeBodyLeft

	{0x00, 0x01, 0x05, 0x15, 0x15},		// gCHAR_SnakeBodyUpRight
	{0x00, 0x40, 0x50, 0x54, 0x54},		// gCHAR_SnakeBodyRightDown
	{0x15, 0x55, 0x55, 0x54, 0x50},		// gCHAR_SnakeBodyDownLeft
	{0x15, 0x15, 0x15, 0x05, 0x01},		// gCHAR_SnakeBodyLeftUp

	{0x00, 0x40, 0x50, 0x54, 0x54},		// gCHAR_SnakeBodyUpLeft
	{0x00, 0x01, 0x05, 0x15, 0x15},		// gCHAR_SnakeBodyLeftDown
	{0x15, 0x15, 0x15, 0x05, 0x01},		// gCHAR_SnakeBodyDownRight
	{0x15, 0x55, 0x55, 0x54, 0x50},		// gCHAR_SnakeBodyRightUp

	{0x15, 0x15, 0x04, 0x04, 0x04},		// gCHAR_SnakeTailUp
	{0x00, 0x01, 0x15, 0x15, 0x01},		// gCHAR_SnakeTailRight
	{0x04, 0x04, 0x04, 0x15, 0x15},		// gCHAR_SnakeTailDown
	{0x00, 0x40, 0x54, 0x54, 0x40},		// gCHAR_SnakeTailLeft
	
	{0x00, 0x00, 0x00, 0x00, 0x00},		// gCHAR_Blank
	{0x28, 0xaa, 0xaa, 0xaa, 0x28},		// gCHAR_Apple
	{0x3c, 0xff, 0xff, 0xff, 0x3c},		// gCHAR_Plum
	{0x00, 0x00, 0x00, 0x00, 0x00},		// gCHAR_Exit
	{0xfa, 0xfa, 0xfa, 0xfa, 0xfa},		// gCHAR_ExitMarker
	{0xff, 0xff, 0xff, 0xff, 0xff},		// gCHAR_Wall1 (Solid Purple)
	{0x55, 0x55, 0x55, 0x55, 0x55},		// gCHAR_Wall2 (Solid Green)
	{0xaa, 0xaa, 0xaa, 0xaa, 0xaa},		// gCHAR_Wall3 (Solid Red)
	{0xcc, 0x33, 0xcc, 0x33, 0xcc},		// gCHAR_Wall4 (Purple / Black chequer)
	{0x44, 0x11, 0x44, 0x11, 0x44},		// gCHAR_Wall5 (Green / Black chequer)
	{0xdd, 0x77, 0xdd, 0x77, 0xdd},		// gCHAR_Wall6 (Purple / Green chequer)
	{0xee, 0xbb, 0xee, 0xbb, 0xee},		// gCHAR_Wall7 (Purple / Red chequer)
	{0x66, 0x99, 0x66, 0x99, 0x66},		// gCHAR_Wall8 (Green / Red chequer)
	{0xff, 0xd7, 0xd7, 0xd7, 0xff},		// gCHAR_Wall9 (purple/green box)
	{0xff, 0xff, 0xff, 0xff, 0xff},		// gCHAR_Wall10 (Solid Purple but orange/brown is 1st 2 channels)
};

// CIA2.icr		= $dd0d
// CIA1.icr		= $dc0d
// CIA1.cra		= $dc0e
// VIC.ctrl1	= $d011
// VIC.imr		= $d01a
// VIC.ctrl2	= $d016
// VIC.addr		= $d018

void platInitialSetup()
{
	int x, y, gridc_pixelsThisChar, pixelsThisChar, rowOffset, screenRow;
	
	x = 0;
	for(y = 0; y < ScreenY; ++y)
	{
		si_charScreenStart[y] = x;
		x += ScreenX;
	}

	screenRow = 0;
	gridc_pixelsThisChar = 0;
	pixelsThisChar = 0;
	rowOffset = 0;
	x = 0;
	for(y = 0; y < GridY; ++y)
	{
		si_gfxScreenStart[y] = x + rowOffset;
		x += GridRowHeight;
		if(x >= CharHeight)
		{
			x -= CharHeight;
			rowOffset += (CharHeight*ScreenX);
		}
		
		sc_foreColourLine[y][0] = screenRow;

		gridc_pixelsThisChar = GridRowHeight;
		pixelsThisChar += GridRowHeight;
		if(pixelsThisChar >= CharHeight)
		{
			sc_foreColourLine[y][1] = ++screenRow;
			pixelsThisChar -= CharHeight;
			gridc_pixelsThisChar = GridRowHeight - pixelsThisChar;
		}
		else
		 	sc_foreColourLine[y][1] = 0xff;
			
		sc_lineSplit[y] = gridc_pixelsThisChar;
	}
	
	platCharMemIn();
	for(x = 0; x < 64 * CharHeight; ++x)
		sc_characterBuffer[x] = CharROM[x];
	platCharMemOut();
	
	platSetBackColour(COLOR_BLACK);
	platSetBorderColour(COLOR_BLACK);
	platClearScreen();
	platVICBankGfx();
	platGraphicsOn();

	platSyncGameSpeed();
	CIA1.cra = (CIA1.cra & 0xc0) | 0x10;
	CIA1.crb |= 0x40;

	platInstallRaster();
}

void platSetupGame()
{
	sc_rasterLineMulti = 0;
	sc_rasterLineMono = 234;
}

void platSetupFrontend(char row)
{
	sc_rasterLineMulti = 51 + (row * CharHeight) - (CharHeight+2);
	sc_rasterLineMono = sc_rasterLineMulti + LogoRows + (CharHeight+2);
	memset(Screen, (COLOR_WHITE<<4)|COLOR_BLACK, ScreenX*ScreenY);
	platClearScreen();
	platShowLogo(row);
}

void platSetupLevel()
{
	platClearScreen();
	memset(Screen, (COLOR_GREEN<<4)|COLOR_RED, (ScreenX*(ScreenY-3))+24);
	memset(Colour, COLOR_PURPLE, ScreenX*ScreenY);
}

void platInstallRaster()
{
	asm("sei");
	asm("lda #$7f");
	asm("sta $dc0d");
	asm("sta $dd0d");
	asm("and $d011");
	asm("sta $d011");
	asm("lda %v", sc_rasterLineMulti);
	asm("sta $d012");
	asm("lda #<%v",platIRQMultiColourOn);
	asm("sta $0314");
	asm("lda #>%v", platIRQMultiColourOn);
	asm("sta $0315");
	asm("lda #1");
	asm("sta $d01a");
	asm("cli");
}

void platRemoveRaster()
{
	asm("sei"); 
	asm("lda #$ff");
	asm("sta $dc0d");
	asm("sta $dd0d");
	asm("lda #$31"); 
	asm("sta $0314"); 
	asm("lda #$ea"); 
	asm("sta $0315"); 
	asm("lda #0");
	asm("sta $d01a");
	asm("cli");
}

void platIRQMultiColourOn()
{
	platMultiModeOn();
	asm("lda #<%v", platIRQMultiColourOff);
	asm("sta $0314");
	asm("lda #>%v", platIRQMultiColourOff);
	asm("sta $0315");
	asm("lda %v", sc_rasterLineMono);
	asm("sta $d012");
	asm("asl $d019");
	asm("jmp $ea31");
}

void platIRQMultiColourOff()
{
	platMultiModeOff();
	asm("lda #<%v", platIRQMultiColourOn);
	asm("sta $0314");
	asm("lda #>%v", platIRQMultiColourOn);
	asm("sta $0315");
	asm("lda %v", sc_rasterLineMulti);
	asm("sta $d012");
	asm("asl $d019");
	asm("jmp $ea81");
}

void platBeginFrame()
{
	CIA1.crb &= 0xfe;
	CIA1.cra &= 0xfe;
	CIA1.ta_lo = 0xff;
	CIA1.tb_lo = 0xff;
	CIA1.ta_hi = 0xff;
	CIA1.tb_hi = 0xff;
	CIA1.crb |= 1;
	CIA1.cra |= 1;
}
void platSyncEndFrame()
{
	if(sb_syncActive)
		while(*((int*)&CIA1.ta_hi) > si_syncSpeed);
}

void platSyncFastSpeed()
{
	sb_syncActive = NO;
}

void platSyncGameSpeed()
{
	si_syncSpeed = SnakeStartSpeed;
	sb_syncActive = YES;
}

void platAdjustSyncSpeed()
{
	si_syncSpeed += SnakeSpeedIncrease;
}

void platClearScreen()
{
	kernelOut();
	memset(GfxBaseAddress, 0, (ResolutionX/8) * ResolutionY);
	sc_linesScrolled = 0;
	kernelIn();
}

void platPlot(char gx, char gy, char gv)
{
	char *pGfxPlot, charLinesThisRow;

	charLinesThisRow = sc_lineSplit[gy];
	pGfxPlot = GfxBaseAddress + si_gfxScreenStart[gy] + (gx * CharHeight);
	memcpy(pGfxPlot, cc_pixels[gv], charLinesThisRow);
	if(charLinesThisRow < GridRowHeight)
	{
		pGfxPlot += (CharHeight*ScreenX) - CharHeight + charLinesThisRow;
		memcpy(pGfxPlot, &cc_pixels[gv][charLinesThisRow], GridRowHeight - charLinesThisRow);
	}
	
	if(gv <= gCHAR_Blank)
	{
		if(gv <= gCHAR_SnakeHeadLeft)
		{
			int offset = si_charScreenStart[sc_foreColourLine[gy][0]]+gx;
			Screen[offset] = (COLOR_GREEN<<4)+COLOR_RED;
			if(0xff != sc_foreColourLine[gy][1])
				Screen[offset+ScreenX] = (COLOR_GREEN<<4)+COLOR_RED;
			if(gCHAR_SnakeHeadLeft == gv)
			{
				pGfxPlot = GfxBaseAddress + si_gfxScreenStart[gy+1] + (gx * CharHeight);
				*pGfxPlot = (*pGfxPlot & 0xf0 ) | cc_pixels[gv][GridRowHeight-1];
			}
			else if(gCHAR_SnakeHeadRight == gv)
			{
				pGfxPlot = GfxBaseAddress + si_gfxScreenStart[gy+1] + (gx * CharHeight);
				*pGfxPlot = (*pGfxPlot & 0x0f ) | cc_pixels[gv][GridRowHeight-1];
			}
		}
		else
		{
			++gy;
			gv = gc_grid[gi_gridStart[gy]+gx];
			pGfxPlot = GfxBaseAddress + si_gfxScreenStart[gy] + (gx * CharHeight);
			*pGfxPlot = cc_pixels[gv][0];
		}
	}
}

void platShowTextAligned(char textPosX, char textPosY, const char *szText)
{
	char character, index, lines, *memory;
	int offset, memIndex;
	
	index = 0;
	memIndex = 0;
	memory = GfxBaseAddress + (textPosY * CharHeight * ScreenX) + (textPosX * CharHeight);
	
	character = szText[index];
	while(character)
	{
		if(character >= 'a' - 1)
			character -= 'a' - 1;
		offset = character;
		offset *= CharHeight;
		for(lines = 0; lines < CharHeight; ++lines)
		{
			memory[memIndex] = sc_characterBuffer[offset];
			++memIndex;
			++offset;
		}
		character = szText[++index];
		memory += CharHeight;
		memIndex = 0;
	}
}

void platShowTextSliver(char textPosY, char sliver, const char *szText)
{
	char character, index, *memory;
	int offset;
	
	index = 0;
	memory = GfxBaseAddress + textPosY * CharHeight * ScreenX;
	character = szText[index];
	while(character)
	{
		if(character >= 'a' - 1)
			character -= 'a' - 1;
		offset = character;
		offset *= CharHeight;
		offset += sliver;
		*memory = sc_characterBuffer[offset];
		character = szText[++index];
		memory += CharHeight;
	}
}

void platPrepAppleTicksBarColumn(char column)
{
	char *pGfxPlot, index, y;

	pGfxPlot = GfxBaseAddress + (ScreenX * CharHeight) + (column * CharHeight);
	index = 0;
	for(y = 1 ; y < 21; ++y)
	{
		if(y < 11)
		{
			Screen[si_charScreenStart[y]+column] = (COLOR_PURPLE<<4)+COLOR_PURPLE;
		}
		else
		{
			Screen[si_charScreenStart[y]+column] = (COLOR_ORANGE<<4)+COLOR_BROWN;
			Colour[si_charScreenStart[y]+column] = COLOR_YELLOW;
		}
		memcpy(pGfxPlot, &cc_timePlotChar[index], CharHeight);
		pGfxPlot += (ScreenX * CharHeight);
		index += 2;
		if(index > 4)
			index = 0;
	}
}

void platPrepAppleTicksBar()
{
	platPrepAppleTicksBarColumn(0);
	platPrepAppleTicksBarColumn(ScreenX-1);
}

void platShowTicksToNextAppleColumn(char column, char y1)
{
	char y0;
	int offset;
	
	offset = si_charScreenStart[20]+column;
	for(y0 = 20; y0 > y1; --y0)
	{
		Screen[offset] = (cc_appleTicksColourCycle[sc_colourCycleIndex]<<4)+cc_appleTicksColourCycle[sc_colourCycleIndex+1];
		Colour[offset] = (cc_appleTicksColourCycle[sc_colourCycleIndex+2]);
		offset -= ScreenX;
	}

	for(; y0 > 1; --y0)
	{
		Screen[offset] = (COLOR_PURPLE<<4)+COLOR_PURPLE;
		Colour[offset] = COLOR_PURPLE;
		offset -= ScreenX;
	}
}

void platShowTicksToNextApple(char force)
{
	static char sc_localTicks;
	
	char timeToTick = gc_applesGottenThisLevel < AppleTicksCycleLength ? gc_applesGottenThisLevel : AppleTicksCycleLength - 1;
	if(force || ++sc_localTicks > cc_appleTicksCycleSpeed[timeToTick])
	{
		int y0 = ((gc_nextAppleTicks * 10) / TicksToApple);
		char y1 = 10 - y0 ;
		platShowTicksToNextAppleColumn(0, y1);
		platShowTicksToNextAppleColumn(ScreenX-1, y1);

		if(++sc_colourCycleIndex > 2)
			sc_colourCycleIndex = 0;

		sc_localTicks = 0;
	}
}

void platShowLogo(char row)
{
	char *m;
	int index;

	m = GfxBaseAddress+(row*320)+(((40-LogoCols)/2)*CharHeight);
	index = 0;
	memset(&Screen[si_charScreenStart[row-1]], (COLOR_GREEN<<4)+COLOR_LIGHTGREEN, ((LogoRows/8)+1)*ScreenX);
	memset(&Colour[si_charScreenStart[row-1]], COLOR_YELLOW, ((LogoRows/8)+1)*ScreenX);
	while(index < LogoRows * LogoCols)
	{
		memcpy(m, &cc_snakeLogo[index], (LogoCols * CharHeight));
		index += ((LogoCols) * CharHeight);
		m += ScreenX * CharHeight;
	}
	sc_linesScrolled = 0;
	sc_colorsScrolled = LogoRows + (row * CharHeight);
	sc_scrollColourIndex = 0;
}

void platScrollScreenUp()
{
#if 0
	char row, columns, *row1Ptr, *row2Ptr, *pointer = GfxBaseAddress;
	
	for(row = ScreenY-1; row > 0 ; --row)
	{
		row1Ptr = pointer + 7;
		kernelOut();
		for(columns = ScreenX; columns > 0 ; --columns)
		{
			pointer[0] = pointer[1];
			pointer[1] = pointer[2];
			pointer[2] = pointer[3];
			pointer[3] = pointer[4];
			pointer[4] = pointer[5];
			pointer[5] = pointer[6];
			pointer[6] = pointer[7];
			pointer += 8;
		}
		
		row2Ptr = pointer;
		for(columns = ScreenX; columns > 0; --columns)
		{
			*row1Ptr = *row2Ptr;
			row1Ptr += 8;
			row2Ptr += 8;
		}
		kernelIn();
	}
#else
	asm("lda #<%w", (int)GfxBaseAddress);
	asm("sta ptr1");
	asm("lda #>%w", (int)GfxBaseAddress);
	asm("sta ptr1+1");
	asm("lda #%b", ScreenY-1);
scrollloop:
	asm("pha");
	asm("lda ptr1+1");
	asm("sta ptr2+1");
	asm("lda ptr1");
	asm("clc" );
	asm("adc #7");
	asm("sta ptr2");
	asm("bcc %g",j1);
	asm("inc ptr2+1");
j1:

	asm("lda ptr1+1");
	asm("sta ptr3+1");
	asm("lda ptr1");
	asm("clc" );
	asm("adc #1");
	asm("sta ptr3");
	asm("bcc %g",j2);
	asm("inc ptr3+1");
j2:

	kernelOut();

	asm("ldx #%b", ScreenX);
charloop:
	asm("ldy #0");
	asm("lda (ptr3),y");
	asm("sta (ptr1),y");
	asm("iny");
	asm("lda (ptr3),y");
	asm("sta (ptr1),y");
	asm("iny");
	asm("lda (ptr3),y");
	asm("sta (ptr1),y");
	asm("iny");
	asm("lda (ptr3),y");
	asm("sta (ptr1),y");
	asm("iny");
	asm("lda (ptr3),y");
	asm("sta (ptr1),y");
	asm("iny");
	asm("lda (ptr3),y");
	asm("sta (ptr1),y");
	asm("iny");
	asm("lda (ptr3),y");
	asm("sta (ptr1),y");

	asm("lda ptr1");
	asm("clc");
	asm("adc #8");
	asm("sta ptr1");
	asm("bcc %g", j3);
	asm("inc ptr1+1");
	asm("clc");
j3:
	asm("lda ptr3");
	asm("adc #8");
	asm("sta ptr3");
	asm("bcc %g", j4);
	asm("inc ptr3+1");
j4:
	asm("dex");
	asm("bne %g", charloop);

	asm("lda ptr1");
	asm("sta ptr3");
	asm("lda ptr1+1");
	asm("sta ptr3+1");
	asm("ldx #%b", ScreenX);
	asm("ldy #0");
xcharloop:
	asm("lda (ptr3),y");
	asm("sta (ptr2),y");
	asm("lda ptr3");
	asm("clc");
	asm("adc #8");
	asm("sta ptr3");
	asm("bcc %g", j5);
	asm("inc ptr3+1");
	asm("clc");

j5:	
	asm("lda ptr2");
	asm("adc #8");
	asm("sta ptr2");
	asm("bcc %g", j6);
	asm("inc ptr2+1");
	
j6:
	asm("dex");
	asm("bne %g", xcharloop);

	kernelIn();

	asm("pla");
	asm("tax");
	asm("dex");
	asm("txa");
	asm("beq %g", j7);
	asm("jmp %g", scrollloop);
j7:
#endif
	if(sc_colorsScrolled)
	{
		--sc_colorsScrolled;
		platRollColours();
	}
	if(++sc_linesScrolled == 8)
	{
		sc_linesScrolled = 0;
		if(sc_rasterLineMulti > 8)
			sc_rasterLineMulti -= 8;
		if(sc_rasterLineMono > 16)
			sc_rasterLineMono -= 8;
		memset(&Screen[si_charScreenStart[sc_colorsScrolled/CharHeight]], (COLOR_WHITE<<4)+COLOR_BLACK, ScreenX);
	}
}

void platRollColours()
{
	char screenColours, colourColours, top;
	int size;
	
	if(sc_colorsScrolled > LogoRows)
	{
		size = LogoRows / CharHeight;
		top = (sc_colorsScrolled / CharHeight) - size;
	}
	else
	{
		size = sc_colorsScrolled / CharHeight;
		top = 0;
	}
	size = size * ScreenX;
	if(++sc_scrollColourIndex > 2)
		sc_scrollColourIndex = 0;
	screenColours = (cc_scrollColours[sc_scrollColourIndex] << 4) + cc_scrollColours[sc_scrollColourIndex+1];
	colourColours = cc_scrollColours[sc_scrollColourIndex+2];
	memset(&Screen[si_charScreenStart[top]], screenColours, size);
	memset(&Colour[si_charScreenStart[top]], colourColours, size);
}

void platShutDown()
{
	platRemoveRaster();
	platMultiModeOff();
	platGraphicsOff();
	platVICBankNormal();
}

char platReadGameKeys(char currDir)
{
	char ch;
	
	if(!kbhit())
		return currDir;
		
	ch = cgetc();
	if(',' == ch)
	{
		if(--currDir > gCHAR_SnakeHeadLeft)
			currDir = gCHAR_SnakeHeadLeft;
	}
	else if('.' == ch)
	{
		if(++currDir > gCHAR_SnakeHeadLeft)
			currDir = gCHAR_SnakeHeadUp;
	}
	else if('i' == ch || 'a' == ch || 145 == ch)
		currDir = gCHAR_SnakeHeadUp;
	else if('m' == ch || 'z' == ch || 17 == ch)
		currDir = gCHAR_SnakeHeadDown;
	else if('j' == ch || ':' == ch || 'l' == ch || 157 == ch)
		currDir = gCHAR_SnakeHeadLeft;
	else if('k' == ch || ';' == ch || 29 == ch)
		currDir = gCHAR_SnakeHeadRight;

	return currDir;
}

char platReadFEKeys()
{
	if(!kbhit())
		return 0;

	return cgetc();
}

char platReadSpaceKey()
{
	if(!kbhit())
		return 0;

	return ' ' == cgetc();
}

void platFlushKeys()
{
	while(kbhit())
		cgetc();
}
