#include "Watchy_GSR.h"
#include "stationaryfonts/NunitoSans_Bold28pt7b.h"
#include "stationaryfonts/NunitoSans_Light28pt7b.h"

RTC_DATA_ATTR uint8_t TextualTimeStyle;
int TextualTimeState = 0;

class TextualTimeGSR : public WatchyGSR
{
public:
  TextualTimeGSR() : WatchyGSR() { initAddOn(this); }

  void RegisterWatchFaces()
  {
    TextualTimeStyle = AddWatchStyle("Text", this);
  }

  void InsertInitWatchStyle(uint8_t StyleID)
  {
    if (StyleID == TextualTimeStyle)
    {
      Design.Face.Bitmap = nullptr;
      Design.Face.SleepBitmap = nullptr;
      Design.Face.Gutter = 0;
      Design.Face.TimeFont = &NunitoSans_Bold28pt7b;
      Design.Face.TimeColor = GSR_AutoFore;
      Design.Status.Inverted = false;
    }
  }

  void InsertDrawWatchStyle(uint8_t StyleID)
  {
    if (StyleID == TextualTimeStyle)
    {
      if (!SafeToDraw())
        return;

      display.fillScreen(BackColor());
      display.setTextColor(ForeColor());
      display.setTextWrap(false);

      static const char *lows[10] = {
          "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
      static const char *teensone[11] = {
          "", "ten", "eleven", "twelve", "thir", "four", "fif", "six", "seven", "eight", "nine"};
      static const char *teenstwo[11] = {
          "", "", "", "teen", "teen", "teen", "teen", "teen", "teen", "teen", "teen"};
      static const char *tens[10] = {
          "zero", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

      uint16_t lines = 0;
      display.setFont(&NunitoSans_Bold28pt7b);

      // heures en toutes lettres
      display.setCursor(8, lines * 47 + 42);
      if (WatchTime.Local.Hour == 0)
      {
        display.print(tens[2]); // "twenty"
        lines++;
        display.setCursor(8, lines * 47 + 42);
        display.print(lows[4]); // "four"
      }
      else if (WatchTime.Local.Hour < 10)
      {
        display.print(lows[WatchTime.Local.Hour]);
      }
      else if (WatchTime.Local.Hour < 20)
      {
        display.print(teensone[WatchTime.Local.Hour - 9]);
        if (WatchTime.Local.Hour > 12)
        {
          lines++;
          display.setCursor(8, lines * 47 + 42);
          display.print(teenstwo[WatchTime.Local.Hour % 10]);
        }
      }
      else
      {
        display.print(tens[WatchTime.Local.Hour / 10]);
        if (WatchTime.Local.Hour % 10 > 0)
        {
          lines++;
          display.setCursor(8, lines * 47 + 42);
          display.print(lows[WatchTime.Local.Hour % 10]);
        }
      }

      // minutes en toutes lettres
      lines++;
      display.setCursor(8, lines * 47 + 42);
      display.setFont(&NunitoSans_Light28pt7b);

      if (WatchTime.Local.Minute == 0)
      {
        display.print("o'clock");
      }
      else if (WatchTime.Local.Minute < 10)
      {
        display.print("oh");
        lines++;
        display.setCursor(8, lines * 47 + 42);
        display.print(lows[WatchTime.Local.Minute]);
      }
      else if (WatchTime.Local.Minute < 20)
      {
        display.print(teensone[WatchTime.Local.Minute - 9]);
        if (WatchTime.Local.Minute > 12)
        {
          lines++;
          display.setCursor(8, lines * 47 + 42);
          display.print(teenstwo[WatchTime.Local.Minute % 10]);
        }
      }
      else
      {
        display.print(tens[WatchTime.Local.Minute / 10]);
        if (WatchTime.Local.Minute % 10 > 0)
        {
          lines++;
          display.setCursor(8, lines * 47 + 42);
          display.print(lows[WatchTime.Local.Minute % 10]);
        }
      }
    }
  }
};

TextualTimeGSR TextualTimeGSRLoader;