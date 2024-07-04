#ifndef __DFPCMS_H__
#define __DFPCMS_H__

#include "moduleInit.h"
#include "dfPlayerCommands.h"

void dfpcms_init(UART_HandleTypeDef *huartdf, UART_HandleTypeDef *huartserial);
void dfpcms_readInfo( uint8_t *buf , uint8_t size );
void dfpcms_sendInfo( uint8_t *buf , uint8_t size );
void dfpcms_getNumberOfSongs( );

#endif