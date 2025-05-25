#ifndef REDUBGSR_H
#define REDUBGSR_H

#include "Watchy_GSR.h"
#include "background.h"
#include "Technology18pt7b.h"
#include "Technology40pt7b.h"

RTC_DATA_ATTR uint8_t RedubStyle;

// Noms de jours en français (abréviations 3 lettres)
static const char *JOURS_FR[] = {"LUN", "MAR", "MER", "JEU", "VEN", "SAM", "DIM"};

class RedubGSRClass : public WatchyGSR
{
public:
    RedubGSRClass() : WatchyGSR() { initAddOn(this); }

    void RegisterWatchFaces() override
    {
        RedubStyle = AddWatchStyle("Redub", this);
    }

    void InsertDrawWatchStyle(uint8_t StyleID) override
    {
        float batt = getBatteryVoltage() / 4.2f;
        batt = batt < 0 ? 0 : (batt > 1 ? 1 : batt);

        uint16_t fg = ForeColor();
        uint16_t bg = BackColor();

        display.fillScreen(bg);
        display.drawBitmap(0, 0, background, 200, 200, fg);

        display.setTextColor(bg);
        display.setTextWrap(false);

        // — Date (jour en français)
        display.setFont(&Technology18pt7b);
        uint8_t wday = (WatchTime.Local.Wday + 6) % 7;
        String ds = String(JOURS_FR[wday]) + " " + String(WatchTime.Local.Day);
        ds.toUpperCase();
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(ds, 0, 0, &x1, &y1, &w, &h);
        display.setCursor(175 - w, 90);
        display.print(ds);

        // — Heure et minutes (deux chiffres)
        display.setFont(&Technology40pt7b);

        // Heure
        uint8_t H = WatchTime.Local.Hour;
        String sh = (H < 10) ? (String("0") + H) : String(H);
        String c = String(sh.charAt(0));
        display.getTextBounds(c, 0, 0, &x1, &y1, &w, &h);
        display.setCursor(58 - w, 152);
        display.print(c);
        c = String(sh.charAt(1));
        display.getTextBounds(c, 0, 0, &x1, &y1, &w, &h);
        display.setCursor(91 - w, 152);
        display.print(c);

        // Double-point
        String colon = ":";
        display.getTextBounds(colon, 0, 0, &x1, &y1, &w, &h);
        display.setCursor(100 - w / 2, 152);
        display.print(colon);

        // Minutes
        uint8_t M = WatchTime.Local.Minute;
        String sm = (M < 10) ? (String("0") + M) : String(M);
        c = String(sm.charAt(0));
        display.getTextBounds(c, 0, 0, &x1, &y1, &w, &h);
        display.setCursor(142 - w, 152);
        display.print(c);
        c = String(sm.charAt(1));
        display.getTextBounds(c, 0, 0, &x1, &y1, &w, &h);
        display.setCursor(175 - w, 152);
        display.print(c);

        // — Batterie
        display.fillRect(160, 53, 15 * batt, 6, bg);
    }
};

RedubGSRClass RedubGSRClassLoader;

#endif // REDUBGSR_H