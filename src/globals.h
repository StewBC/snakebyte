/*
 *  globals.h
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define	bool					char
#define	YES						1
#define	NO						0

#define	GridX					40
#define	GridY					37

#define	MaxSegments				255
#define TicksToApple			128
#define ApplesPerLevel			10

enum 
{
	gSTATE_InitialSetup,
	gSTATE_Frontend,
	gSTATE_GameSetup,
	gSTATE_LevelSetup,
	gSTATE_Play,
	gSTATE_Crash,
	gSTATE_CrashAnim,
	gSTATE_ExitLevel,
	gSTATE_GameOver,
	gSTATE_AllDone,
};

enum
{
	feSTATE_Welcome,
	feSTATE_HoldWelcome,
	feSTATE_SetupScroll,
	feSTATE_HoldPreScroll,
	feSTATE_Scrolling,
	feSTATE_AddText,
	feSTATE_AskPlums,
	feSTATE_DemoLoopSetup,
	feSTATE_DemoLoop,
};

enum
{
	gCHAR_SnakeHeadUp,
	gCHAR_SnakeHeadRight,	
	gCHAR_SnakeHeadDown,	
	gCHAR_SnakeHeadLeft,

	gCHAR_SnakeBodyUp,
	gCHAR_SnakeBodyRight,
	gCHAR_SnakeBodyDown,
	gCHAR_SnakeBodyLeft,

	gCHAR_SnakeBodyUpRight,
	gCHAR_SnakeBodyRightDown,
	gCHAR_SnakeBodyDownLeft,
	gCHAR_SnakeBodyLeftUp,

	gCHAR_SnakeBodyUpLeft,
	gCHAR_SnakeBodyLeftDown,
	gCHAR_SnakeBodyDownRight,
	gCHAR_SnakeBodyRightUp,

	gCHAR_SnakeTailUp,
	gCHAR_SnakeTailRight,	
	gCHAR_SnakeTailDown,	
	gCHAR_SnakeTailLeft,
	
	gCHAR_Blank,
	gCHAR_Apple,
	gCHAR_Plum,
	gCHAR_Exit,
	gCHAR_ExitMarker,
	gCHAR_Wall1,
	gCHAR_Wall2,
	gCHAR_Wall3,
	gCHAR_Wall4,
	gCHAR_Wall5,
	gCHAR_Wall6,
	gCHAR_Wall7,
	gCHAR_Wall8,
	gCHAR_Wall9,
	gCHAR_Wall10,
};

typedef struct tagSnake
{
	char x[MaxSegments];
	char y[MaxSegments];
	char state[MaxSegments];
	char direction;
	char head;
	char length;
	char grow;
} t_Snake;

typedef struct tagLevelLine
{
	char x0, y0, x1, y1, style;
} t_LevelLine;

extern t_Snake gt_snake;
extern bool gb_entranceOpen;
extern char gc_grid[GridY*GridX];
extern char gc_gameState;
extern char gc_feState;
extern char gc_level;
extern char gc_lives;
extern char gc_nextAppleTicks;
extern char gc_applesToGetThisLevel;
extern char gc_applesGottenThisLevel;
extern char gc_numPlums;
extern char gc_appleX;
extern char gc_appleY;
extern int gi_highScore;
extern int gi_score;
extern int gi_levelBonus;
extern int gi_gridStart[GridY];

#endif _GLOBALS_H_
