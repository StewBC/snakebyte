/*
 *  game.h
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#ifndef _GAME_H_
#define _GAME_H_

void gameInitialSetup(void);
void gameSetup(void);
void levelSetup(void);
char gamePlay(bool userMode);
bool crash(bool userMode);
bool exitLevel(void);
bool gameOver(void);

#endif _GAME_H_
