#include "DFPCMS.h"
#include "Serial.h"

//UART_HandleTypeDef *huart_dfpcms;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


uint8_t DFPCMS_sequence[6] = {START_BYTE,0,0,0,0,END_BYTE};

uint8_t numberOfSongs = 0;

uint8_t deviceVolume = 0;
#define VOLUP_LIMIT 30
#define VOLDOWN_LIMIT 0 

uint8_t deviceEQ = 0;

uint8_t deviceSong = 0;

void dfpcms_clearBuf( )
{
  for( uint8_t i = 0; i < 4; i++) DFPCMS_sequence[1+i] = 0;
}


void dfpcms_init(UART_HandleTypeDef *huartdf, UART_HandleTypeDef *huartserial)
{
  //huart_dfpcms = huartdf;
  serialPrint( huartserial, "Init %s module\n", "DPFCMS");
}
void dfpcms_readInfo( uint8_t *buf , uint8_t size )
{
  if ( buf && buf[0] == START_BYTE && buf[SEQ_SIZE_CMD - 1] == END_BYTE && ( buf[1] >= INIT_CMD && buf[1] <= START_BYTE ) )
  {
    switch (buf[1])
    {
    case GETNUMSONGS_CMD:
      numberOfSongs = buf[2];
      serialPrint( &huart2 , "Number of Songs:%2d\n", numberOfSongs);
      break;
    case INIT_CMD:
    case PLAY_CMD:
    case PAUSE_CMD:
    case PREV_CMD:
    case NEXT_CMD:
    case RESUME_CMD:
      uint8_t Verf = buf[2];
      if ( Verf ) {serialPrint( &huart2 , "CMS %x respond successfully\n", buf[1]);}
      else {serialPrint( &huart2 , "CMS %x respond error\n", buf[1]);}
    case SETVOL_CMD:
      deviceVolume = buf[3];
      serialPrint( &huart2 , "Device Volume:%2d\n", deviceVolume);
    case SETEQ_CMD:
      deviceEQ = buf[2];
      serialPrint( &huart2 , "Device EQ:%2d\n", deviceEQ);
    case SETSONG_CMD:
      deviceSong = buf[2];
      serialPrint( &huart2 , "Device Song:%2d\n", deviceSong);
      break;
    case GETVOL_CMD:
    default:
      break;
    }
  }
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