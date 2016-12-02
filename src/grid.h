/*
 *  grid.h
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#ifndef _GRID_H_
#define _GRID_H_

void gridInitialSetup(void);
void gridSetup(const bool showBorder, const t_LevelLine *pLevelLines, const char numLines);
char gridDrawSnake(void);
void gridDrawPlum(void);
void gridAddApple(void);
void gridAddPlums(void);
void gridOpenWall(char row);
void gridCloseWall(void);

#endif _GRID_H_
