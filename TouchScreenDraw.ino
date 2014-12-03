#include <Adafruit_GFX.h>
#include "SWTFT.h"

#include "TouchScreen.h"

#define XP 6
#define YP A1
#define XM A2
#define YM 7
#define TS_MINX 100
#define TS_MINY 140
#define TS_MAXX 940
#define TS_MAXY 940

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

SWTFT tft;

int resetCounter = 0;

void resetScreen()
{
  int x, y, w, h, d;

  resetCounter = 0;
  
  tft.fillScreen(BLACK);
    
  w = tft.width();
  h = tft.height(); 
  d = 20;
  
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  for(int x=0; x<w; x+=d)
  {
    tft.drawFastVLine(x, 0, h, WHITE);
    tft.setCursor(x, 0);
    tft.print(x);
  }
  tft.setCursor(228, 10);
  tft.print("Y");
  
  for(int y=0; y<h; y+=d)
  {
    tft.drawFastHLine(0, y, w, WHITE);
    tft.setCursor(0, y);
    tft.print(y);
  }
  tft.setCursor(8, 310);
  tft.print("X");
}

void setup(void)
{
  tft.reset();
  tft.setRotation(0);
  uint16_t identifier = tft.readID();  
   
  Serial.begin(9600);
  Serial.print("LCD driver chip: ");
  Serial.println(identifier, HEX);

  tft.begin(identifier);
  resetScreen();
}

void loop()
{
  Point p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, tft.height());
    p.y = map(p.y, TS_MAXY, TS_MINY, 0, tft.width());

    tft.fillRect(162, 306, tft.width() - 164, 12, 0x0000);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    
    tft.setCursor(164, 308);
    tft.print(" X=");
    tft.print(p.y);
    tft.print(" Y=");
    tft.print(p.x);
    
    tft.fillCircle(p.y, p.x, 8, GREEN); 
    
    if (resetCounter <= 0) {
      resetCounter = 1;
    }
  }
  else
  {
    if (resetCounter > 0) {
      resetCounter++;
    }
  }
  
  if(resetCounter > 5000) {
    resetScreen();
  }
}
