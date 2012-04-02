/*
 *  platconio.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "../globals.h"
#include "../plat.h"
#include "logoconio.h"
#include "../frontend.h"

#define BarFilled					'='
#define BarEmpty					'-'
#define SnakeStartSpeed				0xff
#define SnakeSpeedIncrease			0x01
#define NumHudLines					3
#define GridBufferSize				GridX*GridY
#define TextBufferSize				ScreenX*ScreenY

void platShowLogo(char row);

static char sc_gridBuffer[GridBufferSize];
static char sc_textBuffer[TextBufferSize];
static char sc_screenX;
static char sc_screenY;
static char sc_barTicks;
static char sc_barWidth;
static char sc_barLeftEdge;
static char sc_screenOffsetTop;
static char sc_screenOffsetLeft;
static char sc_bufferScreenHeight;
static bool sb_syncActive;
static int si_backBufferRowStart[GridY];
static int si_syncSpeed;

const char cc_showCharacter[] =
{
	'*', // gCHAR_SnakeHeadUp,
	'*', // gCHAR_SnakeHeadRight,	
	'*', // gCHAR_SnakeHeadDown, 
	'*', // gCHAR_SnakeHeadLeft,

	'O', // gCHAR_SnakeBodyUp,
	'O', // gCHAR_SnakeBodyRight,
	'O', // gCHAR_SnakeBodyDown,
	'O', // gCHAR_SnakeBodyLeft,

	'O', // gCHAR_SnakeBodyUpRight,
	'O', // gCHAR_SnakeBodyRightDown,
	'O', // gCHAR_SnakeBodyDownLeft,
	'O', // gCHAR_SnakeBodyLeftUp,

	'O', // gCHAR_SnakeBodyUpLeft,
	'O', // gCHAR_SnakeBodyLeftDown,
	'O', // gCHAR_SnakeBodyDownRight,
	'O', // gCHAR_SnakeBodyRightUp,

	'o', // gCHAR_SnakeTailUp,
	'o', // gCHAR_SnakeTailRight,	
	'o', // gCHAR_SnakeTailDown, 
	'o', // gCHAR_SnakeTailLeft,

	' ', // gCHAR_Blank,
	'a', // gCHAR_Apple,
	'p', // gCHAR_Plum,
	'.', // gCHAR_Exit,
	'|', // gCHAR_ExitMarker
	'X', // gCHAR_Wall1,
	'x', // gCHAR_Wall2,
	'#', // gCHAR_Wall3,
	'$', // gCHAR_Wall4,
	'%', // gCHAR_Wall5,
	'&', // gCHAR_Wall6,
	'+', // gCHAR_Wall7,
	'8', // gCHAR_Wall8,
	'=', // gCHAR_Wall9,
	'X', // gCHAR_Wall10,
};

void platInitialSetup()
{
	int y, x = 0;
	
	screensize(&sc_screenX, &sc_screenY);

	sc_bufferScreenHeight = sc_screenY - NumHudLines;
	sc_barTicks = ((TicksToApple + sc_screenX) / sc_screenX);
	sc_barWidth = (TicksToApple / sc_barTicks);
	sc_barLeftEdge = (sc_screenX - sc_barWidth) / 2;

	for(y = 0; y < GridY; ++y)
	{
		si_backBufferRowStart[y] = x;
		x += GridX;
	}
	
	platSyncGameSpeed();
}

void platSetupGame()
{
}

void platSetupFrontend(char row)
{
	sc_screenOffsetTop = 0;
	sc_screenOffsetLeft = 0 ;
	platClearScreen();
	platShowLogo(row);
}

void platSetupLevel()
{
	platClearScreen();
}

void platBeginFrame()
{
	if(gc_gameState != gSTATE_Frontend || feSTATE_DemoLoop == gc_feState)
	{
		if(sc_screenX < ScreenX)
		{
			char sX = gt_snake.x[gt_snake.head];

			if(sX >= sc_screenX/2)
			{
				sX -= sc_screenX/2;
				if(sX + sc_screenX >= GridX)
					sX = GridX - sc_screenX;
			}
			else
				sX = 0;
				
			sc_screenOffsetLeft = sX;
		}
		
		if(sc_screenY < GridY + NumHudLines)
		{
			char sY = gt_snake.y[gt_snake.head];

			if(sY >= sc_bufferScreenHeight/2)
			{
				sY -= sc_bufferScreenHeight/2;
				if(sY + sc_bufferScreenHeight >= GridY)
					sY = GridY - sc_bufferScreenHeight;
			}
			else
				sY = 0;
				
			sc_screenOffsetTop = sY;
		}
	}
	else
	{
		
	}
}

void platSyncEndFrame()
{
	char y, *src;
	
	if(gc_gameState != gSTATE_Frontend || feSTATE_DemoLoop == gc_feState)
	{
		src = &sc_gridBuffer[si_backBufferRowStart[sc_screenOffsetTop]+sc_screenOffsetLeft];
		for(y = 0; y < sc_bufferScreenHeight; ++y)
		{
			gotoxy(0,y);
			cprintf("%.*s", sc_screenX, src);
			src += GridX;
		}

		src = &sc_textBuffer[si_backBufferRowStart[ScreenY-NumHudLines]+sc_screenOffsetLeft];
		for(; y < sc_screenY; ++y)
		{
			gotoxy(0,y);
			cprintf("%.*s", sc_screenX, src);
			src += ScreenX;
		}
	}
	else
	{
		src = &sc_textBuffer[si_backBufferRowStart[sc_screenOffsetTop]+sc_screenOffsetLeft];
		for(y = 0; y < sc_screenY; ++y)
		{
			gotoxy(0,y);
			cprintf("%.*s", sc_screenX, src);
			src += ScreenX;
		}
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
	si_syncSpeed -= SnakeSpeedIncrease;
}

void platClearScreen()
{
	clrscr();
	memset(sc_gridBuffer, ' ', GridBufferSize);
	memset(sc_textBuffer, ' ', TextBufferSize);
}

void platPlot(char gx, char gy, char gv)
{
	sc_gridBuffer[si_backBufferRowStart[gy]+gx] = cc_showCharacter[gv];
}

void platShowTextAligned(char textPosX, char textPosY, const char *szText)
{
	char *dst, character, szIndex = 0;
	
	dst = &sc_textBuffer[si_backBufferRowStart[textPosY] + textPosX];
	
	character = szText[szIndex];
	while(character)
	{
		*dst = character;
		++dst;
		character = szText[++szIndex];
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
	
	if(sc_oldTicks < gc_nextAppleTicks && gc_nextAppleTicks - sc_oldTicks < sc_barTicks)
		return ;
	else	
	{
		char x, value = sc_barLeftEdge + /*sc_barWidth -*/ (gc_nextAppleTicks / sc_barTicks);
		char *dst = &sc_textBuffer[si_backBufferRowStart[ScreenY-NumHudLines]+sc_barLeftEdge];
		
		for(x = sc_barLeftEdge; x < value; ++x)
		{
			*dst = BarFilled;
			++dst;
		}
			
		for( ; value < sc_barLeftEdge + sc_barWidth; ++value)
		{
			*dst = BarEmpty;
			++dst;
		}
			
		sc_oldTicks = gc_nextAppleTicks;
	}
}

void platShowLogo(char )
{
	char y, s;
	
	s = (ScreenX - strlen(cc_snakeLogo[0])) / 2;
	for(y = 0; y < cc_numLogoLines; ++y)
		platShowTextAligned(s, 1+y, cc_snakeLogo[y]);

}

void platScrollScreenUp()
{
	static char sc_count = 0;
	if(CharHeight == ++sc_count)
	{
		memmove(sc_textBuffer, sc_textBuffer+ScreenX, (ScreenY-1) * ScreenX);
		sc_count = 0;
	}
}

void platRollColours()
{
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
