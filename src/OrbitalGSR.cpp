#include <Fonts/FreeSansBold9pt7b.h>
#include "../Watchface Addons/Orbital/OrbitalGSR.h"
#include "../Watchface Addons/Orbital/MoonPhases20pt7b.h"
#include "../Watchface Addons/Orbital/OpenWeather_Regular12pt7b.h"

// ── Réglages “Orbital” ─────────────────────────────────────────────────────────
#define DETAILS_ENABLED true // passe à false pour désactiver les détails
float lat = 43.48333;
float lon = -1.518300;

const char *daysOfWeek[] = {
    "dimanche", "lundi", "mardi", "mercredi",
    "jeudi", "vendredi", "samedi"};

const String shortMonths[] = {
    "ja", "fe", "ma", "av", "ma", "ju", "ji",
    "ao", "se", "oc", "no", "de"};

int batteryVMax = 4200;
int batteryVMin = 3500;

// ── Instance de l’AddOn ────────────────────────────────────────────────────────
OrbitalGSR OrbitalGSRClassLoader;

// ── Constructeur + hooks GSR ───────────────────────────────────────────────────
OrbitalGSR::OrbitalGSR() : WatchyGSR()
{
    initAddOn(this);
}

void OrbitalGSR::RegisterWatchFaces()
{
    AddWatchStyle("Orbital", this);
}

void OrbitalGSR::InsertInitWatchStyle(uint8_t StyleID)
{
    if (StyleID == CurrentStyleID())
    {
        WantWeather(true); // activation météo
    }
}

void OrbitalGSR::InsertDrawWatchStyle(uint8_t StyleID)
{
    if (StyleID == CurrentStyleID() && SafeToDraw())
    {
        drawWatchFace();
    }
}

// ── Dessin principal ───────────────────────────────────────────────────────────
void OrbitalGSR::drawWatchFace()
{
    primaryColor = BackColor();
    secondaryColor = ForeColor();
    details = DETAILS_ENABLED;
    twentyFour = Is24HourMode();

    display.fillScreen(primaryColor);
    display.setTextWrap(false);
    display.setTextColor(secondaryColor);

    sun.setPosition(lat, lon, 0);
    sun.setCurrentDate(
        tmYearToCalendar(WatchTime.Local.Year),
        WatchTime.Local.Month,
        WatchTime.Local.Day);

    drawWatchMinute();
    drawWatchHour();
    drawWatchDay();
    drawWatchMonth();
    drawBattery();
    drawMoon();
    drawDayTime();

    if (IsWeatherAvailable())
        drawWeather();
    drawInternalTemp();

    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(0, 13);
    display.print(daysOfWeek[WatchTime.Local.Wday]);
}

void OrbitalGSR::drawWatchMinute()
{
    float ma = 360.0 * (WatchTime.Local.Minute / 60.0);
    int tick = 6;
    fillArc3(0, ma, 88, 13, secondaryColor, 1);
    fillArc2(tick, ma - tick, 88, 13, primaryColor, tick);
    fillArc2(ma + (details ? 2 * tick : tick), 360 - tick, 85, 7, secondaryColor, tick);
    if (details)
        drawDetails(ma, 88, String(WatchTime.Local.Minute));
}

void OrbitalGSR::drawWatchHour()
{
    float ha = twentyFour
                   ? 360.0 * (WatchTime.Local.Hour / 24.0)
                   : 360.0 * (((WatchTime.Local.Hour + 11) % 12 + 1) / 12.0);
    int tick = 360 / (twentyFour ? 24 : 12);
    fillArc3(0, ha, 68, 15, secondaryColor, 1);
    fillArc2(tick, ha - tick, 68, 15, primaryColor, tick);
    fillArc2(ha + tick, 360 - tick, 64, 8, secondaryColor, tick);
    if (details)
        drawDetails(ha, 68, String(WatchTime.Local.Hour));
}

void OrbitalGSR::drawWatchDay()
{
    float da = 360.0 * (WatchTime.Local.Day / 31.0);
    int tick = 360 / DaysPerMonth(WatchTime.Local.Year, WatchTime.Local.Month);
    fillArc3(0, da, 42, 15, secondaryColor, 2);
    if (da == 360.0)
    {
        fillArc2(tick, da, 42, 15, primaryColor, tick);
    }
    else
    {
        fillArc2(tick, da - tick, 42, 15, primaryColor, tick);
        fillArc2(da + (details ? 2 * tick : tick), 360 - tick, 40, 7, secondaryColor, tick);
    }
    if (details)
        drawDetails(da, 42, String(WatchTime.Local.Day));
}

void OrbitalGSR::drawWatchMonth()
{
    uint8_t m = WatchTime.Local.Month;
    float ma = 360.0f * ((m + 1) / 12.0f);
    int tick = 360 / 12; // 30°
    fillArc3(0, ma, 22, 17, secondaryColor, 2);

    if (ma == 360.0f)
    {
        fillArc2(tick, ma, 22, 17, primaryColor, tick);
    }
    else
    {
        fillArc2(tick, ma - tick, 22, 17, primaryColor, tick);
        fillArc2(ma + (details ? 2 * tick : tick),
                 360 - tick,
                 20, 8,
                 secondaryColor,
                 tick);
    }
    if (details)
    {
        drawDetails(ma, 22, shortMonths[m]);
    }
}

void OrbitalGSR::drawBattery()
{
    float voltage = getBatteryVoltage() * 1000.0f;
    float ba = 360 * (voltage - batteryVMin) / float(batteryVMax - batteryVMin);
    ba = constrain(ba, 0, 360);
    fillArc3(0, ba, 4, 8, secondaryColor, 1);
}

void OrbitalGSR::drawMoon()
{
    long long t = (long long)makeTime(WatchTime.Local);
    int mp = sun.moonPhase(t);
    display.setFont(&MoonPhases20pt7b);
    display.setCursor(4, 196);
    display.print((char)(mp + 0x50));
}

void OrbitalGSR::drawDayTime()
{
    float sr = sun.calcSunrise(), ss = sun.calcSunset();
    if (twentyFour)
    {
        fillArc3(sr / 1440.0f * 360.0f, ss / 1440.0f * 360.0f, 55, 2, secondaryColor, 1);
    }
    else
    {
        float sA = sr / 720.0f * 360.0f, ssA = ss / 720.0f * 360.0f - 360.0f;
        if (WatchTime.Local.Hour < 12)
            fillArc3(sA, 360, 55, 2, secondaryColor, 1);
        else
            fillArc3(0, ssA, 55, 2, secondaryColor, 1);
    }
    if (details)
    {
        int hrSun = floor(sr / 60.0f), hrSet = floor(ss / 60.0f);
        if (hrSun == WatchTime.Local.Hour)
        {
            float mA = fmod(sr, 60.0f) / 60.0f * 360.0f;
            fillArc3(mA, 360, 79, 1, secondaryColor, 1);
        }
        else if (hrSet == WatchTime.Local.Hour)
        {
            float mA = fmod(ss, 60.0f) / 60.0f * 360.0f;
            fillArc3(0, mA, 79, 1, secondaryColor, 1);
        }
    }
}

void OrbitalGSR::drawWeather()
{
    int code = GetWeatherID();
    int temp = GetWeatherTemperature();
    int tempFeel = GetWeatherTemperatureFeelsLike();

    // Sélection du caractère pour l'icône
    char icon = 'A';
    if (code >= 200 && code <= 232)
        icon = 'G';
    else if ((code >= 300 && code <= 321) || (code >= 520 && code <= 531))
        icon = 'E';
    else if (code >= 500 && code <= 504)
        icon = 'F';
    else if (code == 511 || (code >= 600 && code <= 622))
        icon = 'H';
    else if (code >= 701 && code <= 781)
        icon = 'I';
    else if (code == 800)
        icon = 'A';
    else if (code == 801)
        icon = 'B';
    else if (code == 802)
        icon = 'C';
    else if (code == 803 || code == 804)
        icon = 'D';
    // nuit
    if (WatchTime.Local.Hour < 8 || WatchTime.Local.Hour >= 20)
        icon += 0x20;

    // Affiche icône
    display.setFont(&OpenWeather_Regular12pt7b);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(icon), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(200 - w - 4, 27);
    display.print(icon);

    // Affiche température
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(165, 180);
    display.print(String(temp) + (IsMetric() ? " °C" : " °F"));
}

void OrbitalGSR::drawInternalTemp()
{
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(165, 195);
    display.print(SBMA.readTemperature());
}

void OrbitalGSR::fillArc2(float sa, float ea, unsigned r, unsigned w, unsigned c, float st)
{
    unsigned cx = 100, cy = 100;
    for (float i = sa; i <= ea; i += st)
    {
        float x1 = cx + (r - w / 2.0) * cos((i - 90) * DEG2RAD);
        float y1 = cy + (r - w / 2.0) * sin((i - 90) * DEG2RAD);
        float x2 = cx + (r + w / 2.0) * cos((i - 90) * DEG2RAD);
        float y2 = cy + (r + w / 2.0) * sin((i - 90) * DEG2RAD);
        display.drawLine(x1, y1, x2, y2, c);
    }
}

void OrbitalGSR::fillArc3(float sa, float ea, unsigned r, unsigned w, unsigned c, float st)
{
    unsigned cx = 100, cy = 100;
    for (float i = sa; i <= ea - st; i += st)
    {
        float x1 = cx + (r - w / 2.0) * cos((i - 90) * DEG2RAD);
        float y1 = cy + (r - w / 2.0) * sin((i - 90) * DEG2RAD);
        float x2 = cx + (r + w / 2.0) * cos((i - 90) * DEG2RAD);
        float y2 = cy + (r + w / 2.0) * sin((i - 90) * DEG2RAD);
        float x3 = cx + (r - w / 2.0) * cos((i - 90 + st) * DEG2RAD);
        float y3 = cy + (r - w / 2.0) * sin((i - 90 + st) * DEG2RAD);
        float x4 = cx + (r + w / 2.0) * cos((i - 90 + st) * DEG2RAD);
        float y4 = cy + (r + w / 2.0) * sin((i - 90 + st) * DEG2RAD);
        display.fillTriangle(x1, y1, x2, y2, x3, y3, c);
        display.fillTriangle(x2, y2, x3, y3, x4, y4, c);
    }
}

unsigned OrbitalGSR::DaysPerMonth(unsigned y, unsigned m)
{
    if (m == 2)
    {
        if (y % 4 || (y % 100 == 0 && y % 400))
            return 28;
        return 29;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11)
        return 30;
    return 31;
}

void OrbitalGSR::drawDetails(float angle, unsigned radius, String value)
{
    unsigned cx = 100, cy = 100;
    float x = cx + radius * cos((angle - 90) * DEG2RAD);
    float y = cy + radius * sin((angle - 90) * DEG2RAD);
    x += 3 + 7 * (sin((angle + 90) * DEG2RAD) - 1);
    y += 2 + 9 * (sin((180 - angle) * DEG2RAD) - 0.5);
    display.setFont(nullptr);
    display.setCursor(x, y);
    display.print(value);
}