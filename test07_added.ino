
#include <Adafruit_ILI9341.h>

#define TFT_CS   33
#define TFT_DC   32
#define TFT_MOSI 14
#define TFT_CLK  27
#define TFT_RST  26

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

void plotGraph(int x, int y, float value, int sensorNum, bool isTemperature)
{
  int graphWidth = 200;
  int graphHeight = 100;
  int graphMargin = 10;
  int x0 = x - graphWidth + graphMargin;
  int y0 = y + graphMargin;
  int x1 = x - graphMargin;
  int y1 = y + graphHeight - graphMargin;
  int xTick = graphWidth / 10;
  int yTick = graphHeight / 10;
  
  // draw the graph border
  tft.drawRect(x0, y0, graphWidth - graphMargin * 2, graphHeight - graphMargin * 2, ILI9341_WHITE);
  
  // draw the x-axis and y-axis
  tft.drawLine(x0, y1, x1, y1, ILI9341_WHITE);
  tft.drawLine(x0, y1, x0, y0, ILI9341_WHITE);
  
  // draw the x-axis ticks and labels
  for (int i = 1; i <= 10; i++)
  {
    int xTickPos = x0 + i * xTick;
    tft.drawLine(xTickPos, y1, xTickPos, y1 - 5, ILI9341_WHITE);
    tft.setCursor(xTickPos - 10, y1 + 5);
    tft.print(i);
  }
  
  // draw the y-axis ticks and labels
  for (int i = 1; i <= 10; i++)
  {
    int yTickPos = y1 - i * yTick;
    tft.drawLine(x0, yTickPos, x0 + 5, yTickPos, ILI9341_WHITE);
    tft.setCursor(x0 - 25, yTickPos - 5);
    tft.print(i * 10);
  }
  
  // plot the value on the graph
  int valuePos = map(value * 10, 0, 100, y1, y0);
  static float prevValue[SENSOR_NUM][2] = {0};
  int color = (isTemperature) ? ILI9341_RED : ILI9341_BLUE;
  tft.drawCircle(x1, valuePos, 2, color);
  tft.drawLine(x1 - 3, valuePos, x1 + 3, valuePos, color);
  tft.drawLine(x1, valuePos - 3, x1, valuePos + 3, color);
  
  // clear the previous value
  int prevValuePos = map(prevValue[sensorNum][isTemperature], 0, 100, y1, y0);
  tft.drawCircle(x1, prevValuePos, 2, ILI9341_BLACK);
  tft.drawLine(x1 - 3, prevValuePos, x1 + 3, prevValue
