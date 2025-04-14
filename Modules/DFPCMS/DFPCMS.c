#include "DFPCMS.h"
#include "dfminiplayer.h"
#include "Serial.h"

//UART_HandleTypeDef *huart_dfpcms;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

#if defined(SERIAL_DFPLAYERMINI)
    struct DFPlayerMiniData dfpl;
#endif

uint8_t DFPCMS_sequence[6] = {START_BYTE,0,0,0,0,END_BYTE};

int numberOfSongs = -1;
uint8_t deviceVolume = 0;
#define VOLUP_LIMIT 30
#define VOLDOWN_LIMIT 0 
uint8_t deviceEQ = 0;
int deviceSong = -1;
bool deviceInitializeze = false;
bool waitingForUartResponse = false;
uint8_t prevSong = 0;
uint8_t statusDevice = true;

void dfpcms_clearBuf( )
{
  for( uint8_t i = 0; i < 4; i++) DFPCMS_sequence[1+i] = 0;
}


void dfpcms_init()
{
  //huart_dfpcms = huartdf;
  Serial_print( &huart2, "Init %s module\r\n", "DPFCMS");
  dfpcms_initiation();
}
void dfpcms_resetInit()
{
  deviceInitializeze = false;
}

void dfpcms_resetNumberOfSongs()
{
  numberOfSongs = -1;
}
void dfpcms_readInfo( uint8_t *buf , uint8_t size )
{
  if ( buf && buf[0] == START_BYTE && buf[SEQ_SIZE_CMD - 1] == END_BYTE && ( buf[1] >= INIT_CMD && buf[1] <= START_BYTE ) )
  {
    switch (buf[1])
    {
    case GETNUMSONGS_CMD:
      numberOfSongs = buf[2];
      Serial_print( &huart2 , "Number of Songs:%2d\r\n", numberOfSongs);
      break;
    case INIT_CMD:
    case PLAY_CMD:
    case PAUSE_CMD:
    case PREV_CMD:
    case NEXT_CMD:
    case RESUME_CMD:
      uint8_t Verf = buf[2];
      if ( Verf > 0 ) {
        Serial_print( &huart2 , "CMS %x respond successfully\r\n", buf[1]);
        if ( buf[1] == INIT_CMD) deviceInitializeze = true;
      }
      else {Serial_print( &huart2 , "CMS %x respond error\r\n", buf[1]);}
      break;
    case SETVOL_CMD:
      deviceVolume = buf[3];
      Serial_print( &huart2 , "Device Volume:%2d\r\n", deviceVolume);
      break;
    case SETEQ_CMD:
      deviceEQ = buf[2];
      Serial_print( &huart2 , "Device EQ:%2d\r\n", deviceEQ);
      break;
    case SETSONG_CMD:
      deviceSong = buf[2];
      Serial_print( &huart2 , "Device Song:%2d\r\n", deviceSong);
      break;
    case STATUS_CMD:
      statusDevice = (uint8_t)buf[2];
      Serial_print( &huart2 , "Device Status:%s\r\n", statusDevice==0||statusDevice==2?"OFF":"ON");
      break;
    case GETVOL_CMD:
      break;
    default:
      break;
    }
  }
  waitingForUartResponse = false;
}
bool dfpcms_getInit()
{
  return deviceInitializeze;
}	
void dfpcms_initiation()
{
  #if defined(SERIAL_ESP32)
      dfpcms_sendCms( INIT_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      dfpl._timeOutDuration = 500;
      dfpl._sending[0] = 0x7E; //{0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF};
      dfpl._sending[1] = 0xFF;
      dfpl._sending[2] = 06;
      dfpl._sending[3] = 00;
      dfpl._sending[4] = 01;
      dfpl._sending[5] = 00;
      dfpl._sending[6] = 00;
      dfpl._sending[7] = 00;
      dfpl._sending[8] = 00;
      dfpl._sending[9] = 0xEF;
      dfpl._receivedIndex = 0;
      dfpl.device = DFPLAYER_DEVICE_SD;
      dfpl._isAvailable = false;
      dfpl._isSending = false;
      DFPLayerMini_begin(&dfpl,true,true);
      dfpcms_pause();
      dfpcms_setVolumeVal( deviceVolume );
      deviceInitializeze = true;
  #endif
}
void dfpcms_play()
{
  #if defined(SERIAL_ESP32)
      dfpcms_sendCms( PLAY_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      DFPLayerMini_play(&dfpl,deviceSong);
  #endif
}

void dfpcms_stop()
{
  #if defined(SERIAL_ESP32)
      dfpcms_sendCms( PLAY_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
  DFPLayerMini_stop(&dfpl);
  #endif
}
void dfpcms_pause()
{
  #if defined(SERIAL_ESP32)
      dfpcms_sendCms( PAUSE_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      DFPLayerMini_pause(&dfpl);
  #endif
}
void dfpcms_resume()
{
  #if defined(SERIAL_ESP32)
      dfpcms_sendCms( RESUME_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      DFPLayerMini_start(&dfpl);
  #endif
}
void dfpcms_previous()
{
  #if defined(SERIAL_ESP32)
      dfpcms_sendCms( PREV_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      DFPLayerMini_previous(&dfpl);
  #endif
}
void dfpcms_next()
{
  #if defined(SERIAL_ESP32)
      dfpcms_sendCms( NEXT_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      DFPLayerMini_next(&dfpl);
  #endif
}
void dfpcms_setVolume( bool volume )
{
  #if defined(SERIAL_ESP32)
      uint8_t vol = volume ? VOLUP_CMD : VOLDOWN_CMD;
      dfpcms_sendCms( vol );
  #elif defined(SERIAL_DFPLAYERMINI)
      volume ? DFPLayerMini_volumeUp(&dfpl): DFPLayerMini_volumeDown(&dfpl);
  #endif
}
void dfpcms_setVolumeVal( uint8_t volume )
{
  #if defined(SERIAL_ESP32)
      DFPCMS_sequence[1] = VOLSET_CMD;
      for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
      DFPCMS_sequence[2] = volume;
      dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      DFPLayerMini_volume(&dfpl, volume);
      deviceVolume = volume;
  #endif
}
int dfpcms_getVolume( )
{
  return deviceVolume;
}
void dfpcms_setSong( uint8_t song )
{
  #if defined(SERIAL_ESP32)
      DFPCMS_sequence[1] = SETSONG_CMD;
      for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
      DFPCMS_sequence[2] = song;
      dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      DFPLayerMini_play(&dfpl, song);
      deviceSong = song;
  #endif
}
int dfpcms_getCurrentSong( )
{
  return deviceSong;
}
void dfpcms_getNumberOfSongs( )
{
  #if defined(SERIAL_ESP32)
      DFPCMS_sequence[1] = GETNUMSONGS_CMD;
      for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
      dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      numberOfSongs = DFPLayerMini_readFileCounts(&dfpl);
  #endif
}
int dfpcms_getLocalNumberOfSongs( )
{
  return numberOfSongs;
}
void dfpcms_setEQ( uint8_t eq )
{
  #if defined(SERIAL_ESP32)
      DFPCMS_sequence[1] = SETEQ_CMD;
      for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
      DFPCMS_sequence[2] = eq;
      dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
  #elif defined(SERIAL_DFPLAYERMINI)
      deviceEQ = (uint8_t)DFPLayerMini_readEQ(&dfpl);
  #endif
}
int dfpcms_getEQ()
{
  return deviceEQ;
}	
void dfpcms_sendInfo( uint8_t *buf , uint8_t size )
{
  Serial_write( &huart3 , buf , size );
  dfpcms_clearBuf();
}
void dfpcms_sendCms( uint8_t data )
{
  DFPCMS_sequence[1] = data;
  for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
  dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
}

void dfpcms_setPrevSong(uint8_t song)
{
  prevSong = song;
}

int dfpcms_getPrevSong()
{
  return prevSong;
}

void DFPCMS_getStatus()
{
  #if defined(SERIAL_ESP32)
    dfpcms_sendCms( STATUS_CMD  );
  #elif defined(SERIAL_DFPLAYERMINI)
    statusDevice = (uint8_t)DFPLayerMini_readState(&dfpl);
  #endif
}
bool dfpcms_getStatusLocal()
{
  return !(statusDevice==0||statusDevice==2);
}


void dfpcms_waitingInitication( )
{
  while ( !dfpcms_getInit() )
  {
    waitingForUartResponse = true;
    dfpcms_initiation();
    #if defined(SERIAL_ESP32)
      uint8_t countUartWait = 0;
      while (waitingForUartResponse && countUartWait < 50)
      {
        /* code */
        HAL_Delay(50);
        countUartWait += 1;
      }
    #endif
    
  }
  waitingForUartResponse = false;
}

void dfpcms_waitingSetupSong( uint8_t song  )
{
  while ( dfpcms_getCurrentSong() != song )
  {
    waitingForUartResponse = true;
    dfpcms_setSong(song);
    #if defined(SERIAL_ESP32)
      uint8_t countUartWait = 0;
      while (waitingForUartResponse && countUartWait < 50)
      {
        /* code */
        HAL_Delay(50);
        countUartWait += 1;
      }
    #endif
  }
  waitingForUartResponse = false;
}

void dfpcms_waitingVolume( uint8_t volume )
{
  while ( dfpcms_getVolume()!= volume )
  {
    waitingForUartResponse = true;
    dfpcms_setVolumeVal( volume );
    #if defined(SERIAL_ESP32)
    uint8_t countUartWait = 0;
    while (waitingForUartResponse && countUartWait < 50)
    {
      /* code */
      HAL_Delay(50);
      countUartWait += 1;
    }
  #endif
  }
  waitingForUartResponse = false;
}

void dfpcms_waitingPlayPause( bool stat )
{
  while ( dfpcms_getStatusLocal() != stat )
  {
    waitingForUartResponse = true;
    stat? dfpcms_pause() : dfpcms_play();
    #if defined(SERIAL_ESP32)
    uint8_t countUartWait = 0;
    while (waitingForUartResponse && countUartWait < 50)
    {
      /* code */
      HAL_Delay(50);
      countUartWait += 1;
    }
  #endif
    DFPCMS_getStatus();
    HAL_Delay(50);
  }
  waitingForUartResponse = false;
}

void dfpcms_waitingResume()
{
  while ( dfpcms_getStatusLocal() != false)
  {
    waitingForUartResponse = true;
    dfpcms_resume();
    #if defined(SERIAL_ESP32)
    uint8_t countUartWait = 0;
    while (waitingForUartResponse && countUartWait < 50)
    {
      /* code */
      HAL_Delay(50);
      countUartWait += 1;
    }
  #endif
    DFPCMS_getStatus();
    HAL_Delay(50);
  }
  waitingForUartResponse = false;
}