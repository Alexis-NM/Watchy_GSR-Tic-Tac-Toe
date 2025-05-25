# Watchy GSR Add-ons

This monorepo centralizes multiple plug-and-play watchface add-ons for the Watchy running the GSR firmware. Simply pick the add-ons you want, include them in your sketch, and build!

---

## Table of Contents

1. [Prerequisites](#prerequisites)  
2. [Add-ons Included](#add-ons-included)  
3. [Installation](#installation)  
4. [Usage](#usage)  
5. [Contributing](#contributing)  
6. [License](#license)  

---

## Prerequisites

- Watchy development environment (PlatformIO or Arduino IDE) configured for GSR  
- A Watchy device running GSR firmware  

---

## Add-ons Included

- **Album Cover Watchface** (`AlbumCoverWatchface`)  
- **TicTacToe Game** (`TicTacToe`) – *WIP, high battery drain*  
- **RedubGSR Watchface** (`RedubGSR`)  
- **OrbitalGSR Watchface** (`OrbitalGSR`)  
- **CaptnGSR Watchface** (`CaptnGSR`)  
- **StationaryGSR Watchface** (`Stationary`)  

---

## Installation

1. **Clone the monorepo**  
   ```bash
   git clone https://github.com/yourusername/Watchy_GSR-Add-ons.git
   cd Watchy_GSR-Add-ons
   ```

2. **Copy desired add-ons**  
   Copy the folder(s) for the add-on(s) you want into your Watchy sketch directory, e.g.:  
   ```
   WatchyProject/
   ├─ src/
   │  ├─ GSR.ino
   │  └─ Watchface Addons/
   │     ├─ AlbumCoverWatchface/
   │     ├─ RedubGSR/
   │     ├─ OrbitalGSR/
   │     ├─ CaptnGSR/
   │     ├─ Stationary/
   │     └─ TicTacToe/     ← (optional, WIP)
   ```

3. **Include in `GSR.ino`**  
   In your `GSR.ino`, add `#include` lines for each add-on above `setup()`. For example:
   ```cpp
   #include "Watchface Addons/AlbumCoverWatchface/AlbumCoverWatchface.h"
   #include "Watchface Addons/RedubGSR/RedubGSR.h"
   #include "Watchface Addons/OrbitalGSR/OrbitalGSR.h"
   #include "Watchface Addons/CaptnGSR/CaptnGSR.h"
   #include "Watchface Addons/Stationary/stationaryGSR.h"
   // #include "Watchface Addons/TicTacToe/TicTacToe.h"  // WIP: currently high battery drain
   ```
   _Uncomment the TicTacToe include at your own risk – it’s still under heavy optimization._

4. **Build & Flash**  
   ```bash
   pio run --target upload
   ```  
   or use the Arduino IDE’s upload button.

---

## Usage

- After flashing, reboot your Watchy.  
- In the watchface selection menu, you’ll see entries for each included add-on:
  - **AlbumCover** – cycles through BMP album covers.  
  - **RedubGSR** – emulates a classic Casio digital watch style with segmented digits.  
  - **OrbitalGSR** – displays time, date, weather, and battery level using minimalist circles.  
  - **CaptnGSR** – a Captain Haddock meme-inspired watchface with fun graphic overlays.
  - **Stationary** – Pebble-style stationary text layout adapted for Watchy.  
  - **TicTacToe** – (WIP) launch the morpion game; expect higher battery usage.

---

## Contributing

Contributions welcome!  
- Open an issue to report bugs or request features.  
- Submit a pull request with your enhancements or fixes.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.  
