#include "../Watchface Addons/CaptnGSR/CaptnGSR.h"

// Définition des variables RTC (une seule définition)
RTC_DATA_ATTR uint8_t MotoStyle = 0;
RTC_DATA_ATTR bool CaptnGSRClass::light = true;
RTC_DATA_ATTR int CaptnGSRClass::face = 0;

// Coordonnées et paramètres météo/NTP (utilisez votre propre clé si besoin)
#define LAT "51.5074"
#define LON "-0.1278"
#define GMT_OFFSET_SEC (3600 * 0)

#define OPENWEATHERMAP_APIKEY "f058fe1cad2afe8e2ddc5d063a64cecb"
#define TEMP_UNIT "metric"
#define TEMP_LANG "en"
#define WEATHER_UPDATE_INTERVAL 30 // en minutes
#define NTP_SERVER "pool.ntp.org"

// Tableau des jours de la semaine débutant par lundi
static const char *DOW_EN[7] = {
    "Monday", "Tuesday", "Wednesday", "Thursday",
    "Friday", "Saturday", "Sunday"};

void CaptnGSRClass::drawCaptnWednesday(bool light, float batt)
{
    int16_t x1, y1;
    uint16_t w, h;
    String textstring, textstringday, textstringsteps;

    // Fond et image principale
    display.fillScreen(light ? GxEPD_BLACK : GxEPD_WHITE);
    display.drawBitmap(0, 0, captnwednesday_img, 200, 200, light ? GxEPD_WHITE : GxEPD_BLACK);

    display.setTextColor(light ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextWrap(false);

    // --- Date (jour de la semaine), semaine démarrant lundi
    display.setFont(&Tintin_Dialogue9pt7b);
    // Décalage : (0=dimanche)->6, (1=lundi)->0, (2=mardi)->1 ...
    uint8_t idx = (WatchTime.Local.Wday + 6) % 7;
    textstringday = DOW_EN[idx];
    display.setCursor(76, 66);
    display.print(textstringday);

    // --- Date (jour-mois)
    display.setFont(&Tintin_Dialogue10pt7b);
    textstring = String(WatchTime.Local.Day) + "-" + monthShortStr(WatchTime.Local.Month);
    display.setCursor(0, 194);
    display.print(textstring);

    // --- Heure
    display.setFont(&Tintin_Dialogue16pt7b);
    display.setCursor(60, 199);
    int displayHour = Is24HourMode() ? WatchTime.Local.Hour : ((WatchTime.Local.Hour + 11) % 12) + 1;
    if (displayHour < 10)
        display.print("0");
    display.print(displayHour);
    display.print(":");
    if (WatchTime.Local.Minute < 10)
        display.print("0");
    display.println(WatchTime.Local.Minute);

    // --- Batterie (segments)
    int8_t batteryLevel = 0;
    if (batt > 4.10)
        batteryLevel = 3;
    else if (batt > 3.95)
        batteryLevel = 2;
    else if (batt > 3.80)
        batteryLevel = 1;
    for (int8_t seg = 0; seg < batteryLevel; seg++)
    {
        display.fillRect(
            159,
            151 + (seg * (BATTERY_SEGMENT_SPACING - 2)),
            BATTERY_SEGMENT_HEIGHT - 3,
            BATTERY_SEGMENT_WIDTH - 2,
            light ? GxEPD_BLACK : GxEPD_WHITE);
    }

    // --- Pas
    uint32_t stepCount = CurrentStepCount();
    display.setFont(&Tintin_Dialogue8pt7b);
    display.setCursor(160, 184);
    display.print("Steps");
    display.drawFastHLine(159, 184, 40, light ? GxEPD_BLACK : GxEPD_WHITE);
    textstringsteps = String(stepCount);
    display.getTextBounds(textstringsteps, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(198 - w, 199);
    display.println(textstringsteps);
}

// Definition of the global loader instance
CaptnGSRClass CaptnGSRClassLoader;
