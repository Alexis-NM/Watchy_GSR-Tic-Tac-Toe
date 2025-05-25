#ifndef WATCHY_MOTOGSR_H
#define WATCHY_MOTOGSR_H

#include <Watchy_GSR.h>
#include "captnwednesday_img.h"
#include "Tintin_Dialogue8pt7b.h"
#include "Tintin_Dialogue9pt7b.h"
#include "Tintin_Dialogue10pt7b.h"
#include "Tintin_Dialogue16pt7b.h"

// Dimensions pour l'indicateur de batterie
static const uint8_t BATTERY_SEGMENT_WIDTH = 7;
static const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
static const uint8_t BATTERY_SEGMENT_SPACING = 9;

// Identifiant du style WatchFace (déclaré extern pour éviter redéfinition)
extern RTC_DATA_ATTR uint8_t MotoStyle;

class CaptnGSRClass : public WatchyGSR
{
public:
    CaptnGSRClass() : WatchyGSR() { initAddOn(this); }

    // Enregistrer le style au démarrage
    void RegisterWatchFaces() override
    {
        MotoStyle = AddWatchStyle("Captn", this);
    }

    // Initialisation du style (configurations spécifiques)
    void InsertInitWatchStyle(uint8_t StyleID) override
    {
        if (StyleID == MotoStyle)
        {
            WantWeather(true);
        }
    }

    // Dessin de la WatchFace
    void InsertDrawWatchStyle(uint8_t StyleID) override
    {
        if (StyleID == MotoStyle && SafeToDraw())
        {
            // Passe le voltage batterie à la fonction de dessin
            drawCaptnWednesday(IsLightMode(), getBatteryVoltage());
        }
    }

protected:
    void drawCaptnWednesday(bool light, float batt);

private:
    // Variables internes persistantes pour alternance clair/sombre si besoin
    RTC_DATA_ATTR static bool light;
    RTC_DATA_ATTR static int face;
};

extern CaptnGSRClass CaptnGSRClassLoader;

#endif // WATCHY_MOTOGSR_H
