# Watchy TicTacToe Game Addon

A simple, minimalist, plug-and-play TicTacToe game for the Watchy with GSR firmware.

---

## Overview

This open-source TicTacToe game addon brings the classic 3×3 grid game to your Watchy in a clean, easy-to-use package. Designed for seamless integration with the GSR firmware, it requires minimal setup—just drop it into your Watchface Addons folder and update your `GSR.ino` with a single include.

---

## Requirements

- **GSR firmware**
- Arduino IDE (or PlatformIO) configured for Watchy development

---

## Installation

1. **Clone or Download**  
   ```bash
   git clone https://github.com/yourusername/watchy-tictactoe.git
   ```
2. **Copy Addon**  
   Copy the `TicTacToe` folder into your Watchy project under:  
   ```
   Watchface Addons/TicTacToe/
   ```
3. **Include in GSR.ino**  
   In your `GSR.ino` (located at the root of your Watchy sketch), add the following line **above** the `setup()` function or alongside other addon includes:  
   ```cpp
   #include "../Watchface Addons/TicTacToe/TicTacToe.h"
   ```
4. **Compile & Upload**  
   Build and flash the GSR firmware as usual. Upon reboot, select “TicTacToe” from the watchface menu.

---

## Contributing

Contributions, issues, and feature requests are welcome! Please open an issue or submit a pull request on GitHub.
