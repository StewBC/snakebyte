/*
 *  grid.h
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#ifndef _GRID_H_
#define _GRID_H_

void gridInitialSetup();
void gridSetup(const bool showBorder, const t_LevelLine *pLevelLines, const char numLines);
char gridDrawSnake();
void gridDrawPlum();
void gridAddApple();
void gridAddPlums();
void gridOpenWall(char row);
void gridCloseWall();

#endif _GRID_H_
