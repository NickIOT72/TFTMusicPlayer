#ifndef __SCREENPLAYER_H__
#define __SCREENPLAYER_H__

#include "ScreenManager.h"

int screenPlayer_init( struct screenManager *sm  );
int screenPlayer_eval(struct screenManager *sm);
bool returnSongStatus();
void setSongFromMainMenu(uint8_t s);
void setStatusSongFromMainMenu(bool t);
void setBarInit();
#endif