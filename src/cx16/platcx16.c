/*
 *  platcx16.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *  cx16 platform added Oct 2019
 *
 */

#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <cx16.h>
#include "../globals.h"
#include "../plat.h"
#include "logocx16.h"
#include "../frontend.h"

#define SnakeStartSpeed			32
#define SnakeSpeedIncrease		0x01
#define	GridRowHeight			5
#define	GridRowWidth			4
#define	AppleTicksCycleLength	10
#define clk_l 					(*(volatile unsigned char *)0xa2) // it emu r33 this is the fast clock tick location

// forward declarations (not needed) of functions added here that were not in plat.h
void vaddress(unsigned char hi, unsigned short address);
void vpoke(unsigned char hi, unsigned short address, unsigned char byte);
void platShowLogo();

static bool sb_syncActive;
static int si_syncSpeed;
static char sc_scrollOffset;
unsigned char sc_colourCycleIndex;
static unsigned char timePixels;

const unsigned short gfx_address = 0x4000;
const char cc_appleTicksCycleSpeed[AppleTicksCycleLength] = {8, 8, 7, 7, 6, 6, 5, 5, 4, 3};

// format is gr, 0b for rbg colours, indexed from 0
const char cc_palette[] = 
{
	0x00, 0x00,	// Black
	0xe5, 0x09, // Dark Green
	0x65, 0x0b, // Brown-Red 
	0x6f, 0x0b, // Purple 
	0x93, 0x0c, // Orange-brown
	0xff, 0x0f, // White
	0xf7, 0x0f, // Yellow
	0xe5, 0x09, // Dark Green
	0xf9, 0x0d, // Light Green
	0x70, 0x08, // Dark Brown
};
const unsigned char cc_pallette_len = sizeof(cc_palette) / sizeof(cc_palette[0]);

// the graphics that make up the snake, etc.  In this mode ($c0, 4bpp) each byte is 2 pixels so 
// a traditional column of 8 pixels on the c64 is now 4 bytes wide.  Lots of doubling up as 
// every pixel was stretched to make this data
const char cc_pixels[][GridRowHeight*GridRowWidth] =
{
	{0x00, 0x22, 0x22, 0x00, 0x00, 0x22, 0x22, 0x00, 0x11, 0x22, 0x22, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},	// gCHAR_SnakeHeadUp
	{0x22, 0x11, 0x00, 0x00, 0x22, 0x11, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x11, 0x00, 0x00},	// gCHAR_SnakeHeadRight
	{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x11, 0x22, 0x22, 0x11, 0x00, 0x22, 0x22, 0x00, 0x00, 0x22, 0x22, 0x00},	// gCHAR_SnakeHeadDown
	{0x00, 0x00, 0x11, 0x22, 0x00, 0x00, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x11, 0x22},	// gCHAR_SnakeHeadLeft
	{0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11},	// gCHAR_SnakeBodyUp
	{0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},	// gCHAR_SnakeBodyRight
	{0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11},	// gCHAR_SnakeBodyDown
	{0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},	// gCHAR_SnakeBodyLeft
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11},	// gCHAR_SnakeBodyUpRight
	{0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00},	// gCHAR_SnakeBodyRightDown
	{0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x00, 0x00},	// gCHAR_SnakeBodyDownLeft
	{0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x00, 0x11},	// gCHAR_SnakeBodyLeftUp
	{0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00},	// gCHAR_SnakeBodyUpLeft
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11},	// gCHAR_SnakeBodyLeftDown
	{0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x00, 0x11},	// gCHAR_SnakeBodyDownRight
	{0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x00, 0x00},	// gCHAR_SnakeBodyRightUp
	{0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00},	// gCHAR_SnakeTailUp
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x11},	// gCHAR_SnakeTailRight
	{0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11},	// gCHAR_SnakeTailDown
	{0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x00, 0x00, 0x00},	// gCHAR_SnakeTailLeft
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	// gCHAR_Blank
	{0x00, 0x22, 0x22, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x22, 0x22, 0x00},	// gCHAR_Apple
	{0x00, 0x33, 0x33, 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x33, 0x33, 0x00},	// gCHAR_Plum
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	// gCHAR_Exit
	{0x33, 0x33, 0x22, 0x22, 0x33, 0x33, 0x22, 0x22, 0x33, 0x33, 0x22, 0x22, 0x33, 0x33, 0x22, 0x22, 0x33, 0x33, 0x22, 0x22},	// gCHAR_ExitMarker
	{0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33},	// gCHAR_Wall1 (Solid Purple)
	{0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11},	// gCHAR_Wall2 (Solid Green)
	{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},	// gCHAR_Wall3 (Solid Red)
	{0x33, 0x00, 0x33, 0x00, 0x00, 0x33, 0x00, 0x33, 0x33, 0x00, 0x33, 0x00, 0x00, 0x33, 0x00, 0x33, 0x33, 0x00, 0x33, 0x00},	// gCHAR_Wall4 (Purple / Black chequer)
	{0x11, 0x00, 0x11, 0x00, 0x00, 0x11, 0x00, 0x11, 0x11, 0x00, 0x11, 0x00, 0x00, 0x11, 0x00, 0x11, 0x11, 0x00, 0x11, 0x00},	// gCHAR_Wall5 (Green / Black chequer)
	{0x33, 0x11, 0x33, 0x11, 0x11, 0x33, 0x11, 0x33, 0x33, 0x11, 0x33, 0x11, 0x11, 0x33, 0x11, 0x33, 0x33, 0x11, 0x33, 0x11},	// gCHAR_Wall6 (Purple / Green chequer)
	{0x33, 0x22, 0x33, 0x22, 0x22, 0x33, 0x22, 0x33, 0x33, 0x22, 0x33, 0x22, 0x22, 0x33, 0x22, 0x33, 0x33, 0x22, 0x33, 0x22},	// gCHAR_Wall7 (Purple / Red chequer)
	{0x11, 0x22, 0x11, 0x22, 0x22, 0x11, 0x22, 0x11, 0x11, 0x22, 0x11, 0x22, 0x22, 0x11, 0x22, 0x11, 0x11, 0x22, 0x11, 0x22},	// gCHAR_Wall8 (Green / Red chequer)
	{0x33, 0x33, 0x33, 0x33, 0x33, 0x11, 0x11, 0x33, 0x33, 0x11, 0x11, 0x33, 0x33, 0x11, 0x11, 0x33, 0x33, 0x33, 0x33, 0x33},	// gCHAR_Wall9 (purple/green box)
	{0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33},	// gCHAR_Wall10 (Solid Purple but orange/brown is 1st 2 channels)
};

// Set the vera address
void vaddress(unsigned char hi, unsigned short address)
{
	VERA.address_hi = hi;
	VERA.address = address;
}

// Write a byte to data 0 for the address
void vpoke(unsigned char hi, unsigned short address, unsigned char byte)
{
	vaddress(hi, address);
	VERA.data0 = byte;
}

void platInitialSetup()
{
	int y;

	// switch back to uppercase
	__asm__("lda #142");
	__asm__("jsr BSOUT");

	// Set screen to 40 x 25
	vpoke(15, 1, 0x40);
	vpoke(15, 2, 0x36);

	// Hide layer 1
	vpoke(15, 0x3000, 0xc0);

	// Set layer 1 memory to $4000
	vpoke(15, 0x3005, (gfx_address >> 10));

	// Clear the memory (screen)
	platClearScreen();

	// Show Layer 1
	vpoke(15, 0x3000, 0xc1);

	// Enable waiting for the raster
	VERA.irq_enable |= 1;

	// Install the palette
	for(y = 0; y<cc_pallette_len; y++)
		vpoke(15, 0x1000+y, cc_palette[y]);

	// Set the speed at which to play
	platSyncGameSpeed();
}

void platSetupGame()
{
}

void platSetupFrontend(char )
{
	platClearScreen();
	platShowLogo();
}

void platSetupLevel()
{
	platClearScreen();
}

void platBeginFrame()
{
	// set the clock to 0 to time the frame
	clk_l = 0;
}

void platSyncEndFrame()
{
	if(!sb_syncActive)
		return;

	// hold here till it's time for the next frame
	while(clk_l < si_syncSpeed/4);
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
	si_syncSpeed -= SnakeSpeedIncrease;
}

void platClearScreen()
{
	// clear layer 0 with the conio commands
	bgcolor(0);
	textcolor(5);
	clrscr();

	// reset the vscroll
	sc_scrollOffset = 0;
	vpoke(15, 0x2008,sc_scrollOffset);

	// clear layer 1 graphics in assembly
	__asm__("ldy #208");

	__asm__("lda #0");
	__asm__("sta tmp1");
	__asm__("lda #$40");
	__asm__("sta tmp2");

	__asm__("lda #$10");
	__asm__("sta $9f22");

loopo:
	__asm__("lda tmp1");
	__asm__("sta $9f20");
	__asm__("lda tmp2");
	__asm__("sta $9f21");

	__asm__("ldx #160");
	__asm__("lda #0");

loopi:
	__asm__("sta $9f23");
	__asm__("dex");
	__asm__("bne %g", loopi);
	__asm__("dey");
	__asm__("beq %g", done);
	__asm__("lda tmp1");
	__asm__("clc");
	__asm__("adc #160");
	__asm__("sta tmp1");
	__asm__("bcc %g", loopo);
	__asm__("inc tmp2");
	__asm__("bne %g", loopo);
done: ;
}

void platPlot(char gx, char gy, char gv)
{
	char i, j;
	short address = gfx_address + (gy * 5 * 160) + (gx * GridRowWidth);

	// copy the contents of a cell into layer 1
	for(i = 0; i < GridRowHeight*GridRowWidth; i += GridRowWidth )
	{
		for(j = 0; j < GridRowWidth ; j++)
		{
			vpoke(0, address+j, cc_pixels[gv][i+j]);
		}
		address += 160;
	}

	if(gv <= gCHAR_Blank)
	{
		if(gv <= gCHAR_SnakeHeadLeft)
		{
			// The Snake head is 5 rows high when going left/right so poke 
			// the 6th row in
			if(gv == gCHAR_SnakeHeadLeft || gv == gCHAR_SnakeHeadRight)
			{
				// The 6th row is just a copy of the 5th row (row number 4, zero based)
				i = (GridRowHeight-1)*GridRowWidth;
				for(j = 0; j < GridRowWidth ; j++)
				{
					vpoke(0, address+j, cc_pixels[gv][i+j]);
				}
				address += 160;
			}
		}
		else
		{
			// Go to the column below and fix its top row, in case it was 
			// overwritten by the 6th row of the snake head
			++gy;
			gv = gc_grid[gi_gridStart[gy]+gx];
			for(j = 0; j < GridRowWidth ; j++)
			{
				// copy the top row of the tile over itself
				vpoke(0, address+j, cc_pixels[gv][j]);
			}
		}
	}

}

void platShowTextAligned(char textPosX, char textPosY, const char *szText)
{
	// all text done with conio to layer 0
	gotoxy(textPosX, textPosY);
	cprintf("%s", szText);
}

void platShowTextSliver(char textPosY, char sliver, const char *szText)
{
	// if a whole line of text is now "visible"
	if(!sliver)
	{
		// Scroll the screen data up, and reset the VERA.VScroll
		// text memory is at address 0
		__asm__("lda #0");
		__asm__("sta tmp1");

		// calculate how many rows to scroll
		__asm__("lda %v", sc_scrollOffset);
		__asm__("lsr");
		__asm__("lsr");
		__asm__("lsr");
		__asm__("sta tmp2");
		
		// This is time-critical or you end up seeing the
		// scrolled image flash for a brief second, at the top
		// so all done in asm to minimize time taken
		// Wait for a vertical blank
vbl:
		__asm__("lda $9f27");
		__asm__("bne %g", vbl);

		// reset the vscroll 
		__asm__("lda #$0f");
		__asm__("sta $9f22");
		__asm__("lda #$08");
		__asm__("sta $9f20");
		__asm__("lda #$20");
		__asm__("sta $9f21");
		__asm__("lda #0");
		__asm__("sta $9f23");

		// set data0 to point at 0 and data1 to point at tmp2, with a stride of 2 (skip the color)
		__asm__("lda #$20");
		__asm__("sta $9f22");
		__asm__("lda #0");
		__asm__("sta $9f20");
		__asm__("sta $9f21");
		__asm__("lda #1");
		__asm__("sta $9f25");
		__asm__("lda #$20");
		__asm__("sta $9f22 ");
		__asm__("lda #0");
		__asm__("sta $9f20");
		__asm__("lda tmp2");
		__asm__("sta $9f21");
		__asm__("lda #0");
		__asm__("sta $9f25");

		// rows to scroll in Y
		// the screen size reports 80x60 - it's just not looking at the
		// 0x0f0001 value, so hard-coded 25 here
		__asm__("ldy #25"); 

loopo:
		// columns to scroll in x
		__asm__("ldX #40");

loopi:
		// copy the lower (on screen) row over the higher row (scroll up)
		__asm__("lda $9f24");
		__asm__("sta $9f23");

		// do for all columns
		__asm__("dex");
		__asm__("bne %g", loopi);

		// see if all rows were done
		__asm__("dey");
		__asm__("beq %g", done);

		// increment the row (both dst and src - they are $100 apart)
		__asm__("inc tmp1");
		__asm__("inc tmp2");

		// update the data0 and data1 address lines
		__asm__("lda #0");
		__asm__("sta $9f20");

		__asm__("lda tmp1");
		__asm__("sta $9f21");

		__asm__("lda #1");
		__asm__("sta $9f25");

		__asm__("lda #0");
		__asm__("sta $9f20");

		__asm__("lda tmp2");
		__asm__("sta $9f21");

		__asm__("lda #0");
		__asm__("sta $9f25");

		// a = 0 so jmp back to outer loop
		__asm__("beq %g", loopo);

done:
		sc_scrollOffset = 0;

		// Show the next line of text, but off-screen
		platShowTextAligned(0, textPosY + 3, szText);
	}
}

void platPrepAppleTicksBar()
{
	platShowTicksToNextApple(YES);
}


void platShowTicksToNextAppleColumn(char column, char y1)
{
	unsigned char r;
	unsigned int offseta, offsetb, top;
	const unsigned char colPixels[] = {0x44, 0x99, 0x33}; // orange-brown, dark-brown and purple pattern

	// Start at 20th char row, 8 pixel rows per char row, 160 bytes per row and 4 bytes per character column  
	offseta = gfx_address + 20 * 8 * 160 + column * 4;
	offsetb = offseta - ((20-y1) * 8 * 160);
	top = gfx_address + 16 + column * 4; // row 2 (16 pixels from the top) + the column offset

	// Draw the time-taken bar in alternating browns
	for(; offseta > offsetb; )
	{
		// This is a lazy way of getting a local variable into the accumulator
		// make it global and cache the result in the tmp variables
		timePixels = colPixels[sc_colourCycleIndex];
		__asm__("lda %v", timePixels);
		__asm__("sta tmp1");
		timePixels = colPixels[1-sc_colourCycleIndex];
		__asm__("lda %v", timePixels);
		__asm__("sta tmp2");
		timePixels = colPixels[2];
		__asm__("lda %v", timePixels);
		__asm__("sta tmp3");

		// Alternate dark/light every 2 pixel rows
		for(r=0; r<2; r++)
		{
			// This is significantly faster that vpoke'ing the values.
			// could be made much faster still but this is good enough
			// the speed does however affect how quickly the snake die
			// plays out since this is called rapidly as the snake
			// shrivels
			vaddress(0x10, offseta);
			__asm__("lda tmp1");
			__asm__("sta $9f23");
			__asm__("sta $9f23");
			__asm__("sta $9f23");
			__asm__("sta $9f23");
			offseta -= 160;
		}

		// the second set in the other colour
		timePixels = colPixels[1-sc_colourCycleIndex];
		for(r=0; r<2; r++)
		{
			vaddress(0x10, offseta);
			__asm__("lda tmp2");
			__asm__("sta $9f23");
			__asm__("sta $9f23");
			__asm__("sta $9f23");
			__asm__("sta $9f23");
			offseta -= 160;
		}
	}

	// fill the row to the top in purple
	timePixels = colPixels[2];
	for(; offseta > top; )
	{
		vaddress(0x10, offseta);
		__asm__("lda tmp3");
		__asm__("sta $9f23");
		__asm__("sta $9f23");
		__asm__("sta $9f23");
		__asm__("sta $9f23");
		offseta -= 160;
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

		sc_colourCycleIndex = 1 - sc_colourCycleIndex;

		sc_localTicks = 0;
	}
}

// the logo is hard-wired to show in row 16, centered
// at gfx address $4a20
void platShowLogo()
{
	// point at the logo
	__asm__("lda #<%v", cc_snakeLogo);
	__asm__("sta ptr1");
	__asm__("lda #>%v", cc_snakeLogo);
	__asm__("sta ptr1+1");

	__asm__("lda #<$4a20");
	__asm__("sta ptr2");
	__asm__("lda #>$4a20");
	__asm__("sta ptr2+1");

	// set up rows to copy
	__asm__("lda #83"); // LogoRows
	__asm__("sta tmp2");

	// set up vera
	__asm__("lda #$10");
	__asm__("sta $9f22");
loopo:
	__asm__("lda ptr2+1");
	__asm__("sta $9f21");
	__asm__("lda ptr2");
	__asm__("sta $9f20");
	__asm__("ldx #95"); // LogoCols
	__asm__("ldy #0");
loopi:
	__asm__("lda (ptr1),y");
	__asm__("sta $9f23");
	__asm__("iny");
	__asm__("dex");
	__asm__("bne %g", loopi);
	__asm__("dec tmp2");
	__asm__("beq %g", done);
	__asm__("tya");
	__asm__("clc");
	__asm__("adc ptr1");
	__asm__("sta ptr1");
	__asm__("bcc %g", step);
	__asm__("inc ptr1+1");
step:
	__asm__("lda #160");
	__asm__("clc");
	__asm__("adc ptr2");
	__asm__("sta ptr2");
	__asm__("bcc %g", loopo);
	__asm__("inc ptr2+1");
	__asm__("bne %g", loopo);
done: ;
}

void platScrollScreenUp()
{
	// Scroll layer 0 (text) with the vscroll
	sc_scrollOffset++;
	vpoke(15, 0x2008, sc_scrollOffset);

	// properly scroll layer 1 with vera
	__asm__("lda #<$4020");
	__asm__("sta ptr1");
	__asm__("lda #>$4020");
	__asm__("sta ptr1+1");

	__asm__("lda #<$40c0");
	__asm__("sta ptr2");
	__asm__("lda #>$40c0");
	__asm__("sta ptr2+1");

	// set up rows to scroll
	__asm__("ldy #100"); // LogoRows + top of screen to logo (16 lines) + 1 blank line

	// set up vera
	__asm__("lda #$10");
	__asm__("sta $9f22");
	// switch to data1
	__asm__("lda #1");
	__asm__("sta $9f25");
	__asm__("lda #$10");
	__asm__("sta $9f22");
loopo:
	__asm__("lda ptr2+1");
	__asm__("sta $9f21");
	__asm__("lda ptr2");
	__asm__("sta $9f20");
	// switch to data0
	__asm__("lda #0");
	__asm__("sta $9f25");
	__asm__("lda ptr1+1");
	__asm__("sta $9f21");
	__asm__("lda ptr1");
	__asm__("sta $9f20");
	// setup for cols to scroll
	__asm__("ldx #95"); // LogoCols
loopi:
	__asm__("lda $9f24");
	__asm__("sta $9f23");
	__asm__("dex");
	__asm__("bne %g", loopi);
	__asm__("dey");
	__asm__("beq %g", done);

	__asm__("lda #1");
	__asm__("sta $9f25");

	__asm__("lda #160");
	__asm__("clc");
	__asm__("adc ptr1");
	__asm__("sta ptr1");
	__asm__("bcc %g", step1);
	__asm__("inc ptr1+1");
step1:
	__asm__("lda #160");
	__asm__("clc");
	__asm__("adc ptr2");
	__asm__("sta ptr2");
	__asm__("bcc %g", loopo);
	__asm__("inc ptr2+1");
	__asm__("bne %g", loopo);

done:
	// the logo colours cycle during the scroll too
	platRollColours();
}

void platRollColours()
{
	static unsigned char offset = 0;
	unsigned char i;

	// 3 colors, 2 bytes each = 6, at an index 12 (6 color into palette)
	for(i=0;i<6;i++)
	{
		vpoke(15, 0x100c+i, cc_palette[12+((i+offset) % 6)]);
	}
	offset += 2;
}

void platShutDown()
{
	platClearScreen();
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
