#include "DFPCMS.h"
#include "Serial.h"

//UART_HandleTypeDef *huart_dfpcms;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


uint8_t DFPCMS_sequence[6] = {START_BYTE,0,0,0,0,END_BYTE};

int numberOfSongs = -1;
uint8_t deviceVolume = 0;
#define VOLUP_LIMIT 30
#define VOLDOWN_LIMIT 0 
uint8_t deviceEQ = 0;
uint8_t deviceSong = 0;
bool deviceInitializeze = false;

bool waitingForUartResponse = false;

void dfpcms_clearBuf( )
{
  for( uint8_t i = 0; i < 4; i++) DFPCMS_sequence[1+i] = 0;
}


void dfpcms_init()
{
  //huart_dfpcms = huartdf;
  serialPrint( &huart2, "Init %s module\r\n", "DPFCMS");
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
      serialPrint( &huart2 , "Number of Songs:%2d\r\n", numberOfSongs);
      break;
    case INIT_CMD:
    case PLAY_CMD:
    case PAUSE_CMD:
    case PREV_CMD:
    case NEXT_CMD:
    case RESUME_CMD:
      uint8_t Verf = buf[2];
      if ( Verf ) {
        serialPrint( &huart2 , "CMS %x respond successfully\r\n", buf[1]);
        if ( buf[1] == INIT_CMD) deviceInitializeze = true;
      }
      else {serialPrint( &huart2 , "CMS %x respond error\r\n", buf[1]);}
      break;
    case SETVOL_CMD:
      deviceVolume = buf[3];
      serialPrint( &huart2 , "Device Volume:%2d\r\n", deviceVolume);
      break;
    case SETEQ_CMD:
      deviceEQ = buf[2];
      serialPrint( &huart2 , "Device EQ:%2d\r\n", deviceEQ);
      break;
    case SETSONG_CMD:
      deviceSong = buf[2];
      serialPrint( &huart2 , "Device Song:%2d\r\n", deviceSong);
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
  dfpcms_sendCms( INIT_CMD );
}
void dfpcms_play()
{
  dfpcms_sendCms( PLAY_CMD );
}
void dfpcms_pause()
{
  dfpcms_sendCms( PAUSE_CMD );
}
void dfpcms_resume()
{
  dfpcms_sendCms( RESUME_CMD );
}
void dfpcms_previous()
{
  dfpcms_sendCms( PREV_CMD );
}
void dfpcms_next()
{
  dfpcms_sendCms( NEXT_CMD );
}
void dfpcms_setVolume( bool volume )
{
  uint8_t vol = volume ? VOLUP_CMD : VOLDOWN_CMD;
  dfpcms_sendCms( vol );
}
void dfpcms_setVolumeVal( uint8_t volume )
{
  DFPCMS_sequence[1] = VOLSET_CMD;
  for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
  DFPCMS_sequence[2] = volume;
  dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
}
int dfpcms_getVolume( )
{
  return deviceVolume;
}
void dfpcms_setSong( uint8_t song )
{
  DFPCMS_sequence[1] = SETSONG_CMD;
  for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
  DFPCMS_sequence[2] = song;
  dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
}
int dfpcms_getCurrentSong( )
{
  return deviceSong;
}
void dfpcms_getNumberOfSongs( )
{
  DFPCMS_sequence[1] = GETNUMSONGS_CMD;
  for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
  dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
}
int dfpcms_getLocalNumberOfSongs( )
{
  return numberOfSongs;
}
void dfpcms_setEQ( uint8_t eq )
{
  DFPCMS_sequence[1] = SETEQ_CMD;
  for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
  DFPCMS_sequence[2] = eq;
  dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
}
int dfpcms_getEQ()
{
  return deviceEQ;
}	
void dfpcms_sendInfo( uint8_t *buf , uint8_t size )
{
  serialWrite( &huart3 , buf , size );
  dfpcms_clearBuf();
}
void dfpcms_sendCms( uint8_t data )
{
  DFPCMS_sequence[1] = data;
  for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
  dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
}


void dfpcms_waitingInitication( )
{
  while ( !dfpcms_getInit() )
  {
    waitingForUartResponse = true;
    dfpcms_initiation();
    uint8_t countUartWait = 0;
    while (waitingForUartResponse && countUartWait < 50)
    {
      /* code */
      HAL_Delay(50);
      countUartWait += 1;
    }
    
  }
  waitingForUartResponse = false;
}

void dfpcms_waitingSetupSong( uint8_t song  )
{
  while ( dfpcms_getCurrentSong() != song )
  {
    waitingForUartResponse = true;
    dfpcms_setSong(song);
    uint8_t countUartWait = 0;
    while (waitingForUartResponse && countUartWait < 50)
    {
      /* code */
      HAL_Delay(50);
      countUartWait += 1;
    }
  }
  waitingForUartResponse = false;
}

void dfpcms_waitingVolume( uint8_t volume )
{
  while ( dfpcms_getVolume()!= volume )
  {
    waitingForUartResponse = true;
    dfpcms_setVolumeVal( volume );
    uint8_t countUartWait = 0;
    while (waitingForUartResponse && countUartWait < 50)
    {
      /* code */
      HAL_Delay(50);
      countUartWait += 1;
    }
  }
  waitingForUartResponse = false;
}