#include "screenVerification.h"
#include "Rectangle.h"
#include "Text.h"
#include "Circle.h"
#include "ILI9486.h"
#include "TimeCounter.h"
#include "Serial.h"
#include "DFPCMS.h"
#include "Button.h"
#include "Joystick.h"
#include "main.h"

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

struct text pointtext;
char pointText[10];
struct timeCounter timeVerification;
struct button tryAgainButton; 
struct joystick js2;

int screenVerificationListOfSongsNull_show()
{
  int err = -1;

  struct Rectangle rect1;
  struct Rectangle rectBottom;

  struct text introtext;
  struct text introtext2;
  struct text introtext3;

  rect1.backgroundColor = TURKEY_LIGTH;
  rect1.borderColor = TURKEY_LIGTH;
  rect1.borderSize = 0;
  rect1.circleBorder = 0;
  rect1.heigth = 40;
  rect1.width = 440;
  rect1.xo = 20;
  rect1.yo = 20;

  rectBottom.backgroundColor = TURKEY_LIGTH;
  rectBottom.borderColor = TURKEY_LIGTH;
  rectBottom.borderSize = 0;
  rectBottom.circleBorder = 0;
  rectBottom.heigth = 40;
  rectBottom.width = 440;
  rectBottom.xo = 20;
  rectBottom.yo = 260;

  introtext.color = RED;
  introtext.f = mono12x7bold;
  introtext.size = 2;
  strcpy( introtext.text, "No songs on" );
  introtext.xo = 90;
  introtext.yo = 100;

  introtext2.color = RED;
  introtext2.f = mono12x7bold;
  introtext2.size = 2;
  strcpy( introtext2.text, "SD card. Check" );
  introtext2.xo = 40;
  introtext2.yo = 140;

  introtext3.color = RED;
  introtext3.f = mono12x7bold;
  introtext3.size = 2;
  strcpy( introtext3.text, "and try again" );
  introtext3.xo = 60;
  introtext3.yo = 180;

  tryAgainButton.r.backgroundColor = WHITE;
  tryAgainButton.r.borderColor = TURKEY_LIGTH;
  tryAgainButton.r.borderSize = 3;
  tryAgainButton.r.circleBorder = 5;
  tryAgainButton.r.xo = 180;
  tryAgainButton.r.yo = 200;
  tryAgainButton.r.width = 90;
  tryAgainButton.r.heigth = 50;
  tryAgainButton.t.color = TURKEY_LIGTH;
  tryAgainButton.t.f = mono12x7bold;
  tryAgainButton.t.size = 2;
  strcpy(  tryAgainButton.t.text , "R");
  tryAgainButton.t.xo = 210;
  tryAgainButton.t.yo = 240;
  tryAgainButton.isPressed = false;
  tryAgainButton.pressedColor = GREY_LIGTH;
  tryAgainButton.pressedTextColor = TURKEY_LIGTH;
  tryAgainButton.releasedColor = WHITE;
  tryAgainButton.releasedTextColor = TURKEY_LIGTH;
  tryAgainButton.tm.tStart = 50;

  fillScreen(WHITE);
  rectBottom.heigth = 40;
  rectBottom.width = 440;
  rectangle_draw(&rectBottom);
  rect1.heigth = 40;
  rect1.width = 440;
  rectangle_draw(&rect1);
  text_draw(&introtext);
  text_draw(&introtext2);
  text_draw(&introtext3);
  button_draw(&tryAgainButton);

  return err;
}

int screenVerificationListOfSongsFailed_show()
{
  int err = -1;

  struct Rectangle rect1;
  struct Rectangle rectBottom;

  struct text introtext;
  struct text introtext2;
  struct text introtext3;

  rect1.backgroundColor = TURKEY_LIGTH;
  rect1.borderColor = TURKEY_LIGTH;
  rect1.borderSize = 0;
  rect1.circleBorder = 0;
  rect1.heigth = 40;
  rect1.width = 440;
  rect1.xo = 20;
  rect1.yo = 20;

  rectBottom.backgroundColor = TURKEY_LIGTH;
  rectBottom.borderColor = TURKEY_LIGTH;
  rectBottom.borderSize = 0;
  rectBottom.circleBorder = 0;
  rectBottom.heigth = 40;
  rectBottom.width = 440;
  rectBottom.xo = 20;
  rectBottom.yo = 260;

  introtext.color = RED;
  introtext.f = mono12x7bold;
  introtext.size = 2;
  strcpy( introtext.text, "Couldn't connect" );
  introtext.xo = 15;
  introtext.yo = 100;

  introtext2.color = RED;
  introtext2.f = mono12x7bold;
  introtext2.size = 2;
  strcpy( introtext2.text, "with device!" );
  introtext2.xo = 90;
  introtext2.yo = 140;

  introtext3.color = RED;
  introtext3.f = mono12x7bold;
  introtext3.size = 2;
  strcpy( introtext3.text, "Try again" );
  introtext3.xo = 115;
  introtext3.yo = 180;

  tryAgainButton.r.backgroundColor = WHITE;
  tryAgainButton.r.borderColor = TURKEY_LIGTH;
  tryAgainButton.r.borderSize = 3;
  tryAgainButton.r.circleBorder = 5;
  tryAgainButton.r.xo = 180;
  tryAgainButton.r.yo = 200;
  tryAgainButton.r.width = 90;
  tryAgainButton.r.heigth = 50;
  tryAgainButton.t.color = TURKEY_LIGTH;
  tryAgainButton.t.f = mono12x7bold;
  tryAgainButton.t.size = 2;
  strcpy(  tryAgainButton.t.text , "R");
  tryAgainButton.t.xo = 210;
  tryAgainButton.t.yo = 240;
  tryAgainButton.isPressed = false;
  tryAgainButton.pressedColor = GREY_LIGTH;
  tryAgainButton.pressedTextColor = TURKEY_LIGTH;
  tryAgainButton.releasedColor = WHITE;
  tryAgainButton.releasedTextColor = TURKEY_LIGTH;
  tryAgainButton.tm.tStart = 50;

  fillScreen(WHITE);
  rectBottom.heigth = 40;
  rectBottom.width = 440;
  rectangle_draw(&rectBottom);
  rect1.heigth = 40;
  rect1.width = 440;
  rectangle_draw(&rect1);
  text_draw(&introtext);
  text_draw(&introtext2);
  text_draw(&introtext3);
  button_draw(&tryAgainButton);

  return err;
}

int screenVerificationListOfSongs_show()
{
  int err = -1;

  struct Rectangle rect1;
  struct Rectangle rect2;

  struct text introtext;
  struct text introtext2;

  rect1.backgroundColor = TURKEY_LIGTH;
  rect1.borderColor = TURKEY_LIGTH;
  rect1.borderSize = 0;
  rect1.circleBorder = 0;
  rect1.heigth = 40;
  rect1.width = 440;
  rect1.xo = 20;
  rect1.yo = 20;

  rect2.backgroundColor = TURKEY_LIGTH;
  rect2.borderColor = TURKEY_LIGTH;
  rect2.borderSize = 0;
  rect2.circleBorder = 0;
  rect2.heigth = 40;
  rect2.width = 440;
  rect2.xo = 20;
  rect2.yo = 260;

  introtext.color = TURKEY_LIGTH;
  introtext.f = mono12x7bold;
  introtext.size = 2;
  strcpy( introtext.text, "Ready. Getting" );
  introtext.xo = 30;
  introtext.yo = 100;

  introtext2.color = TURKEY_LIGTH;
  introtext2.f = mono12x7bold;
  introtext2.size = 2;
  strcpy( introtext2.text, "list of songs" );
  introtext2.xo = 30;
  introtext2.yo = 160;

  pointtext.color = TURKEY_LIGTH;
  pointtext.f = mono12x7bold;
  pointtext.size = 2;
  strcpy( pointtext.text, "." );
  pointtext.xo = 100;
  pointtext.yo = 200;

  fillScreen(WHITE);
  rect2.heigth = 40;
  rect2.width = 440;
  rectangle_draw(&rect2);
  rect1.heigth = 40;
  rect1.width = 440;
  rectangle_draw(&rect1);
  text_draw(&introtext);
  text_draw(&introtext2);
  text_draw(&pointtext);

  return err;
}

int screenVerificationFailed_show()
{
  int err = -1;

  struct Rectangle rect1;
  struct Rectangle rectBottom;

  struct text introtext;
  struct text introtext2;
  struct text introtext3;

  rect1.backgroundColor = TURKEY_LIGTH;
  rect1.borderColor = TURKEY_LIGTH;
  rect1.borderSize = 0;
  rect1.circleBorder = 0;
  rect1.heigth = 40;
  rect1.width = 440;
  rect1.xo = 20;
  rect1.yo = 20;

  rectBottom.backgroundColor = TURKEY_LIGTH;
  rectBottom.borderColor = TURKEY_LIGTH;
  rectBottom.borderSize = 0;
  rectBottom.circleBorder = 0;
  rectBottom.heigth = 40;
  rectBottom.width = 440;
  rectBottom.xo = 20;
  rectBottom.yo = 260;

  introtext.color = RED;
  introtext.f = mono12x7bold;
  introtext.size = 2;
  strcpy( introtext.text, "No device" );
  introtext.xo = 115;
  introtext.yo = 100;

  introtext2.color = RED;
  introtext2.f = mono12x7bold;
  introtext2.size = 2;
  strcpy( introtext2.text, "detected!" );
  introtext2.xo = 120;
  introtext2.yo = 140;

  introtext3.color = RED;
  introtext3.f = mono12x7bold;
  introtext3.size = 2;
  strcpy( introtext3.text, "Try again" );
  introtext3.xo = 115;
  introtext3.yo = 180;

  tryAgainButton.r.backgroundColor = WHITE;
  tryAgainButton.r.borderColor = TURKEY_LIGTH;
  tryAgainButton.r.borderSize = 3;
  tryAgainButton.r.circleBorder = 5;
  tryAgainButton.r.xo = 180;
  tryAgainButton.r.yo = 200;
  tryAgainButton.r.width = 90;
  tryAgainButton.r.heigth = 50;
  tryAgainButton.t.color = TURKEY_LIGTH;
  tryAgainButton.t.f = mono12x7bold;
  tryAgainButton.t.size = 2;
  strcpy(  tryAgainButton.t.text , "R");
  tryAgainButton.t.xo = 210;
  tryAgainButton.t.yo = 240;
  tryAgainButton.isPressed = false;
  tryAgainButton.pressedColor = GREY_LIGTH;
  tryAgainButton.pressedTextColor = TURKEY_LIGTH;
  tryAgainButton.releasedColor = WHITE;
  tryAgainButton.releasedTextColor = TURKEY_LIGTH;
  tryAgainButton.tm.tStart = 50;

  fillScreen(WHITE);
  rectBottom.heigth = 40;
  rectBottom.width = 440;
  rectangle_draw(&rectBottom);
  rectangle_draw(&rect1);
  text_draw(&introtext);
  text_draw(&introtext2);
  text_draw(&introtext3);
  button_draw(&tryAgainButton);

  return err;
}

int screenVerification_show()
{
  int err = -1;

  struct Rectangle rect1;
  struct Rectangle rect2;

  struct text introtext;
  struct text introtext2;

  rect1.backgroundColor = TURKEY_LIGTH;
  rect1.borderColor = TURKEY_LIGTH;
  rect1.borderSize = 0;
  rect1.circleBorder = 0;
  rect1.heigth = 40;
  rect1.width = 440;
  rect1.xo = 20;
  rect1.yo = 20;

  rect2.backgroundColor = TURKEY_LIGTH;
  rect2.borderColor = TURKEY_LIGTH;
  rect2.borderSize = 0;
  rect2.circleBorder = 0;
  rect2.heigth = 40;
  rect2.width = 440;
  rect2.xo = 20;
  rect2.yo = 260;

  introtext.color = TURKEY_LIGTH;
  introtext.f = mono12x7bold;
  introtext.size = 2;
  strcpy( introtext.text, "Verifiying" );
  introtext.xo = 90;
  introtext.yo = 100;

  introtext2.color = TURKEY_LIGTH;
  introtext2.f = mono12x7bold;
  introtext2.size = 2;
  strcpy( introtext2.text, "connection" );
  introtext2.xo = 90;
  introtext2.yo = 160;

  pointtext.color = TURKEY_LIGTH;
  pointtext.f = mono12x7bold;
  pointtext.size = 2;
  strcpy( pointtext.text, "." );
  pointtext.xo = 100;
  pointtext.yo = 200;

  fillScreen(WHITE);

  rectangle_draw(&rect1);
  rectangle_draw(&rect2);
  text_draw(&introtext);
  text_draw(&introtext2);
  text_draw(&pointtext);

  return err;
}

void doNothing(  struct joystick *js )
{
  // Do nothing
}

int screenVerification_eval(struct screenManager *sm)
{
  static bool deviceAsked = false;
  static uint8_t counterTimeVerification = 1;
  static bool deviceDetected = false;
  timeCounter_verifyTimer(&timeVerification);
  if ( timeVerification.timerReached ){
    if (  counterTimeVerification % 10 == 0){
      fillRect( 90 , 170 , 320 , 40 , WHITE );
      strcpy( pointtext.text, "." );
      text_draw(&pointtext);
    }
    else{
      if ( counterTimeVerification >= 6 )
      {
        if ( !deviceAsked && !deviceDetected ){
          dfpcms_initiation();
          deviceAsked = true;
          strcat( pointtext.text, "." );
          text_draw(&pointtext);
        }
        else if( deviceAsked && !deviceDetected  ) {
          if ( dfpcms_getInit() )
          {
            serialPrint( &huart2 , "%s" ,"Device initiated\r\n"  );
            deviceDetected = true;
            screenVerificationListOfSongs_show();
            counterTimeVerification = 1;
            dfpcms_getNumberOfSongs();
          }
          else{
            if ( counterTimeVerification % 3 == 0)  dfpcms_initiation();
            strcat( pointtext.text, "." );
            text_draw(&pointtext);
          }
        }
        else if (  deviceAsked && deviceDetected )
        {
          if (  dfpcms_getLocalNumberOfSongs() == 0  )
          {
            screenVerificationListOfSongsNull_show();
            while (1)
            {
              joystick_getValuesByTimer(&js2 , doNothing );
              if( !js2.sw ){
                tryAgainButton.r.backgroundColor = tryAgainButton.pressedColor;
                button_draw(&tryAgainButton);
                while ( !js2.sw){
                  joystick_getValuesByTimer(&js2 , doNothing );
                  HAL_Delay(1);
                }
                tryAgainButton.r.backgroundColor = tryAgainButton.releasedColor;
                button_draw(&tryAgainButton);
                screenVerificationListOfSongs_show();
                counterTimeVerification = 1;
                break;
              }
              HAL_Delay(1);
            }
          }
          else if (dfpcms_getLocalNumberOfSongs() > 0  )
          {
            serialPrint( &huart2 , "NUmber of Songs: %2d\r\n" ,dfpcms_getLocalNumberOfSongs()  );
            fillRect( 90 , 170 , 320 , 40 , WHITE );
            pointtext.xo = pointtext.xo  - 60;
            pointtext.yo = pointtext.yo + 30;
            pointtext.size = dfpcms_getLocalNumberOfSongs() > 9?1:2;
            sprintf( pointtext.text, "%d songs on list", dfpcms_getLocalNumberOfSongs());
            text_draw(&pointtext);
            HAL_Delay(3000);
            sm->actualScreen = 4;
          }
          else{
            if ( counterTimeVerification % 3 == 0)  dfpcms_getNumberOfSongs();
            strcat( pointtext.text, "." );
            text_draw(&pointtext);
          }
        }
        
      }
      else
      {
        strcat( pointtext.text, "." );
        text_draw(&pointtext);
      }
    }
    counterTimeVerification += 1;
    if ( counterTimeVerification >= 100 )
    {
      serialPrint( &huart2 , "%s" ,"Iniation error\r\n"  );
      if (!deviceDetected) screenVerificationFailed_show();
      else screenVerificationListOfSongsFailed_show();
      while (1)
      {

        joystick_getValuesByTimer(&js2 , doNothing );
        if( !js2.sw ){
          tryAgainButton.r.backgroundColor = tryAgainButton.pressedColor;
          button_draw(&tryAgainButton);
          while ( !js2.sw){
            joystick_getValuesByTimer(&js2 , doNothing );
            HAL_Delay(1);
          }
          tryAgainButton.r.backgroundColor = tryAgainButton.releasedColor;
          button_draw(&tryAgainButton);
          screenVerification_show();
          deviceAsked = false;
          deviceDetected = false;
          dfpcms_resetInit();
          dfpcms_resetNumberOfSongs();
          counterTimeVerification = 1;
          break;
        }
        HAL_Delay(1);
      }
      
    }
    timeCounter_resetTimer( &timeVerification );
  }
  return -1;
}

int screenVerification_init( struct screenManager *sm  )
{
  int err = -1;
  sm->s.tc.delay = 0;
  err = screenVerification_show();
  timeVerification.delay = 100;
  timeCounter_init( &timeVerification );
  timeCounter_initTimer( &timeVerification );
  js2.sw_port = JS_SW_GPIO_Port;
  js2.swPin = JS_SW_Pin;
  js2.hadc_js = &hadc1;
  js2.adcChannels[0] = 12;
  js2.adcChannels[1] = 13;

  js2.tm.delay = 50;

  joystick_init(&js2);
  return err;
}