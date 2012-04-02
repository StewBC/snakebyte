/*
 *  globals.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include "globals.h"

t_Snake gt_snake;
bool gb_entranceOpen;
char gc_grid[GridY*GridX];
char gc_gameState;
char gc_feState;
char gc_level;
char gc_lives;
char gc_nextAppleTicks;
char gc_applesToGetThisLevel;
char gc_applesGottenThisLevel;
char gc_numPlums;
char gc_appleX;
char gc_appleY;
int gi_highScore;
int gi_score;
int gi_levelBonus;
int gi_gridStart[GridY];
