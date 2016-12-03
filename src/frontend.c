/*
 *  frontend.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include <stdio.h>
#include "globals.h"
#include "data.h"
#include "plat.h"
#include "game.h"
#include "frontend.h"

#define TextSpacing		2
#define TextStart		18
#define	WelcomeLogoPos	2
#define ScrollLogoPos	7
#define LogoHeight		11
#define WelcomeHoldTime	100
#define ScrollHoldTime	20
#define	PlumTimeout		100

void frontendShowHiScore(void);
void frontendShowLevel(void);
void frontendDemoStep(void);

static char sc_textBuffer[ScreenX];
static char sc_textLine;
static char sc_textRow;
static char sc_linesScrolled;
static char sc_demoLocal;
static int	si_feTicker;

void frontendInitialSetup()
{
	gc_feState = feSTATE_Welcome;
}

bool frontend()
{
	char key = platReadFEKeys();

	if(feSTATE_AskPlums != gc_feState && key)
	{
		if('q' == key || 'Q' == key)
			return gSTATE_AllDone;

		key = 0;
		si_feTicker = 0;
		platClearScreen();
		platShowTextAligned(0, ScreenY-1, cc_numPlumsText);
		gc_feState = feSTATE_AskPlums;
	}

	switch(gc_feState)
	{
		case feSTATE_Welcome:
			si_feTicker = 0;
			platSetupFrontend(WelcomeLogoPos);
			sc_textLine = 0;
			platShowTextAligned(0, 0, cc_welcomeText[sc_textLine]);
			sc_textRow = WelcomeLogoPos + LogoHeight + 1;
			while(++sc_textLine < cc_numWelcomeTextLines)
			{
				platShowTextAligned(0, sc_textRow, cc_welcomeText[sc_textLine]);
				sc_textRow += 2;
			}
			gc_feState = feSTATE_HoldWelcome;
		break;
		
		case feSTATE_HoldWelcome:
			if(++si_feTicker > WelcomeHoldTime)
				gc_feState = feSTATE_SetupScroll;
		break;
		
		case feSTATE_SetupScroll:
			platSetupFrontend(ScrollLogoPos);
			sc_textRow = TextStart;
			sc_textLine = 0;
			sc_textRow = ScreenY - 2;
			sc_linesScrolled = 0;
			si_feTicker = 0;
			gc_feState = feSTATE_HoldPreScroll;
		break;

		case feSTATE_HoldPreScroll:
			platRollColours();
			if(++si_feTicker > ScrollHoldTime)
				gc_feState = feSTATE_Scrolling;
		break;

		case feSTATE_Scrolling:
			platScrollScreenUp();
			if(++sc_linesScrolled >= (TextSpacing*CharHeight) - CharHeight)
			{
				gc_feState = feSTATE_AddText;
				sc_linesScrolled = 0;
			}
		break;
		
		case feSTATE_AddText:
			if(sc_textLine < cc_numScrollTextLines)
			{
				if(sc_linesScrolled < CharHeight)
				{
					platShowTextSliver(sc_textRow, sc_linesScrolled, cc_scrollText[sc_textLine]);
				}
				else
				{
					++sc_textLine;
					sc_linesScrolled = 0;
					gc_feState = feSTATE_Scrolling;
				}
			}
			else
			{
				if(sc_linesScrolled > (ScreenY - 3) * CharHeight)
				{
					gc_feState = feSTATE_DemoLoopSetup;
					sc_linesScrolled = -1;
					sc_textLine = 0;
				}
			}
			platScrollScreenUp();
			++sc_linesScrolled;
		break;
		
		case feSTATE_AskPlums:
			if(key >= '0' && key < '3')
			{
				gc_numPlums = key - '0';
				gc_feState = feSTATE_Welcome;	
				return gSTATE_GameSetup;
			}
			
			if(++si_feTicker > PlumTimeout)
				gc_feState = feSTATE_Welcome;
		break;
		
		case feSTATE_DemoLoopSetup:
			sc_demoLocal = 0;
			gc_numPlums = 1;
			gc_feState = feSTATE_DemoLoop;
		break;
		
		case feSTATE_DemoLoop:
			frontendDemoStep();
		break;
	}
	return gSTATE_Frontend;
}

void frontendShowBonus()
{
	sprintf(sc_textBuffer, cc_bonusText, gi_levelBonus);
	platShowTextAligned(0, ScreenY-1, sc_textBuffer);
}

void frontendShowScore()
{
	sprintf(sc_textBuffer, "%02d", gi_score);
	platShowTextAligned(7, ScreenY-2, sc_textBuffer);
}

void frontendShowHiScore()
{
	sprintf(sc_textBuffer, "%02d", gi_highScore);
	platShowTextAligned(4, ScreenY-1, sc_textBuffer);
}

void frontendShowNumSnakes()
{
	sprintf(sc_textBuffer, "%d", gc_lives);
	platShowTextAligned(24, ScreenY-2, sc_textBuffer);
}

void frontendShowNumApples()
{
	sprintf(sc_textBuffer, "%-2d", gc_applesToGetThisLevel - gc_applesGottenThisLevel);
	platShowTextAligned(37, ScreenY-2, sc_textBuffer);
}

void frontendShowLevel()
{
	sprintf(sc_textBuffer, "%2d", gc_level+1);
	platShowTextAligned(38, ScreenY-1, sc_textBuffer);
}

void frontendShowHud()
{
	platShowTextAligned(0, ScreenY-2, cc_scoreText);
	platShowTextAligned(0, ScreenY-1, cc_hiScoreText);
	frontendShowScore();
	frontendShowHiScore();
	frontendShowNumSnakes();
	frontendShowNumApples();
	frontendShowLevel();
}

void frontendDemoStep()
{
	if(!sc_demoLocal)
	{
		gameSetup();
		gc_level = cc_numLevels - 1;
		sc_demoLocal = 1;
	}
	
	if(1 == sc_demoLocal)
	{
		levelSetup();
		sc_demoLocal = 2;
	}

	if(2 == sc_demoLocal)
	{
		char AIResult = gamePlay(NO);
		if(gSTATE_Play != AIResult)
			sc_demoLocal = 3;
	}
	
	if(3 == sc_demoLocal)
	{
		if(crash(NO))
		{
			gc_applesToGetThisLevel = ApplesPerLevel;
			if(++gc_level >= cc_numLevels)
				gc_level = 0;

			sc_demoLocal = 1;
		}
	}
}
