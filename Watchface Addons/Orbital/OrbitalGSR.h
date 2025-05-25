#ifndef ORBITAL_GSR_H
#define ORBITAL_GSR_H

#include <Watchy_GSR.h>
#include "SunSet.h"

#define DEG2RAD 0.0174532925

// ── Réglages partagés ───────────────────────────────────────────────────────────
extern float lat;
extern float lon;
extern const char *daysOfWeek[];
extern const String shortMonths[];
extern int batteryVMax;
extern int batteryVMin;

/**
 * OrbitalGSR
 * AddOn WatchyGSR pour le cadran "Orbital"
 */
class OrbitalGSR : public WatchyGSR
{
    using WatchyGSR::WatchyGSR;

public:
    OrbitalGSR();
    void RegisterWatchFaces() override;
    void InsertInitWatchStyle(uint8_t StyleID) override;
    void InsertDrawWatchStyle(uint8_t StyleID) override;

private:
    SunSet sun;
    uint16_t primaryColor, secondaryColor;
    bool details, twentyFour;

    void drawWatchFace();
    void drawWatchMinute();
    void drawWatchHour();
    void drawWatchDay();
    void drawWatchMonth();
    void drawBattery();
    void drawMoon();
    void drawDayTime();
    void drawWeather();
    void drawInternalTemp();

    void fillArc2(float sa, float ea, unsigned r, unsigned w, unsigned c, float st);
    void fillArc3(float sa, float ea, unsigned r, unsigned w, unsigned c, float st);
    unsigned DaysPerMonth(unsigned y, unsigned m);
    void drawDetails(float angle, unsigned radius, String value);
};

extern OrbitalGSR OrbitalGSRClassLoader;

#endif // ORBITAL_GSR_H