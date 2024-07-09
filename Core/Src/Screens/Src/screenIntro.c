#include "screenIntro.h"
#include "Rectangle.h"
#include "Text.h"
#include "Circle.h"

#include "listColors.h"
#include "fonts.h"
#include "tft.h"
#include "functions.h"

#define DELAY_TIMER_MILLIS 2000

int screenIntro_show()
{
  int err = -1;

  struct Rectangle rect1;

  struct text introtext;
  struct text introtext2;
  struct text introtext3;

  introtext.color = TURKEY_LIGTH;
  introtext.f = mono12x7bold;
  introtext.size = 3;
  strcpy( introtext.text, "Music" );
  introtext.xo = 140;
  introtext.yo = 100;

  introtext2.color = TURKEY_LIGTH;
  introtext2.f = mono12x7bold;
  introtext2.size = 3;
  strcpy( introtext2.text, "Player" );
  introtext2.xo = 120;
  introtext2.yo = 175;

  introtext3.color = TURKEY_LIGTH;
  introtext3.f = mono12x7bold;
  introtext3.size = 3;
  strcpy( introtext3.text, "App" );
  introtext3.xo = 160;
  introtext3.yo = 250;

  rect1.backgroundColor = WHITE;
  rect1.borderColor = TURKEY_LIGTH;
  rect1.borderSize = 15;
  rect1.circleBorder = 10;
  rect1.heigth = 280;
  rect1.width = 440;
  rect1.xo = 20;
  rect1.yo = 20;

  fillScreen(WHITE);

  rectangle_draw(&rect1);
  text_draw(&introtext);
  text_draw(&introtext2);
  text_draw(&introtext3);
  return err;
}

int screenIntro_init( struct screenManager *sm )
{
  int err = -1;
  sm->s.tc.delay = DELAY_TIMER_MILLIS;
  err = screenIntro_show();
  return err;
}

