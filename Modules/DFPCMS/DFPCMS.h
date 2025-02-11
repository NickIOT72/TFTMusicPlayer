#ifndef __DFPCMS_H__
#define __DFPCMS_H__

#include "moduleInit.h"
#include "dfPlayerCommands.h"

#define DFPLAYER_EQ_NORMAL 0
#define DFPLAYER_EQ_POP 1
#define DFPLAYER_EQ_ROCK 2
#define DFPLAYER_EQ_JAZZ 3
#define DFPLAYER_EQ_CLASSIC 4
#define DFPLAYER_EQ_BASS 5

#define STATUS_PLAY false
#define STATUS_PAUSE true

void dfpcms_init();
void dfpcms_readInfo( uint8_t *buf , uint8_t size );
void dfpcms_sendInfo( uint8_t *buf , uint8_t size );
void dfpcms_getNumberOfSongs( );
void dfpcms_sendCms( uint8_t data );
void dfpcms_initiation();
bool dfpcms_getInit();
void dfpcms_play();
void dfpcms_pause();
void dfpcms_resume();
void dfpcms_previous();
void dfpcms_next();
void dfpcms_setVolume( bool volume );
int dfpcms_getVolume( );
void dfpcms_setVolumeVal( uint8_t volume );
void dfpcms_setSong( uint8_t song );
int dfpcms_getCurrentSong( );
void dfpcms_setEQ( uint8_t eq );
int dfpcms_getEQ();
int dfpcms_getLocalNumberOfSongs( );
void dfpcms_resetInit();
void dfpcms_resetNumberOfSongs();
void dfpcms_setPrevSong(uint8_t song);
int dfpcms_getPrevSong();
void DFPCMS_getStatus();
void dfpcms_waitingInitication( );
void dfpcms_waitingSetupSong( uint8_t song );
void dfpcms_waitingVolume( uint8_t volume );
bool dfpcms_getStatusLocal();
void dfpcms_waitingPlayPause( bool stat );
void dfpcms_waitingResume();

#endif