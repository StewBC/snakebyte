/*
 *  plat.h
 *  SnakeByte
 *
 *  Created by Stefan Wessels, May 2011.
 *
 */

#ifndef _PLAT_H_
#define _PLAT_H_

#define ScreenX				40
#define	ScreenY				25
#define	CharHeight			8

void platInitialSetup();
void platSetupGame();
void platSetupFrontend(char row);
void platSetupLevel();
void platBeginFrame();
void platSyncEndFrame();
void platSyncFastSpeed();
void platSyncGameSpeed();
void platAdjustSyncSpeed();
void platClearScreen();
void platPlot(char gx, char gy, char gv);
void platShowTextAligned(char textPosX, char textPosY, const char *szText);
void platShowTextSliver(char textPosY, char sliver, const char *szText);
void platPrepAppleTicksBar();
void platShowTicksToNextApple(char force);
void platScrollScreenUp();
void platRollColours();
void platShutDown();
char platReadGameKeys(char currDir);
char platReadFEKeys();
char platReadSpaceKey();
void platFlushKeys();

#endif _PLAT_H_
