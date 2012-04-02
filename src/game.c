/*
 *  game.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include "globals.h"
#include "data.h"
#include "plat.h"
#include "grid.h"
#include "game.h"
#include "frontend.h"

#define NumLives			3
#define AppleEatGrowLength	10
#define	SnakeInitalLength	8
#define	SnakeStartX			GridX/2
#define	SnakeStartY			GridY-2
#define GameOverHoldTime	40
#define LevelExitHoldTime	10

char gameRunSnakeAI(char x, char y, char currDir);

static char sc_gameLocalState;
static char sc_extraApplesThisLevel;
static int si_levelScore;

const char cc_bodyFromNewDirAndOldDir[4][4] = 
{	// old - Up					// Old - Right				// Old - Down				// Old - Left
	{gCHAR_SnakeBodyUp, 		gCHAR_SnakeBodyRightUp, 	gCHAR_SnakeBodyDown, 		gCHAR_SnakeBodyLeftUp},		// New - Up
	{gCHAR_SnakeBodyUpRight, 	gCHAR_SnakeBodyRight, 		gCHAR_SnakeBodyDownRight, 	gCHAR_SnakeBodyLeft},		// New - Right
	{gCHAR_SnakeBodyUp, 		gCHAR_SnakeBodyRightDown, 	gCHAR_SnakeBodyDown, 		gCHAR_SnakeBodyLeftDown},	// New - Down
	{gCHAR_SnakeBodyUpLeft, 	gCHAR_SnakeBodyRight, 		gCHAR_SnakeBodyDownLeft,	gCHAR_SnakeBodyLeft},		// New - Left
};

void gameInitialSetup()
{
	gi_highScore = 0;
	gc_applesGottenThisLevel = 0;
	sc_gameLocalState = 0;
}

void gameSetup()
{
	gi_score = 0;
	gc_level = 0;
	gc_applesToGetThisLevel = ApplesPerLevel;
	si_levelScore = 10 + (gc_numPlums * 5);
	gi_levelBonus = 20 + (gc_numPlums * 10);
	gc_lives = NumLives - 1;
	platSetupGame();
}

void levelSetup()
{
	gt_snake.head = 0;
	gt_snake.length = 0;
	gt_snake.direction = gCHAR_SnakeHeadUp;
	gt_snake.grow = SnakeInitalLength;
	gt_snake.x[gt_snake.head] = SnakeStartX;
	gt_snake.y[gt_snake.head] = SnakeStartY;
	gt_snake.state[gt_snake.head] = gt_snake.direction;

	gc_applesGottenThisLevel = 0;
	sc_extraApplesThisLevel = 0;
	gc_nextAppleTicks = 0;
	
	platSetupLevel();
	frontendShowHud();
	gridSetup(YES, ct_levels[gc_level], cc_levelLines[gc_level]);
	gridAddApple();
	gridAddPlums();
	gridDrawSnake();
}

char gamePlay(bool userMode)
{
	char x, y, dir, newDir, collide, bForce = NO, retVal = gSTATE_Play, head = gt_snake.head;

	gridDrawPlum();

	x = gt_snake.x[head];
	y = gt_snake.y[head];
	dir = gt_snake.state[head];
	
	if(userMode)
		gt_snake.direction = newDir = platReadGameKeys(dir);
	else
		gt_snake.direction = newDir = gameRunSnakeAI(x, y, dir);

	if(gb_entranceOpen)
	{
		if(!gt_snake.length)
			gt_snake.direction = newDir = gCHAR_SnakeHeadUp;
			
		if(!gt_snake.grow)
			gridCloseWall();
	}
	
	gt_snake.state[head] = cc_bodyFromNewDirAndOldDir[newDir][dir];

	if(++head >= MaxSegments)
		head = 0;

	gt_snake.head = head;
	gt_snake.state[head] = newDir;

	switch(newDir)
	{
		case gCHAR_SnakeHeadUp:
			gt_snake.x[head] = x;
			gt_snake.y[head] = y - 1;
		break;

		case gCHAR_SnakeHeadRight:
			gt_snake.x[head] = x + 1;
			gt_snake.y[head] = y ;
		break;

		case gCHAR_SnakeHeadDown:
			gt_snake.x[head] = x;
			gt_snake.y[head] = y + 1;
		break;

		case gCHAR_SnakeHeadLeft:
			gt_snake.x[head] = x - 1;
			gt_snake.y[head] = y;
		break;
	}

	if(gt_snake.grow)
	{
		++gt_snake.length;
		--gt_snake.grow;
	}

	collide = gridDrawSnake();
	
	if(gCHAR_Apple == collide)
	{
		platAdjustSyncSpeed();
		
		if(++gc_applesGottenThisLevel == gc_applesToGetThisLevel)
			gridOpenWall(0);
		else if(!sc_extraApplesThisLevel)
			gridAddApple();
		else
			--sc_extraApplesThisLevel;
			
		if(gt_snake.length < MaxSegments - AppleEatGrowLength)
			gt_snake.grow = AppleEatGrowLength;

		gc_nextAppleTicks = 0;
		frontendShowNumApples();
		gi_score += si_levelScore ;
		frontendShowScore();
		bForce = YES;
	}
	else if(gc_applesGottenThisLevel != gc_applesToGetThisLevel && ++gc_nextAppleTicks >= TicksToApple)
	{
		gc_nextAppleTicks = 0;
		if(gc_applesToGetThisLevel < 100)
		{
			gc_applesToGetThisLevel += 3;
			sc_extraApplesThisLevel += 3;
			gridAddApple();
			gridAddApple();
			gridAddApple();
		}
		frontendShowNumApples();
	}
	else if(gCHAR_Blank == collide)
		retVal = gSTATE_Play;
	else if(gCHAR_Exit == collide)
	{
		frontendShowNumSnakes();
		retVal = gSTATE_ExitLevel;
	}
	else
	{
		retVal = gSTATE_Crash;
	}
	
	platShowTicksToNextApple(bForce);
	return retVal;
}

bool crash(bool userMode)
{
	if(0 == sc_gameLocalState)
	{
		gt_snake.grow = 0;
		gi_levelBonus = 0;
		platSyncFastSpeed();
		sc_gameLocalState = 1;
	}
	
	if(1 == sc_gameLocalState)
	{
		if(1 != gt_snake.length)
		{
			gridDrawSnake();
			--gt_snake.length;
		}
		else	
			sc_gameLocalState = 2;
	}
	
	if(2 == sc_gameLocalState)
	{
		platSyncGameSpeed();
		if(userMode)
			platShowTextAligned(0, ScreenY-1, cc_crashText);
		sc_gameLocalState = 3;
	}
	
	if(3 == sc_gameLocalState && (!userMode || platReadSpaceKey()))
	{
		sc_gameLocalState = 0;
		return YES;
	}
	
	platShowTicksToNextApple(YES);

	return NO;
}

bool exitLevel()
{
	if(!sc_gameLocalState)
	{
		platSyncFastSpeed();

		if(gi_levelBonus)
		{
			gi_score += gi_levelBonus;
			frontendShowBonus();
			frontendShowScore();
		}
		++gc_lives;
		++gc_level;
		si_levelScore = (gc_level+1) * (10 + (gc_numPlums * 5));
		gi_levelBonus = (gc_level+1) * (20 + (gc_numPlums * 10));
		if(gc_level >= cc_numLevels)
			gc_level = 0;

		sc_gameLocalState = 1;
	}

	if(1 == sc_gameLocalState)
	{
		if(1 != gt_snake.length)
		{
			gridDrawSnake();
			--gt_snake.length;
		}
		else
		{
			sc_gameLocalState = 2;
			platSyncGameSpeed();
		}
	}
	else if(LevelExitHoldTime == ++sc_gameLocalState)
	{
		gc_applesToGetThisLevel = ApplesPerLevel;
		sc_gameLocalState = 0;
		platFlushKeys();
		return YES;
	}

	return NO;
}

bool gameOver()
{
	if(!sc_gameLocalState)
	{
		platClearScreen();
		gridSetup(NO, ct_rip, cc_numRIPLines);
		platShowTextAligned(0, ScreenY-1, cc_gameOvertext);
		gc_applesGottenThisLevel = 0;
		if(gi_highScore < gi_score)
			gi_highScore = gi_score ;
	}
	
	if(platReadFEKeys() || GameOverHoldTime == ++sc_gameLocalState)
	{
		sc_gameLocalState = 0;
		return YES;
	}

	return NO;
}

char gameRunSnakeAI(char x, char y, char currDir)
{
	char gridBlock, dir = currDir;
	static signed char sc_turnAgain = -1;

	if(-1 != sc_turnAgain)
	{
		dir = sc_turnAgain;
		sc_turnAgain = -1;
	}
	
	if(x != gc_appleX)
	{
		if(1 != (currDir & 1))
		{
			if(x < gc_appleX)
			{
				gridBlock = gc_grid[gi_gridStart[y]+x+1];
				if(gCHAR_Blank == gridBlock || gCHAR_Apple == gridBlock)
					dir = gCHAR_SnakeHeadRight;
			}
			else
			{
				gridBlock = gc_grid[gi_gridStart[y]+x-1];
				if(gCHAR_Blank == gridBlock || gCHAR_Apple == gridBlock)
					dir = gCHAR_SnakeHeadLeft;
			}
		}
	}
	else if(y != gc_appleY && 1 == (currDir & 1))
	{
		if(y < gc_appleY)
		{
			gridBlock = gc_grid[gi_gridStart[y+1]+x];
			if(gCHAR_Blank == gridBlock || gCHAR_Apple == gridBlock)
				dir = gCHAR_SnakeHeadDown;
		}
		else
		{
			gridBlock = gc_grid[gi_gridStart[y-1]+x];
			if(gCHAR_Blank == gridBlock || gCHAR_Apple == gridBlock)
				dir = gCHAR_SnakeHeadUp;
		}
	}
	
	if(dir == currDir)
	{
		signed char x1, y1;
		char loopCntr = 0;
		while(++loopCntr < 4)
		{
			switch(dir)
			{
				case gCHAR_SnakeHeadUp:
				x1 = 0;
				y1 = -1;
				break;
			
				case gCHAR_SnakeHeadRight:
				x1 = 1;
				y1 = 0;
				break;
			
				case gCHAR_SnakeHeadDown:
				x1 = 0;
				y1 = 1;
				break;
			
				case gCHAR_SnakeHeadLeft:
				x1 = -1;
				y1 = 0;
				break;
			}
			
			gridBlock = gc_grid[gi_gridStart[y+y1]+x+x1];
			if(gCHAR_Blank == gridBlock || gCHAR_Apple == gridBlock || gCHAR_Exit == gridBlock)
				break;
			else if(gridBlock < gCHAR_Blank)
				sc_turnAgain = dir;
			else
				sc_turnAgain = -1;
				
			if(++dir > gCHAR_SnakeHeadLeft)
				dir = gCHAR_SnakeHeadUp;
		}
	}
	
	if(-1 != sc_turnAgain)
	{
		sc_turnAgain = dir + (dir - currDir);
		if(-1 == sc_turnAgain)
			sc_turnAgain = 3;
		else if(-3 == sc_turnAgain)
			sc_turnAgain = 1;
	}
	
	return dir;
}
