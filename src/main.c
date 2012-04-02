/*
 *  main.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include "globals.h"
#include "plat.h"
#include "grid.h"
#include "game.h"
#include "frontend.h"

void mainLoop();
void setup();
void shutDown();

int main()
{
	setup();
	mainLoop();
	shutDown();
	
	return 0;
}

void mainLoop()
{
	bool bAllDone = NO;

	gc_gameState = gSTATE_Frontend;
	
	while(!bAllDone)
	{
		platBeginFrame();
		switch(gc_gameState)
		{
			case gSTATE_Frontend:
				gc_gameState = frontend();
			break;

			case gSTATE_GameSetup:
				gameSetup();
				gc_gameState = gSTATE_LevelSetup;
			// break;

			case gSTATE_LevelSetup:
				levelSetup();
				gc_gameState = gSTATE_Play;
			break;
		
			case gSTATE_Play:
				gc_gameState = gamePlay(YES);
			break;
		
			case gSTATE_Crash:
				gc_gameState = gSTATE_CrashAnim;
			// break;
			
			case gSTATE_CrashAnim:
				if(crash(YES))
				{
					if(gc_lives)
					{
						--gc_lives;
						gc_gameState = gSTATE_LevelSetup;
					}
					else
						gc_gameState = gSTATE_GameOver;
				}
			break;
		
			case gSTATE_ExitLevel:
				if(exitLevel())
					gc_gameState = gSTATE_LevelSetup;
			break;

			case gSTATE_GameOver:
				if(gameOver())
					gc_gameState = gSTATE_Frontend;
			break;

			case gSTATE_AllDone:
				bAllDone = YES;
			break;
		}              
		platSyncEndFrame();
	}
}

void setup()
{
	platInitialSetup();
	gridInitialSetup();
	gameInitialSetup();
	frontendInitialSetup();
	platFlushKeys();
}

void shutDown()
{
	platShutDown();
}
