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

void platInitialSetup(void);
void platSetupGame(void);
void platSetupFrontend(char row);
void platSetupLevel(void);
void platBeginFrame(void);
void platSyncEndFrame(void);
void platSyncFastSpeed(void);
void platSyncGameSpeed(void);
void platAdjustSyncSpeed(void);
void platClearScreen(void);
void platPlot(char gx, char gy, char gv);
void platShowTextAligned(char textPosX, char textPosY, const char *szText);
void platShowTextSliver(char textPosY, char sliver, const char *szText);
void platPrepAppleTicksBar(void);
void platShowTicksToNextApple(char force);
void platScrollScreenUp(void);
void platRollColours(void);
void platShutDown(void);
char platReadGameKeys(char currDir);
char platReadFEKeys(void);
char platReadSpaceKey(void);
void platFlushKeys(void);

#endif _PLAT_H_
