#include "screenPlayer.h"
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
#include "Triangle.h"  

struct triangle selectorPlayer;
extern struct joystick js2;

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

struct circle prevBtn;
struct circle playBtn;
struct circle nextBtn;

struct circle prevBtn2;
struct circle playBtn2;
struct circle nextBtn2;

struct triangle prevBtnt1;
struct triangle prevBtnt2;

struct triangle playBtnt;

struct Rectangle playBtnP1;
struct Rectangle playBtnP2;

struct triangle nextBtnt1;
struct triangle nextBtnt2;

struct text textTrack;

struct triangle volExt;
struct triangle volInt;
struct triangle volSet;
struct text textVol;

struct button ReturnBtn;

#define STEP_TRI 5 

#define VOL_EXT_PX0 100
#define VOL_EXT_PY0 300
#define VOL_EXT_X_DIST 80
#define VOL_EXT_Y_DIST 50
#define VOLT_EXT_HIP 111.8
#define VOLT_EXT_CIRC_R 16.54

float volumeEquivalent = 5;

#define VOLT_INT_INTERNAL_DISTANCE 5 
#define VOLT_INT_R 11.54
#define VOLT_INT_X0 80
#define VOLT_INT_Y0 295
#define VOLT_INT_X1 25
#define VOLT_INT_Y1 295
#define VOLT_INT_X2 25
#define VOLT_INT_Y2 260

#define VOLT_INT_X_DIST 55 

uint8_t currentSong = 0;
bool statusSong = false;

uint8_t barSelection = 1;
bool selectedBar = false;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) *(float)((out_max - out_min) / (in_max - in_min)) + out_min;
}


void drawNextButton()
{
  circle_draw(&nextBtn2);
  triangle_draw(&nextBtnt1);
  triangle_draw(&nextBtnt2);
}

void drawPlayButton()
{
  circle_draw(&playBtn2);
  if ( statusSong ){
    triangle_draw(&playBtnt);
  }
  else 
  {
    rectangle_draw(&playBtnP1);
    rectangle_draw(&playBtnP2);
  }
}

void drawPrevButton()
{
  circle_draw(&prevBtn2);
  triangle_draw(&prevBtnt1);
  triangle_draw(&prevBtnt2);
}

void drawVol()
{
  triangle_draw(&volInt);
  triangle_draw(&volSet);
  fillRoundRect(60 , 240 , 60, 25, 0 , WHITE);
  text_draw(&textVol);
}


void setVolumneGraph()
{
  volExt.backgroundColor = TURKEY_LIGTH;
  volExt.borderColor = 0;
  volExt.borderSize = 0;
  volExt.x0 = VOL_EXT_PX0;
  volExt.y0 = VOL_EXT_PY0;
  volExt.x1 = volExt.x0 - VOL_EXT_X_DIST;
  volExt.y1 = volExt.y0;
  volExt.x2 = volExt.x1;
  volExt.y2 = volExt.y0 - VOL_EXT_Y_DIST;

  volInt.backgroundColor = WHITE;
  volInt.borderColor = 0;
  volInt.borderSize = 0;
  volInt.x1 = VOLT_INT_X1;
  volInt.y1 = VOLT_INT_Y1;
  volInt.x0 = VOLT_INT_X0;
  volInt.y0 = VOLT_INT_Y0;
  volInt.x2 = VOLT_INT_X2;
  volInt.y2 = VOLT_INT_Y2;

  float volt_int_circ_R = volumeEquivalent; //VOLT_EXT_CIRC_R - VOLT_INT_INTERNAL_DISTANCE - volumeEquivalent;

  float volt_int_X_DIST = (float)( volt_int_circ_R*VOL_EXT_X_DIST) /VOLT_EXT_CIRC_R;
  float volt_int_Y_DIST = (float)(volt_int_circ_R* VOL_EXT_Y_DIST )/VOLT_EXT_CIRC_R;
  float volt_int_H_DIST = (float)(volt_int_circ_R* VOLT_EXT_HIP )/VOLT_EXT_CIRC_R;

  volSet.backgroundColor = GREY_DARK;
  volSet.borderColor = 0;
  volSet.borderSize = 0;
  volSet.x0 = volInt.x0;
  volSet.y0 = volInt.y0;
  volSet.x1 = (int)(volSet.x0 - volt_int_X_DIST);
  volSet.y1 = volSet.y0;
  volSet.x2 = volSet.x1;
  volSet.y2 = (int)(volSet.y1 - volt_int_Y_DIST);/**/

  textVol.color = TURKEY_LIGTH;
  textVol.f = mono12x7bold;
  textVol.size = 1;
  int volPorc = (int)map(volumeEquivalent, 0, VOLT_EXT_CIRC_R - VOLT_INT_INTERNAL_DISTANCE, 0, 100  );
  sprintf( textVol.text, "%d%%", volPorc);
  textVol.xo = 60;
  textVol.yo = 260;

}

void doNot()
{

}

void selectOptionBar(uint8_t bft)
{
  if (bft == 3)
  {
    prevBtn2.backgroundColor = GREY_LIGTH;
    prevBtnt1.backgroundColor = WHITE;
    prevBtnt2.backgroundColor = WHITE;
    drawPrevButton();
  }
  else if (bft == 2)
  {
    playBtn2.backgroundColor = GREY_LIGTH;
    playBtnP1.backgroundColor = WHITE;
    playBtnP2.backgroundColor = WHITE;
    playBtnt.backgroundColor = WHITE;
    drawPlayButton();
  }
  else if (bft == 1)
  {
    nextBtnt1.backgroundColor = WHITE;
    nextBtnt2.backgroundColor = WHITE;
    nextBtn2.backgroundColor = GREY_LIGTH;
    drawNextButton();
  }
  else if (bft == 4)
  {
    volSet.backgroundColor = GREY_LIGTH;
    drawVol();
  }
  else if (bft == 0)
  {
    ReturnBtn.r.backgroundColor = ReturnBtn.pressedColor;
    ReturnBtn.t.color = ReturnBtn.pressedTextColor;
    button_draw(&ReturnBtn);
  }
}

void deselectOptionBar(uint8_t bft)
{
  if (bft == 3)
  {
    prevBtn2.backgroundColor = WHITE;
    prevBtnt1.backgroundColor = TURKEY_LIGTH;
    prevBtnt2.backgroundColor = TURKEY_LIGTH;
    drawPrevButton();
  }
  else if (bft == 2)
  {
    playBtn2.backgroundColor = WHITE;
    playBtnP1.backgroundColor = TURKEY_LIGTH;
    playBtnP2.backgroundColor = TURKEY_LIGTH;
    playBtnt.backgroundColor = TURKEY_LIGTH;
    drawPlayButton();
  }
  else if (bft == 1)
  {
    nextBtn2.backgroundColor = WHITE;
    nextBtnt1.backgroundColor = TURKEY_LIGTH;
    nextBtnt2.backgroundColor = TURKEY_LIGTH;
    drawNextButton();
  }
  else if (bft == 4)
  {
    volSet.backgroundColor = GREY_DARK;
    drawVol();
  }
  else if (bft == 0)
  {
    ReturnBtn.r.backgroundColor = ReturnBtn.releasedColor;
    ReturnBtn.t.color = ReturnBtn.releasedTextColor;
    button_draw(&ReturnBtn);
  }
}

int checkPostionJoystickPlayer( struct joystick *js )
{
  if ( js->adc_X >= 500 && js->adc_X <= 3500 && !(  js->adc_y < 500  || js->adc_y > 3500  ) )
  {
    if (!js->sw) return -1;
    if ( js->tm.delay == 250 ) js->tm.delay = 50;
    return 1;
  }
  else if ((  barSelection == 4   )&& js->adc_y >= 500 && js->adc_y <= 3500 && !(  js->adc_X < 500  || js->adc_X > 3500  ) )
  {
    if (!js->sw) return -1;
    if ( js->tm.delay == 250 ) js->tm.delay = 50;
    return 1;
  }
  return -1;
}

void checkOptionsPlayer(struct joystick *js)
{
  int err = checkPostionJoystickPlayer(js);
  if (err == 1) return;
  if( js->adc_X < 500 || js->adc_X > 3500 )
  {
    uint8_t prevBarSele = barSelection;
    barSelection = barSelection < 4 &&  js->adc_X < 500 ? barSelection +1: barSelection > 0 &&  js->adc_X > 3500? barSelection - 1:barSelection;
    if( prevBarSele != barSelection )
    {
      deselectOptionBar(prevBarSele);
      selectOptionBar(barSelection);
    }
    if (  js->tm.delay == 50 ) js->tm.delay = 250;
  }
  else if ( !js->sw && barSelection < 4  )
  {
    if ( barSelection == 1 ) serialPrint(&huart2 , "%s", "Go to next song");
    else if ( barSelection == 2 ) serialPrint(&huart2 , "%s", "Go to Play");
    else if ( barSelection == 3 ) serialPrint(&huart2 , "%s", "Go to prev song");
    else if ( barSelection == 0 ) serialPrint(&huart2 , "%s", "Return to menu");
    while ( !js2.sw)
    {
      joystick_getValuesByTimer(&js2 , doNot );
      HAL_Delay(1);
    }
  }
  else if ( (js->adc_y < 500 || js->adc_y > 3500 ) && barSelection == 4 )
  {
    if ( js->adc_y < 500  ) serialPrint(&huart2 , "%s", "Decrease Vol");
    else if ( js->adc_y > 3500 ) serialPrint(&huart2 , "%s", "Encrease Vol");
  }
}

void setPlayButton()
{
  playBtn2.backgroundColor = WHITE;
  playBtn2.borderColor = 0;
  playBtn2.borderSize = 0;
  playBtn2.radius = 25;
  playBtn2.xo = 330- 50;
  playBtn2.yo = 280;
  
  playBtnt.backgroundColor = TURKEY_LIGTH;
  playBtnt.borderColor = 0;
  playBtnt.borderSize = 0;
  playBtnt.x0 = playBtn.xo+2 + 15;
  playBtnt.y0 = playBtn.yo;
  playBtnt.x1 = playBtn.xo+2 - 15;
  playBtnt.y1 = playBtn.yo - 15;
  playBtnt.x2 = playBtn.xo+2 - 15;
  playBtnt.y2 = playBtn.yo + 15;

  playBtnP1.backgroundColor = TURKEY_LIGTH;
  playBtnP1.borderColor = 0;
  playBtnP1.borderSize = 0;
  playBtnP1.circleBorder = 0;
  playBtnP1.heigth = 30;
  playBtnP1.width = 10;
  playBtnP1.xo = playBtnt.x1;
  playBtnP1.yo = playBtnt.y1;

  playBtnP2.backgroundColor = TURKEY_LIGTH;
  playBtnP2.borderColor = 0;
  playBtnP2.borderSize = 0;
  playBtnP2.circleBorder = 0;
  playBtnP2.heigth = 30;
  playBtnP2.width = 10;
  playBtnP2.xo = playBtnt.x0 - playBtnP2.width -5;
  playBtnP2.yo = playBtnP1.yo;



}

void setPrevButton()
{
  prevBtn2.backgroundColor = WHITE;
  prevBtn2.borderColor = 0;
  prevBtn2.borderSize = 0;
  prevBtn2.radius = 25;
  prevBtn2.xo = 260- 50;
  prevBtn2.yo = 280;

  prevBtnt1.backgroundColor = TURKEY_LIGTH;
  prevBtnt1.borderColor = 0;
  prevBtnt1.borderSize = 0;
  prevBtnt1.x0 = prevBtn.xo-2 - 20;
  prevBtnt1.y0 = prevBtn.yo;
  prevBtnt1.x1 = prevBtn.xo-2;
  prevBtnt1.y1 = prevBtn.yo - 10;
  prevBtnt1.x2 = prevBtn.xo-2;
  prevBtnt1.y2 = prevBtn.yo + 10;

  prevBtnt2.backgroundColor = TURKEY_LIGTH;
  prevBtnt2.borderColor = 0;
  prevBtnt2.borderSize = 0;
  prevBtnt2.x0 = prevBtn.xo -2;
  prevBtnt2.y0 = prevBtn.yo;
  prevBtnt2.x1 = prevBtn.xo -2 + 20;
  prevBtnt2.y1 = prevBtn.yo - 10;
  prevBtnt2.x2 = prevBtn.xo -2  + 20;
  prevBtnt2.y2 = prevBtn.yo + 10;
}

void setNextButton()
{
  nextBtn2.backgroundColor = WHITE;
  nextBtn2.borderColor = 0;
  nextBtn2.borderSize = 0;
  nextBtn2.radius = 25;
  nextBtn2.xo = 400- 50;
  nextBtn2.yo = 280;

  nextBtnt1.backgroundColor = TURKEY_LIGTH;
  nextBtnt1.borderColor = 0;
  nextBtnt1.borderSize = 0;
  nextBtnt1.x0 = nextBtn.xo +2;
  nextBtnt1.y0 = nextBtn.yo;
  nextBtnt1.x1 = nextBtn.xo +2 - 20;
  nextBtnt1.y1 = nextBtn.yo - 10;
  nextBtnt1.x2 = nextBtn.xo +2 - 20 ;
  nextBtnt1.y2 = nextBtn.yo + 10;

  nextBtnt2.backgroundColor = TURKEY_LIGTH;
  nextBtnt2.borderColor = 0;
  nextBtnt2.borderSize = 0;
  nextBtnt2.x0 = nextBtn.xo +2 + 20;
  nextBtnt2.y0 = nextBtn.yo;
  nextBtnt2.x1 = nextBtn.xo +2;
  nextBtnt2.y1 = nextBtn.yo - 10;
  nextBtnt2.x2 = nextBtn.xo +2 ;
  nextBtnt2.y2 = nextBtn.yo + 10;
}

void setTrackText()
{
  textTrack.color = TURKEY_LIGTH;
  textTrack.f = mono12x7bold;
  textTrack.size = 1;
  sprintf( textTrack.text, "Trk%d ", currentSong);
  textTrack.xo = 100;
  textTrack.yo = 280;
}

void drawMusicSymbol()
{
  struct circle circ1;

  circ1.backgroundColor = BLACK;
  circ1.borderColor = 0;
  circ1.borderSize = 0;
  circ1.radius = 20;
  circ1.xo = 169;
  circ1.yo = 190;

  struct Rectangle rect1;

  rect1.backgroundColor = BLACK;
  rect1.borderColor = 0;
  rect1.borderSize = 0;
  rect1.width = 15;
  rect1.heigth = 80;
  rect1.xo = 175;
  rect1.yo =110;
  rect1.circleBorder = 0;

  struct triangle tri1;
  tri1.backgroundColor = BLACK;
  tri1.borderColor = 0;
  tri1.borderSize = 0;
  tri1.x0 = 175;
  tri1.y0 = 125;
  tri1.x1 = 280;
  tri1.y1 = 110;
  tri1.x2 = 175;
  tri1.y2 = 90;

  struct triangle tri2;
  tri2.backgroundColor = BLACK;
  tri2.borderColor = 0;
  tri2.borderSize = 0;
  tri2.x0 = tri1.x1;
  tri2.y0 = tri1.y1;
  tri2.x1 = tri2.x0;
  tri2.y1 = tri2.y0 - 35;
  tri2.x2 = tri1.x2;
  tri2.y2 = tri1.y2;

  struct Rectangle rect2;

  rect2.backgroundColor = BLACK;
  rect2.borderColor = 0;
  rect2.borderSize = 0;
  rect2.width = 15;
  rect2.heigth = 80;
  rect2.xo = 266;
  rect2.yo =90;
  rect2.circleBorder = 0;

  struct circle circ2;

  circ2.backgroundColor = BLACK;
  circ2.borderColor = 0;
  circ2.borderSize = 0;
  circ2.radius = 20;
  circ2.xo = 260;
  circ2.yo = 170;

  struct Rectangle banda;

  banda.backgroundColor = TURKEY_LIGTH;
  banda.borderColor = TURKEY_LIGTH;
  banda.borderSize = 0;
  banda.circleBorder = 0;
  banda.heigth = 20;
  banda.width = 440;
  banda.xo = 20;
  banda.yo = 220;

  prevBtn.backgroundColor = TURKEY_LIGTH;
  prevBtn.borderColor = 0;
  prevBtn.borderSize = 0;
  prevBtn.radius = 30;
  prevBtn.xo = 260 - 50;
  prevBtn.yo = 280;

  setPrevButton();

  playBtn.backgroundColor = TURKEY_LIGTH;
  playBtn.borderColor = 0;
  playBtn.borderSize = 0;
  playBtn.radius = 30;
  playBtn.xo = 330- 50;
  playBtn.yo = 280;

  setPlayButton();

  nextBtn.backgroundColor = TURKEY_LIGTH;
  nextBtn.borderColor = 0;
  nextBtn.borderSize = 0;
  nextBtn.radius = 30;
  nextBtn.xo = 400- 50;
  nextBtn.yo = 280;

  setNextButton();

  setTrackText();

  setVolumneGraph();

  ReturnBtn.r.backgroundColor = WHITE;
  ReturnBtn.r.borderColor = RED;
  ReturnBtn.r.borderSize = 4;
  ReturnBtn.r.circleBorder = 4;
  ReturnBtn.r.xo = 400;
  ReturnBtn.r.yo = 250;
  ReturnBtn.r.width = 60;
  ReturnBtn.r.heigth = 60;
  ReturnBtn.t.color = RED;
  ReturnBtn.t.f = mono12x7bold;
  ReturnBtn.t.size = 2;
  strcpy(  ReturnBtn.t.text , "R");
  ReturnBtn.t.xo = ReturnBtn.r.xo + 10;
  ReturnBtn.t.yo = ReturnBtn.r.yo + ReturnBtn.r.heigth  - 20;
  ReturnBtn.isPressed = false;
  ReturnBtn.pressedColor = RED;
  ReturnBtn.pressedTextColor = WHITE;
  ReturnBtn.releasedColor = WHITE;
  ReturnBtn.releasedTextColor = RED;
  ReturnBtn.tm.tStart = 500;

  circle_draw(&circ1);
  rectangle_draw(&rect1);
  triangle_draw(&tri1);
  triangle_draw(&tri2);
  rectangle_draw(&rect2);
  circle_draw(&circ2);
  rectangle_draw(&banda);
  circle_draw(&prevBtn);
  circle_draw(&playBtn);
  circle_draw(&nextBtn);

  drawPrevButton();
  drawPlayButton();
  drawNextButton();
  
  text_draw(&textTrack);

  triangle_draw(&volExt);
  drawVol();
  button_draw(&ReturnBtn);

  selectOptionBar(barSelection);
  
}

int screenPlayer_show()
{
  struct text introtext;

  introtext.color = TURKEY_LIGTH;
  introtext.f = mono12x7bold;
  introtext.size = 2;
  strcpy( introtext.text, "Player" );
  introtext.xo = 170;
  introtext.yo = 30;

  fillScreen(WHITE);
  text_draw(&introtext);
  drawMusicSymbol();

  return -1;
}

int screenPlayer_init( struct screenManager *sm  )
{
  int err = -1;
  sm->s.tc.delay = 0;
  err = screenPlayer_show();

  js2.sw_port = JS_SW_GPIO_Port;
  js2.swPin = JS_SW_Pin;
  js2.hadc_js = &hadc1;
  js2.adcChannels[0] = 12;
  js2.adcChannels[1] = 13;
  js2.tm.delay = 50;
  joystick_init(&js2);

  //currentSong = dfpcms_getCurrentSong( );

  return err;
}

int screenPlayer_eval(struct screenManager *sm)
{
  /*static int volumeGet = 0;
  long f = map( volumeGet, 0, 30, 0, VOLT_EXT_CIRC_R - VOLT_INT_INTERNAL_DISTANCE );
  volumeEquivalent = (uint8_t)f;
  setVolumneGraph();
  drawVol();
  volumeGet += 1;
  volumeGet %= 30;
  if ( volumeGet % 5 == 0 )
  {
    statusSong = !statusSong;
    drawPlayButton();
  }
  HAL_Delay(1000);*/

  joystick_getValuesByTimer(&js2 , checkOptionsPlayer );
  HAL_Delay(1);

  return -1;
}