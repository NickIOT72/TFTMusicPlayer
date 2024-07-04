#include "DFPCMS.h"
#include "Serial.h"

//UART_HandleTypeDef *huart_dfpcms;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

uint8_t numberOfSongs = 0;
uint8_t DFPCMS_sequence[6] = {START_BYTE,0,0,0,0,END_BYTE};

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
  if ( buf && buf[0] == START_BYTE && buf[SEQ_SIZE_CMD - 1] == END_BYTE && ( buf[1] >= INIT_CMD && buf[1] <= GETNUMSONGS_CMD ) )
  {
    switch (buf[1])
    {
    case GETNUMSONGS_CMD:
      numberOfSongs = buf[2];
      serialPrint( &huart2 , "Number of Songs:%2d\n", numberOfSongs);
      break;
    
    default:
      break;
    }
  }
}
void dfpcms_sendInfo( uint8_t *buf , uint8_t size )
{
  serialWrite( &huart3 , buf , size );
  dfpcms_clearBuf();
}
void dfpcms_getNumberOfSongs( )
{
  DFPCMS_sequence[1] = GETNUMSONGS_CMD;
  for( uint8_t i = 0; i < 3; i++) DFPCMS_sequence[2+i] = 0;
  dfpcms_sendInfo ( DFPCMS_sequence , SEQ_SIZE_CMD );
}