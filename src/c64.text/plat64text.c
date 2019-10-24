/*
 *  plat64text.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include <string.h>
#include <conio.h>
#include "../globals.h"
#include "../plat.h"
#include "logo64text.h"
#include "../frontend.h"

#define Screen				((char*)(0x0400))
#define Colour				((char*)0xd800)
#define GridDisplayHeight	(ScreenY-3)
#define BarFilled			224
#define BarEmpty			102
#define BarTicks			((TicksToApple + ScreenX) / ScreenX)
#define BarWidth			(TicksToApple / BarTicks)
#define BarLeftEdge			(ScreenX - BarWidth) / 2
#define SnakeStartSpeed		0xfca8
#define SnakeSpeedIncrease	0x08

void platscrollColours(char end);
void platShowLogo(char row);

#define platSetBorderColour(b)	VIC.bordercolor = b
#define platSetBackColour(b)	VIC.bgcolor0 = b

static char sc_screenTop;
static char sc_screenBottom;
static bool sb_syncActive;
static int si_charScreenStart[ScreenY];
static int si_syncSpeed;

const char cc_showCharacter[] =
{
	81, // gCHAR_SnakeHeadUp,
	81, // gCHAR_SnakeHeadRight,	
	81, // gCHAR_SnakeHeadDown, 
	81, // gCHAR_SnakeHeadLeft,

	66, // gCHAR_SnakeBodyUp,
	64, // gCHAR_SnakeBodyRight,
	66, // gCHAR_SnakeBodyDown,
	64, // gCHAR_SnakeBodyLeft,

	85, // gCHAR_SnakeBodyUpRight,
	73, // gCHAR_SnakeBodyRightDown,
	75, // gCHAR_SnakeBodyDownLeft,
	74, // gCHAR_SnakeBodyLeftUp,

	73, // gCHAR_SnakeBodyUpLeft,
	85, // gCHAR_SnakeBodyLeftDown,
	74, // gCHAR_SnakeBodyDownRight,
	75, // gCHAR_SnakeBodyRightUp,

	87, // gCHAR_SnakeTailUp,
	87, // gCHAR_SnakeTailRight,	
	87, // gCHAR_SnakeTailDown, 
	87, // gCHAR_SnakeTailLeft,

	32, // gCHAR_Blank,
	81, // gCHAR_Apple,
	81, // gCHAR_Plum,
	33, // gCHAR_Exit,
	246, // gCHAR_ExitMarker
	224, // gCHAR_Wall1,
	224, // gCHAR_Wall2,
	224, // gCHAR_Wall3,
	102, // gCHAR_Wall4,
	102, // gCHAR_Wall5,
	102, // gCHAR_Wall6,
	102, // gCHAR_Wall7,
	102, // gCHAR_Wall8,
	209, // gCHAR_Wall9,
	224, // gCHAR_Wall10,
};
const char cc_showColour[] =
{
	COLOR_GREEN,	// gCHAR_SnakeHeadUp,
	COLOR_GREEN,	// gCHAR_SnakeHeadRight,	
	COLOR_GREEN,	// gCHAR_SnakeHeadDown,	
	COLOR_GREEN,	// gCHAR_SnakeHeadLeft,

	COLOR_GREEN,	// gCHAR_SnakeBodyUp,
	COLOR_GREEN,	// gCHAR_SnakeBodyRight,
	COLOR_GREEN,	// gCHAR_SnakeBodyDown,
	COLOR_GREEN,	// gCHAR_SnakeBodyLeft,

	COLOR_GREEN,	// gCHAR_SnakeBodyUpRight,
	COLOR_GREEN,	// gCHAR_SnakeBodyRightDown,
	COLOR_GREEN,	// gCHAR_SnakeBodyDownLeft,
	COLOR_GREEN,	// gCHAR_SnakeBodyLeftUp,

	COLOR_GREEN,	// gCHAR_SnakeBodyUpLeft,
	COLOR_GREEN,	// gCHAR_SnakeBodyLeftDown,
	COLOR_GREEN,	// gCHAR_SnakeBodyDownRight,
	COLOR_GREEN,	// gCHAR_SnakeBodyRightUp,

	COLOR_GREEN,	// gCHAR_SnakeTailUp,
	COLOR_GREEN,	// gCHAR_SnakeTailRight,	
	COLOR_GREEN,	// gCHAR_SnakeTailDown,	
	COLOR_GREEN,	// gCHAR_SnakeTailLeft,

	COLOR_BLACK,	// gCHAR_Blank,
	COLOR_RED,		// gCHAR_Apple,
	COLOR_PURPLE,	// gCHAR_Plum,
	COLOR_BLACK,	// gCHAR_Exit,
	COLOR_RED,		// gCHAR_ExitMarker
	COLOR_PURPLE,	// gCHAR_Wall1,				(Solid Purple)
	COLOR_GREEN,	// gCHAR_Wall2,             (Solid Green)
	COLOR_RED,		// gCHAR_Wall3,             (Solid Red)
	COLOR_CYAN,		// gCHAR_Wall4,             (Purple / Black chequer)
	COLOR_BLUE,		// gCHAR_Wall5,             (Green / Black chequer)
	COLOR_LIGHTGREEN,	// gCHAR_Wall6,         (Purple / Green chequer)
	COLOR_LIGHTRED,	// gCHAR_Wall7,             (Purple / Red chequer)
	COLOR_ORANGE,	// gCHAR_Wall8,             (Green / Red chequer)
	COLOR_BROWN,	// gCHAR_Wall9,             (purple/green box)
	COLOR_PURPLE,	// gCHAR_Wall10,            (Solid Purple but orange/brown is 1st 2 channels)
};

void platInitialSetup()
{
	int x, y;
	
	x = 0;
	for(y = 0; y < ScreenY; ++y)
	{
		si_charScreenStart[y] = x;
		x += ScreenX;
	}
	
	platSetBackColour(COLOR_BLACK);
	platSetBorderColour(COLOR_BLACK);
	
	platSyncGameSpeed();
	CIA1.cra = (CIA1.cra & 0xc0) | 0x10;
	CIA1.crb |= 0x40;
}

void platSetupGame()
{

}

void platSetupFrontend(char row)
{
	sc_screenTop = 0;
	sc_screenBottom = GridDisplayHeight;
	platClearScreen();
	platShowLogo(row);
}

void platSetupLevel()
{
	sc_screenTop = GridY - GridDisplayHeight;
	sc_screenBottom = sc_screenTop + GridDisplayHeight;
	platClearScreen();
}

void platBeginFrame()
{
	CIA1.crb &= 0xfe;
	CIA1.cra &= 0xfe;
	CIA1.ta_lo = 0xff;
	CIA1.tb_lo = 0xff;
	CIA1.ta_hi = 0x7f;
	CIA1.tb_hi = 0x7f;
	CIA1.crb |= 1;
	CIA1.cra |= 1;
}

void platSyncEndFrame()
{
	int y, x, copy;
	char byte, sY = gt_snake.y[gt_snake.head];
	
	if(gc_gameState != gSTATE_Frontend || feSTATE_DemoLoop == gc_feState)
	{							 
		if(sY >= GridDisplayHeight/2)
		{
			sY -= GridDisplayHeight/2;
			if(sY + GridDisplayHeight >= GridY)
				sY = GridY - GridDisplayHeight;
		}
		else
			sY = 0;
			
		if(sc_screenTop != sY)
		{
			if(sY > sc_screenTop)
			{
				// memmove(Screen, Screen+ScreenX, (GridDisplayHeight-1)*ScreenX);
				// memmove(Colour, Colour+ScreenX, (GridDisplayHeight-1)*ScreenX);
				platscrollColours(0x48);
				
				y = GridDisplayHeight - 1;
				copy = sY + GridDisplayHeight - 1;
			}
			else
			{
				// memmove(Screen+ScreenX, Screen, (GridDisplayHeight-1)*ScreenX);
				// memmove(Colour+ScreenX, Colour, (GridDisplayHeight-1)*ScreenX);
				asm("ldx #$48");
				
				mll1:
				asm("dex");
				asm("lda $0700,x");
				asm("sta $0728,x");
				asm("lda $db00,x");
				asm("sta $db28,x");
				asm("cpx #$0");
				asm("bne %g",mll1);
				
				mll2:
				asm("dex");
				asm("lda $0600,x");
				asm("sta $0628,x");
				asm("lda $da00,x");
				asm("sta $da28,x");
				asm("cpx #$0");
				asm("bne %g", mll2);
				
				mll3:
				asm("dex");
				asm("lda $0500,x");
				asm("sta $0528,x");
				asm("lda $d900,x");
				asm("sta $d928,x");
				asm("cpx #$0");
				asm("bne %g", mll3);
				
				mll4:
				asm("dex");
				asm("lda $0400,x");
				asm("sta $0428,x");
				asm("lda $d800,x");
				asm("sta $d828,x");
				asm("cpx #$0");
				asm("bne %g", mll4);

				y = 0;
				copy = sY;
			}

			for(x = 0; x < GridX; ++x)
			{
				byte = gc_grid[gi_gridStart[copy]+x];
				Screen[si_charScreenStart[y]+x] = cc_showCharacter[byte];
				Colour[si_charScreenStart[y]+x] = cc_showColour[byte];
			}

			sc_screenTop = sY;
			sc_screenBottom = sY + GridDisplayHeight;
		}
	}

	if(sb_syncActive)
	{
		while(*((int*)&CIA1.ta_hi) > si_syncSpeed);
	}
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
	clrscr();
	memset(Colour, COLOR_WHITE, ScreenX*ScreenY);
}

void platPlot(char gx, char gy, char gv)
{
	if(sc_screenTop <= gy && gy < sc_screenBottom)
	{
		gy -= sc_screenTop;
		Screen[si_charScreenStart[gy]+gx] = cc_showCharacter[gv];
		Colour[si_charScreenStart[gy]+gx] = cc_showColour[gv];
	}
}

void platShowTextAligned(char textPosX, char textPosY, const char *szText)
{
	char character, index = 0;
	int textPos = si_charScreenStart[textPosY] + textPosX;
	
	character = szText[index];
	while(character)
	{
		if(character >= 'a' - 1)
			character -= 'a' - 1;

		Screen[textPos] = character;
		
		++textPos;
		character = szText[++index];
	}
}

void platShowTextSliver(char textPosY, char sliver, const char *szText)
{
	if(!sliver)
		platShowTextAligned(0, textPosY, szText);
}

void platPrepAppleTicksBar()
{
}

void platShowTicksToNextApple(char)
{
	static char sc_oldTicks = 0;
	
	if(sc_oldTicks < gc_nextAppleTicks && gc_nextAppleTicks - sc_oldTicks < BarTicks)
		return ;
	else	
	{
		char i, value = BarLeftEdge + /*BarWidth -*/ (gc_nextAppleTicks / BarTicks);
		
		for(i = BarLeftEdge; i < value; ++i)
			(Screen+((ScreenY-3)*ScreenX))[i] = BarFilled;
			
		for( ; value < BarLeftEdge + BarWidth; ++value)
			(Screen+((ScreenY-3)*ScreenX))[value] = BarEmpty;
			
		sc_oldTicks = gc_nextAppleTicks;
	}
}

void platShowLogo(char )
{
	char x, y, s, l;
	const char *c;
	
	s = (40 - strlen(cc_snakeLogo[0])) / 2;
	for(y = 0; y < cc_numLogoLines; ++y)
	{
		platShowTextAligned(s, 1+y, cc_snakeLogo[y]);
		for(x = 0, c = cc_snakeLogo[y]; *c; ++x, ++c)
		{
			switch(*c)
			{
				case ' ':
					continue;
				break;
				
				case 'o':
					l = COLOR_GREEN;
				break;

				case 'n':
					l = COLOR_BLUE;
				break;

				case 'a':
					l = COLOR_RED;
				break;

				case 'k':
					l = COLOR_PURPLE;
				break;

				case 'e':
					l = COLOR_CYAN;
				break;

				case 'b':
					l = COLOR_ORANGE;
				break;

				case 'y':
					l = COLOR_YELLOW;
				break;

				case 't':
					l = COLOR_LIGHTRED;
				break;
			}
			Colour[si_charScreenStart[1+y]+s+x] = l;
		}
	}

}

void platScrollScreenUp()
{
	static char sc_count = 0;
	if(CharHeight == ++sc_count)
	{
		// memmove(Screen, Screen+ScreenX, (ScreenY-1) * ScreenX);
		// memmove(Colour, Colour+ScreenX, (ScreenY-1) * ScreenX);
		platscrollColours(0xc0);
		sc_count = 0;
	}
}

void platRollColours()
{
}

void platscrollColours(char end)
{
	asm("ldx #0");
	
	ml1:
	asm("lda $0428,x");
	asm("sta $0400,x");
	asm("lda $d828,x");
	asm("sta $d800,x");
	asm("inx");
	asm("bne %g", ml1);
	
	ml2:
	asm("lda $0528,x");
	asm("sta $0500,x");
	asm("lda $d928,x");
	asm("sta $d900,x");
	asm("inx");
	asm("bne %g", ml2);
	
	ml3:
	asm("lda $0628,x");
	asm("sta $0600,x");
	asm("lda $da28,x");
	asm("sta $da00,x");
	asm("inx");
	asm("bne %g", ml3);
	
	asm("ldy #%o", end);
	asm("lda (sp),y");
	asm("sta tmp1");
	
	ml4:
	asm("lda $0728,x");
	asm("sta $0700,x");
	asm("lda $db28,x");
	asm("sta $db00,x");
	asm("inx");
	asm("cpx tmp1");
	asm("bne %g",ml4);
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
