#include "screenMainMenu.h"
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

#include "screenPlayer.h"

int numberOfSongsTrack = 7;
int trackPage = 0;
#define TRACKS_PER_PAGE 4

int trackListDisplayPosition = 0;

struct button trackListOnDisplay[4];
#define TRACK_LIST_LEN  4 

#define SELECTOR_X0 340
#define SELECTOR_Y0 90
#define SELECTOR_X1 360
#define SELECTOR_Y1 70
#define SELECTOR_X2 360
#define SELECTOR_Y2 110

#define SELECTOR_Y_STEP 55

struct triangle selectorTrackList;
extern struct joystick js2;

struct text tracksPage;

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

bool changeScreen = false;
struct timeCounter timeVerificationMain;

int calculateEndTrackPage()
{
  return trackPage*TRACKS_PER_PAGE + 3 < numberOfSongsTrack-1?trackPage*TRACKS_PER_PAGE + 3 : numberOfSongsTrack-1;
}	

void setTrackPage()
{
  tracksPage.color = TURKEY_LIGTH;
  tracksPage.f = mono9x7bold;
  tracksPage.size = 2;
  sprintf( tracksPage.text, " %d-%d of %d",trackPage*TRACKS_PER_PAGE + 1  , calculateEndTrackPage() + 1, dfpcms_getLocalNumberOfSongs());
  tracksPage.xo = 200;
  tracksPage.yo = 30;
}

int checkPostionJoystick( struct joystick *js )
{
  if ( js->adc_y >= 500 && js->adc_y <= 3500  )
  {
    if (!js->sw) return -1;
    if ( js->tm.delay == 250 ) js->tm.delay = 50;
    return 1;
  }
  return -1;
}

void checkOptions(struct joystick *js)
{
  int err = checkPostionJoystick(js);
  if (err == 1) return;
  if( js->adc_y < 500 && trackPage*TRACKS_PER_PAGE + trackListDisplayPosition < numberOfSongsTrack-1 )
  {
    selectorTrackList.backgroundColor = TURKEY_LIGTH;
    triangle_draw(&selectorTrackList);
    //trackListOnDisplay[trackListDisplayPosition].r.backgroundColor = TURKEY_LIGTH;
    //button_draw(&trackListOnDisplay[trackListDisplayPosition]);
    trackListDisplayPosition = trackPage*TRACKS_PER_PAGE + trackListDisplayPosition < numberOfSongsTrack-1?trackListDisplayPosition +1 : trackListDisplayPosition ;
    //trackListOnDisplay[trackListDisplayPosition].r.backgroundColor = TURKEY_DARK;
    //button_draw(&trackListOnDisplay[trackListDisplayPosition]);
    if ( trackListDisplayPosition == TRACKS_PER_PAGE ){
      trackPage += 1;
      trackListDisplayPosition = 0;
      setUpListOfSongs();
      for( int i = 0; i < TRACK_LIST_LEN; i++ ) trackListOnDisplay[i].r.backgroundColor == WHITE?rectangle_draw(&trackListOnDisplay[i].r):button_draw( &trackListOnDisplay[i] );
      setTrackPage();
      fillRoundRect(175 , 5 , 300, 30, 0 , WHITE);
      text_draw(&tracksPage);
      triangle_draw( &selectorTrackList );
    }
    else{
      selectorTrackList.y0 += SELECTOR_Y_STEP;
      selectorTrackList.y1 += SELECTOR_Y_STEP;
      selectorTrackList.y2 += SELECTOR_Y_STEP;
      selectorTrackList.backgroundColor = WHITE;
      triangle_draw(&selectorTrackList);
    }
  
    if (  js->tm.delay == 50 ) js->tm.delay = 250;
  } else if( js->adc_y > 3500 && trackPage*TRACKS_PER_PAGE + trackListDisplayPosition > 0 )
  {
    selectorTrackList.backgroundColor = TURKEY_LIGTH;
    triangle_draw(&selectorTrackList);
    //trackListOnDisplay[trackListDisplayPosition].r.backgroundColor = TURKEY_LIGTH;
    //button_draw(&trackListOnDisplay[trackListDisplayPosition]);
    trackListDisplayPosition = trackListDisplayPosition == 0?TRACKS_PER_PAGE-1:trackListDisplayPosition-1;
    //trackListOnDisplay[trackListDisplayPosition].r.backgroundColor = TURKEY_DARK;
    //button_draw(&trackListOnDisplay[trackListDisplayPosition]);
    if ( trackListDisplayPosition == TRACKS_PER_PAGE-1 && trackPage > 0 ){
      trackPage -= 1;
      setUpListOfSongs();
      for( int i = 0; i < TRACK_LIST_LEN; i++ ) trackListOnDisplay[i].r.backgroundColor == WHITE?rectangle_draw(&trackListOnDisplay[i].r):button_draw( &trackListOnDisplay[i] );
      setTrackPage();
      fillRoundRect(175 , 5 , 300, 30, 0 , WHITE);
      text_draw(&tracksPage);
      triangle_draw( &selectorTrackList );
    }
    else
    {
      selectorTrackList.y0 -= SELECTOR_Y_STEP;
      selectorTrackList.y1 -= SELECTOR_Y_STEP;
      selectorTrackList.y2 -= SELECTOR_Y_STEP;
      selectorTrackList.backgroundColor = WHITE;
      triangle_draw(&selectorTrackList);
    }
    
    if (  js->tm.delay == 50 ) js->tm.delay = 250;
  }
  else if ( js->adc_y >= 500 && js->adc_y <= 3500 && !js->sw)  {
    selectorTrackList.backgroundColor = RED;
    triangle_draw(&selectorTrackList);
    while (!js->sw){
      joystick_getSWvalue(js);
      HAL_Delay(1);
    }
    selectorTrackList.backgroundColor = WHITE;
    triangle_draw(&selectorTrackList);
    int fgsong = dfpcms_getCurrentSong();
    if ( fgsong != trackPage*TRACKS_PER_PAGE + trackListDisplayPosition )
    {
      dfpcms_waitingPlayPause(STATUS_PAUSE);
      setStatusSongFromMainMenu(false);
      dfpcms_waitingSetupSong(trackPage*TRACKS_PER_PAGE + trackListDisplayPosition);

      //dfpcms_setSong(trackPage*TRACKS_PER_PAGE + trackListDisplayPosition);
      //dfpcms_setPrevSong(trackPage*TRACKS_PER_PAGE + trackListDisplayPosition );
    }
    changeScreen = true;
  }
}


void setUpListOfSongs()
{
  for ( int i2 = trackPage*TRACKS_PER_PAGE; i2 <= trackPage*TRACKS_PER_PAGE + 3 ; i2++ )
  {
    int i = i2%TRACKS_PER_PAGE;
    trackListOnDisplay[i].r.backgroundColor =  i2 >= numberOfSongsTrack?WHITE:TURKEY_LIGTH;//trackListPosition == i?TURKEY_DARK:TURKEY_LIGTH;
    trackListOnDisplay[i].r.borderColor = TURKEY_LIGTH;
    trackListOnDisplay[i].r.borderSize = 0;
    trackListOnDisplay[i].r.circleBorder = 0;
    trackListOnDisplay[i].r.xo = 40;
    trackListOnDisplay[i].r.yo = 65 + (i * SELECTOR_Y_STEP);
    trackListOnDisplay[i].r.width = 370;
    trackListOnDisplay[i].r.heigth = 50;
    trackListOnDisplay[i].t.color = WHITE;
    trackListOnDisplay[i].t.f = mono12x7bold;
    trackListOnDisplay[i].t.size = 2;
    sprintf( trackListOnDisplay[i].t.text, "Track %02d ", trackPage*TRACKS_PER_PAGE + i );
    trackListOnDisplay[i].t.xo = trackListOnDisplay[i].r.xo + 10;
    trackListOnDisplay[i].t.yo = trackListOnDisplay[i].r.yo + 40;
    trackListOnDisplay[i].isPressed = false;
    trackListOnDisplay[i].pressedColor = GREY_LIGTH;
    trackListOnDisplay[i].pressedTextColor = TURKEY_LIGTH;
    trackListOnDisplay[i].releasedColor = WHITE;
    trackListOnDisplay[i].releasedTextColor = TURKEY_LIGTH;
    trackListOnDisplay[i].tm.tStart = 50;

    if (  trackListDisplayPosition == i)
    {
      selectorTrackList.x0 = 340 ;
      selectorTrackList.y0 = 90+ SELECTOR_Y_STEP*trackListDisplayPosition;
      selectorTrackList.x1 = 360;
      selectorTrackList.y1 = 70+ SELECTOR_Y_STEP*trackListDisplayPosition;
      selectorTrackList.x2 = 360;
      selectorTrackList.y2 = 110+ SELECTOR_Y_STEP*trackListDisplayPosition;
      selectorTrackList.backgroundColor = WHITE;
      selectorTrackList.borderColor = 0;
      selectorTrackList.borderSize = 0;
    }
  }
}

int screenMainMenu_show()
{
  int err = -1;

  struct text introtext;
  struct Rectangle borderTrack;

  introtext.color = TURKEY_LIGTH;
  introtext.f = mono12x7bold;
  introtext.size = 2;
  strcpy( introtext.text, "Tracks" );
  introtext.xo = 20;
  introtext.yo = 30;

  borderTrack.backgroundColor = WHITE;
  borderTrack.borderColor = TURKEY_LIGTH;
  borderTrack.borderSize = 10;
  borderTrack.circleBorder = 0;
  borderTrack.heigth = 250;
  borderTrack.width = 420;
  borderTrack.xo = 20;
  borderTrack.yo = 50;

  numberOfSongsTrack = dfpcms_getLocalNumberOfSongs();
  
  
  int currentSong = dfpcms_getCurrentSong();
  if ( currentSong == -1 )
  {
    trackPage = 0;
    trackListDisplayPosition = 0;
  }
  else{
    trackPage = currentSong/TRACKS_PER_PAGE;
    trackListDisplayPosition = currentSong%TRACKS_PER_PAGE;
  }

  setUpListOfSongs();

  fillScreen(WHITE);

  text_draw(&introtext);
  rectangle_draw(&borderTrack);
  for( int i = 0; i < TRACK_LIST_LEN; i++ ) button_draw( &trackListOnDisplay[i] );
  triangle_draw( &selectorTrackList );
  setTrackPage();
  fillRoundRect(175 , 5 , 300, 30, 0 , WHITE);
  text_draw(&tracksPage);
  return err;
}

int screenMainMenu_init( struct screenManager *sm  )
{
  int err = -1;
  sm->s.tc.delay = 0;
  err = screenMainMenu_show();

  timeVerificationMain.delay = 1500;
  timeCounter_init( &timeVerificationMain );
  timeCounter_initTimer( &timeVerificationMain );
  js2.sw_port = JS_SW_GPIO_Port;
  js2.swPin = JS_SW_Pin;
  js2.hadc_js = &hadc1;
  js2.adcChannels[0] = 12;
  js2.adcChannels[1] = 13;

  js2.tm.delay = 50;

  joystick_init(&js2);
  return err;
}
int screenMainMenu_eval(struct screenManager *sm)
{
  static int counterStatusMain = 0;
  joystick_getValuesByTimer(&js2 , checkOptions );
  HAL_Delay(1);
  counterStatusMain += 1;
  if ( counterStatusMain >= 200 )
  {
    DFPCMS_getStatus();
    counterStatusMain = 0;
  }
  if ( timeCounter_verifyTimer(&timeVerificationMain) )
  {
    if ( dfpcms_getStatusLocal() && returnSongStatus() )
    {
      uint8_t newSong = dfpcms_getCurrentSong() == dfpcms_getLocalNumberOfSongs( ) -1 ? 0 : dfpcms_getCurrentSong() +1;
      dfpcms_waitingSetupSong( newSong );
      setSongFromMainMenu(newSong);
      setStatusSongFromMainMenu(true);
      dfpcms_waitingPlayPause(STATUS_PLAY);
      DFPCMS_getStatus();
      timeCounter_resetTimer( &timeVerificationMain );
      counterStatusMain = 0;
    }
  }/**/

  if( changeScreen )
  {
    setBarInit();
    changeScreen = false;
    sm->actualScreen = 5;
  }	
  return -1;
}	