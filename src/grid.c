/*
 *  grid.c
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "plat.h"
#include "grid.h"

#define	MaxPlums	2

void gridDrawLine(char x0, char y0, char x1, char y1, char fillChar);

static char sc_plumX[MaxPlums];
static char sc_plumY[MaxPlums];
static signed char sc_plumDX[MaxPlums];
static signed char sc_plumDY[MaxPlums];

const char cc_snakeTailDirection[] = 
{
	gCHAR_SnakeTailUp,			/* gCHAR_SnakeHeadUp */
	gCHAR_SnakeTailRight,		/* gCHAR_SnakeHeadRight */
	gCHAR_SnakeTailDown,		/* gCHAR_SnakeHeadDown */
	gCHAR_SnakeTailLeft,		/* gCHAR_SnakeHeadLeft */

	gCHAR_SnakeTailUp,			/* gCHAR_SnakeBodyUp */
	gCHAR_SnakeTailRight,		/* gCHAR_SnakeBodyRight */
	gCHAR_SnakeTailDown,		/* gCHAR_SnakeBodyDown */
	gCHAR_SnakeTailLeft,		/* gCHAR_SnakeBodyLeft */

	gCHAR_SnakeTailRight,		/* gCHAR_SnakeBodyUpRight */
	gCHAR_SnakeTailDown,		/* gCHAR_SnakeBodyRightDown */
	gCHAR_SnakeTailLeft,		/* gCHAR_SnakeBodyDownLeft */
	gCHAR_SnakeTailUp,			/* gCHAR_SnakeBodyLeftUp */

	gCHAR_SnakeTailLeft,		/* gCHAR_SnakeBodyUpLeft */
	gCHAR_SnakeTailDown,		/* gCHAR_SnakeBodyLeftDown */
	gCHAR_SnakeTailRight,		/* gCHAR_SnakeBodyDownRight */
	gCHAR_SnakeTailUp,			/* gCHAR_SnakeBodyRightUp */
};

void gridInitialSetup()
{
	int x, y;
	
	x = 0;
	for(y = 0; y < GridY; ++y)
	{
		gi_gridStart[y] = x;
		x += GridX;
	}
}

void gridSetup(const bool showBorder, const t_LevelLine *pLevelLines, const char numLines)
{
	char lines;
	
	memset(gc_grid, gCHAR_Blank, GridX*GridY);
	
	if(showBorder)
	{
		gridDrawLine(0, 0, GridX-1, 0, gCHAR_Wall1);
		gridDrawLine(0, GridY-2, GridX-1, GridY-2, gCHAR_Wall1);
		gridDrawLine(0, 1, 0, GridY-3, gCHAR_Wall10);
		gridDrawLine(GridX-1, 1, GridX-1, GridY-3, gCHAR_Wall10);
		platPrepAppleTicksBar();
		gridOpenWall(GridY-2);
		gb_entranceOpen = YES;
	}
	
	for(lines = 0; lines < numLines; ++lines)
	{
		const t_LevelLine *pLine = &(pLevelLines[lines]);
		gridDrawLine(pLine->x0, pLine->y0, pLine->x1, pLine->y1, pLine->style);
	}
}

void gridDrawLine(char x0, char y0, char x1, char y1, char fillChar)
{
	signed char dx, dy, sx, sy, err, e2;

	dx = abs(x1-x0);
	dy = abs(y1-y0);

	if(x0 < x1)
		sx = 1; 
	else 
		sx = -1;

	if(y0 < y1)
		sy = 1;
	else
		sy = -1;

	err = dx - dy;

	while(1)
	{
		gc_grid[gi_gridStart[y0]+x0] = fillChar;
		platPlot(x0, y0, fillChar);
		if(x0 == x1 && y0 == y1)
			break;
		e2 = err << 1;
		if(e2 > -dy) 
		{
			err = err - dy;
			x0 = x0 + sx;
		}
		if(e2 <  dx)
		{
			err = err + dx;
			y0 = y0 + sy;
		}
	}
}

char gridDrawSnake()
{
	char gx, gy, gv, oldHead, tail, collide, head = gt_snake.head ;
	
	if(gt_snake.length)
	{
		oldHead = head - 1;
		if(oldHead >= MaxSegments)
			oldHead = MaxSegments - 1;
		gx = gt_snake.x[oldHead];
		gy = gt_snake.y[oldHead];
		gv = gt_snake.state[oldHead];
		gc_grid[gi_gridStart[gy]+gx] = gv;
		platPlot(gx, gy, gv);
		
		if(!gt_snake.grow)
		{
			if(gt_snake.length > head)
				tail = MaxSegments - gt_snake.length + head;
			else
				tail = head - gt_snake.length;
			gx = gt_snake.x[tail];
			gy = gt_snake.y[tail];
			gc_grid[gi_gridStart[gy]+gx] = gCHAR_Blank;
			platPlot(gx, gy, gCHAR_Blank);
			++tail;
			if(tail >= MaxSegments)
				tail = 0;
			gx = gt_snake.x[tail];
			gy = gt_snake.y[tail];
			gv = cc_snakeTailDirection[gt_snake.state[tail]];
			gc_grid[gi_gridStart[gy]+gx] = gv;
			platPlot(gx, gy, gv);
		}
	}
	gx = gt_snake.x[head];
	gy = gt_snake.y[head];
	gv = gt_snake.direction;
	collide = gc_grid[gi_gridStart[gy]+gx];
	gc_grid[gi_gridStart[gy]+gx] = gv;
	platPlot(gx, gy, gv);
	
	return collide;
}

void gridDrawPlum()
{
	char i, x, y, dx, dy, nx, ny, block;
	
	for(i = 0; i < gc_numPlums; ++i)
	{
		x = sc_plumX[i];
		y = sc_plumY[i];
		dx = sc_plumDX[i];
		dy = sc_plumDY[i];
		nx = x + dx;
		ny = y + dy;
		block = gc_grid[gi_gridStart[ny]+nx] ;
		if(gCHAR_Blank != block)
		{
			if(gCHAR_Blank != gc_grid[gi_gridStart[ny]+x])
			{
				if(gCHAR_Blank != gc_grid[gi_gridStart[y]+nx])
				{
					nx = x ;
					ny = y ;
					sc_plumDX[i] = -dx ;
					sc_plumDY[i] = -dy ;
				}
				else
				{
					ny = y ;
					sc_plumDY[i] = -dy ;
				}
			}
			else
			{
				nx = x ;
				sc_plumDX[i] = -dx ;
			}
		}

		if(gCHAR_Plum == gc_grid[gi_gridStart[y]+x])
		{
			gc_grid[gi_gridStart[y]+x] = gCHAR_Blank;
			platPlot(x,y,gCHAR_Blank);
		}
		sc_plumX[i] = nx ;
		sc_plumY[i] = ny ;
		gc_grid[gi_gridStart[ny]+nx] = gCHAR_Plum;
		platPlot(nx,ny,gCHAR_Plum);
	}
}

void gridAddApple()
{
	do
	{
		gc_appleX = (rand() % (GridX-3))+1;
		gc_appleY = (rand() % (GridY-3))+1;
	} while(gCHAR_Blank != gc_grid[gi_gridStart[gc_appleY]+gc_appleX]);
	gc_grid[gi_gridStart[gc_appleY]+gc_appleX] = gCHAR_Apple;
	platPlot(gc_appleX,gc_appleY,gCHAR_Apple);
}

void gridAddPlums()
{
	char i, x, y;
	
	for(i = 0; i < gc_numPlums; ++i)
	{
		do
		{
			x = (rand() % (GridX-2))+1;
			y = (rand() % ((GridY/3)*2))+1;
		} while(gCHAR_Blank != gc_grid[gi_gridStart[y]+x]);
		gc_grid[gi_gridStart[y]+x] = gCHAR_Plum;
		sc_plumX[i] = x;
		sc_plumY[i] = y;
		sc_plumDX[i] = rand() % 2 ? 1 : -1;
		sc_plumDY[i] = rand() % 2 ? 1 : -1;
		platPlot(x,y,gCHAR_Plum);
	}
}

void gridOpenWall(char row)
{
	char gx = GridX/2;
	
	gridDrawLine(gx-4, row, gx+3, row, gCHAR_ExitMarker);
	gc_grid[gx+(row*GridX)] = gCHAR_Exit;
	gc_appleX = gx;
	gc_appleY = row;
	platPlot(gx, row, gCHAR_Exit);
}

void gridCloseWall()
{
	char gx = GridX/2;
	
	gridDrawLine(gx-4, GridY-2, gx+3, GridY-2, gCHAR_Wall1);
	gb_entranceOpen = NO;
}
