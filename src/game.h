/*
 *  game.h
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#ifndef _GAME_H_
#define _GAME_H_

void gameInitialSetup();
void gameSetup();
void levelSetup();
char gamePlay(bool userMode);
bool crash(bool userMode);
bool exitLevel();
bool gameOver();

#endif _GAME_H_
