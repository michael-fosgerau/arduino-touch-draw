// Connect 5V + GND for TFT (and 3.3v + GND for SD)
#define LCD_D0    D8
#define LCD_D1    D9
#define LCD_D2    D2
#define LCD_D3    D3
#define LCD_D4    D4
#define LCD_D5    D5
#define LCD_D6    D6
#define LCD_D7    D7

#define LCD_RESET A4
#define LCD_CS    A3    
#define LCD_CD    A2    
#define LCD_WR    A1   
#define LCD_RD    A0    

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

#include "TFTLCD.h"

TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

unsigned long timeStart[13];
unsigned long timeEnd[13];
int testNo = 0;

void setup(void) {
  Serial.begin(9600);
  Serial.println("GFX Performance Test");
  
  tft.reset();
  tft.initDisplay();
}

void loop(void) {
  testNo = 0;
  testText();            
  testFillRoundRect();   
  testRoundRect();       
  testTriangles();       
  testFillTriangles();   
  testFillCircles(10, BLUE);
  testDrawCircles(10, WHITE);
  testFillRects(BLUE, RED);
  testDrawRects(BLUE); 
  testFastLines(BLUE, RED);
  testLines();       
  testBars();
  
  tft.println("");
  tft.setTextSize(3);
  tft.println(" WAITING 10s");
  delay(10000);
}

void initTestScreen(String testName)
{
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("TEST: ");
  tft.print(testName);
  tft.setCursor(0, 0);
  tft.goHome();
}

void outputStatLine(int t, String test)
{
  char buf[10];
  String deltaOut = "";
  float delta;
  if (testNo == 1+t) { 
    tft.setTextColor(GREEN); 
  } else { 
    tft.setTextColor(WHITE); 
  }
  deltaOut = "";
  delta = (timeEnd[t]-timeStart[t++])/1000000.0;
  dtostrf(delta,3,6,buf);
  if(delta < 10) {
    deltaOut = " ";
  }
  deltaOut += buf;
  tft.print(" ");
  tft.print(test);
  tft.print(deltaOut);
  tft.println(" s");
  tft.println("");
}

void displayStats()
{
  initTestScreen("displayStats");
  
  int t = 0;
  timeStart[12] = micros();
  tft.setCursor(0, 30);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print(" ");
  tft.setTextSize(2);
  tft.println("GFX PERFORMANCE");
  tft.println("");
  tft.setTextSize(1);
  
  outputStatLine(t++, "testText ............ ");
  outputStatLine(t++, "testRoundRect ....... ");
  outputStatLine(t++, "testTriangles ....... ");
  outputStatLine(t++, "testFillTriangles ... ");
  outputStatLine(t++, "testFillCircles ..... ");
  outputStatLine(t++, "testDrawCircles ..... ");
  outputStatLine(t++, "testFillRects ....... ");
  outputStatLine(t++, "testDrawRects ....... ");
  outputStatLine(t++, "testFastLines ....... ");
  outputStatLine(t++, "testLines ........... ");
  outputStatLine(t++, "testRoundRect ....... ");
  outputStatLine(t++, "testBars ............ ");
  timeEnd[12] = micros();
  outputStatLine(t++, "displayStats ........ ");
     
  delay(1000);
}

void testText() {
  initTestScreen("testText");
  
  timeStart[testNo] = micros();
  
  tft.setCursor(0, 20);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.println("Hello Arduino!");
  
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.println(1234.56);
  
  tft.setTextColor(BLUE);
  tft.setTextSize(5);
  tft.println(0xDEADBEEF, HEX);

  timeEnd[testNo++] = micros();
  displayStats();
}

void testFillRoundRect() {
  initTestScreen("testFillRoundRect");
  
  timeStart[testNo] = micros();

  for (uint16_t x=tft.width(); x > 20 ; x-=6) {
    tft.fillRoundRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, x/8,  tft.Color565(0, x, 0));
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testRoundRect() {
  initTestScreen("testRoundRect");
  
  timeStart[testNo] = micros();
  for (uint16_t x=0; x < tft.width(); x+=6) {
    tft.drawRoundRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, x/8, tft.Color565(x, 0, 0));
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testTriangles() {
  initTestScreen("testTriangles");
  
  timeStart[testNo] = micros();
  for (uint16_t i=0; i<tft.width()/2; i+=5) {
    tft.drawTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, tft.Color565(0, 0, i));
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testFillTriangles() {
  initTestScreen("testFillTriangles");
    
  timeStart[testNo] = micros();
  for (uint16_t i=tft.width()/2; i>10; i-=5) {
    tft.fillTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, 
                     tft.Color565(0, i, i));
    tft.drawTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, tft.Color565(i, i, 0));    
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testFillCircles(uint8_t radius, uint16_t color) {
  initTestScreen("testFillCircles");
  
  timeStart[testNo] = micros();
  for (uint16_t x=radius; x < tft.width(); x+=radius*2) {
    for (uint16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testDrawCircles(uint8_t radius, uint16_t color) {
  initTestScreen("testDrawCircles");
  
  timeStart[testNo] = micros();
  for (uint16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (uint16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testFillRects(uint16_t color1, uint16_t color2) {
  initTestScreen("testFillRects");
 
  timeStart[testNo] = micros();
  for (uint16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testDrawRects(uint16_t color) {
  initTestScreen("testDrawRects");
 
  timeStart[testNo] = micros();
  for (uint16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testFastLines(uint16_t color1, uint16_t color2) {
  initTestScreen("testFastLines");
  
  timeStart[testNo] = micros();
  for (uint16_t y=0; y < tft.height(); y+=5) {
    tft.drawHorizontalLine(0, y, tft.width(), color1);
  }
  for (uint16_t x=0; x < tft.width(); x+=5) {
    tft.drawVerticalLine(x, 0, tft.height(), color2);
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testLines() {
  initTestScreen("testLines");
  
  uint16_t color = RED;  
  timeStart[testNo] = micros();
  for (uint16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
  }
  color = BLUE;
  for (uint16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
  }
  
  color = WHITE; 
  for (uint16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
  }
  color = GREEN;
  for (uint16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
  }
  
  color = CYAN;
  for (uint16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
  }
  color = MAGENTA;
  for (uint16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
  }

  color = YELLOW;
  for (uint16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
  }
  color = RED;
  for (uint16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

void testBars() {
  initTestScreen("testBars");
  
  timeStart[testNo] = micros();
  uint16_t i,j;
  for(i=0; i < tft.height(); i++)
  {
    for(j=0; j < tft.width(); j++)
    {
      if (i>279)      tft.writeData(WHITE);
      else if (i>239) tft.writeData(BLUE);
      else if (i>199) tft.writeData(GREEN);
      else if (i>159) tft.writeData(CYAN);
      else if (i>119) tft.writeData(RED);
      else if (i>79)  tft.writeData(MAGENTA);
      else if (i>39)  tft.writeData(YELLOW);
      else            tft.writeData(BLACK);
    }
  }
  timeEnd[testNo++] = micros();
  displayStats();
}

