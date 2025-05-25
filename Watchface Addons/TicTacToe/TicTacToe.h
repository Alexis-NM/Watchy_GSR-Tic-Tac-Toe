// STILL WORK IN PROGRESS (battery drain pretty high, needs optimization)

#ifndef TICTACTOEGAME_H
#define TICTACTOEGAME_H

#include "Watchy_GSR.h"
#include <Arduino.h>

RTC_DATA_ATTR uint8_t TicTacToeGame;
RTC_DATA_ATTR int8_t board[9];
RTC_DATA_ATTR uint8_t cursorPos;
RTC_DATA_ATTR int8_t gameState;
RTC_DATA_ATTR int8_t winner;
RTC_DATA_ATTR uint8_t playerScore;
RTC_DATA_ATTR uint8_t aiScore;
RTC_DATA_ATTR uint8_t roundCount;
RTC_DATA_ATTR bool showModal;
RTC_DATA_ATTR uint8_t modalSel;
RTC_DATA_ATTR uint32_t resetDisplayUntil;

class TicTacToeGameClass : public WatchyGSR
{
public:
    TicTacToeGameClass() : WatchyGSR()
    {
        initAddOn(this);
        showModal = false;
        resetDisplayUntil = 0;
    }

    void RegisterWatchFaces() override
    {
        TicTacToeGame = AddWatchStyle("Tic-Tac-Toe", this, true);
    }

    void InsertInitWatchStyle(uint8_t StyleID) override
    {
        if (StyleID == TicTacToeGame)
            resetGame();
    }

    void InsertDrawWatchStyle(uint8_t StyleID) override
    {
        if (StyleID == TicTacToeGame)
            drawBoard();
    }

    bool InsertNeedAwake(bool) override { return CurrentGameID() == TicTacToeGame; }

    bool InsertHandlePressed(uint8_t btn, bool &Haptic, bool &Refresh) override
    {
        if (CurrentGameID() != TicTacToeGame)
            return false;
        Haptic = Refresh = true;
        uint32_t now = millis();

        if (showModal)
        {
            switch (btn)
            {
            case 1:
                if (modalSel == 0)
                {
                    playerScore = aiScore = roundCount = 0;
                    resetGame();
                    GameStatus(false);
                    HideGame();
                }
                else
                {
                    playerScore = aiScore = roundCount = 0;
                    resetGame();
                    resetDisplayUntil = now + 1000;
                }
                showModal = false;
                break;
            case 2:
                showModal = false;
                break;
            case 4:
            case 3:
                modalSel = (modalSel + 1) % 2;
                break;
            }
            UpdateScreen();
            return true;
        }

        switch (btn)
        {
        case 1:
            placeMark();
            UpdateScreen();
            return true;
        case 2:
            showModal = true;
            modalSel = 0;
            UpdateScreen();
            return true;
        case 3:
            moveRight();
            UpdateScreen();
            return true;
        case 4:
            moveDown();
            UpdateScreen();
            return true;
        }
        return false;
    }

private:
    void resetGame()
    {
        for (uint8_t i = 0; i < 9; i++)
            board[i] = -1;
        cursorPos = 4;
        gameState = 0;
        winner = -1;
    }

    void drawBoard()
    {
        display.fillScreen(BackColor());
        const uint8_t cell = 40, grid = cell * 3;
        int16_t w = display.width(), h = display.height();
        int16_t x0 = (w - grid) / 2, y0 = (h - grid) / 2;
        int16_t tx, ty;
        uint16_t tw, th;

        String score = "You:" + String(playerScore) + " AI:" + String(aiScore);
        display.getTextBounds(score, 0, 0, &tx, &ty, &tw, &th);
        display.setCursor((w - tw) / 2, th + 5);
        display.println(score);

        if (showModal)
        {
            int16_t mx = (w - 200) / 2;
            int16_t my = (h - 100) / 2;
            display.fillRect(mx, my, 200, 100, BackColor());
            display.drawRect(mx, my, 200, 100, ForeColor());

            String q = "Quit or Reset?";
            display.getTextBounds(q, 0, 0, &tx, &ty, &tw, &th);
            display.setCursor(mx + (200 - tw) / 2, my + th + 5);
            display.println(q);

            String optQuit = "Quit";
            display.getTextBounds(optQuit, 0, 0, &tx, &ty, &tw, &th);
            uint16_t twQuit = tw;
            display.getTextBounds("Reset", 0, 0, &tx, &ty, &tw, &th);
            uint16_t twReset = tw;
            int16_t S = (200 - twQuit - twReset) / 2;
            int16_t margin = S / 2;
            int16_t qx = mx + margin;
            int16_t rx = qx + twQuit + S;
            int16_t qy = my + 100 - th - 5;
            display.setCursor(qx, qy);
            display.println(optQuit);
            if (modalSel == 0)
                for (int i = 0; i < 3; i++)
                    display.drawLine(qx, qy + th + 2 + i, qx + twQuit, qy + th + 2 + i, ForeColor());

            display.setCursor(rx, qy);
            display.println("Reset");
            if (modalSel == 1)
                for (int i = 0; i < 3; i++)
                    display.drawLine(rx, qy + th + 2 + i, rx + twReset, qy + th + 2 + i, ForeColor());
            return;
        }

        {
            for (uint8_t i = 1; i < 3; i++)
            {
                display.drawLine(x0 + cell * i, y0, x0 + cell * i, y0 + grid, ForeColor());
                display.drawLine(x0, y0 + cell * i, x0 + grid, y0 + cell * i, ForeColor());
            }
        }

        for (uint8_t i = 0; i < 9; i++)
        {
            int8_t v = board[i];
            uint8_t cx = x0 + (i % 3) * cell + cell / 2;
            uint8_t cy = y0 + (i / 3) * cell + cell / 2;
            if (v == 1)
                drawX(cx, cy, cell / 2 - 5);
            else if (v == 0)
                display.drawCircle(cx, cy, cell / 2 - 5, ForeColor());
        }

        display.drawRect(x0 + (cursorPos % 3) * cell,
                         y0 + (cursorPos / 3) * cell,
                         cell, cell, ForeColor());

        uint32_t now = millis();
        if (resetDisplayUntil > now)
        {
            String m = "Reset!";
            display.getTextBounds(m, 0, 0, &tx, &ty, &tw, &th);
            display.setCursor((w - tw) / 2, h - th - 5);
            display.println(m);
            return;
        }

        if (gameState == 1)
        {
            String m = winner == 1 ? "You win!" : winner == 0 ? "AI wins!"
                                                              : "Draw!";
            display.getTextBounds(m, 0, 0, &tx, &ty, &tw, &th);
            display.setCursor((w - tw) / 2, h - th - 5);
            display.println(m);
        }
    }

    void drawX(uint8_t cx, uint8_t cy, uint8_t r)
    {
        display.drawLine(cx - r, cy - r, cx + r, cy + r, ForeColor());
        display.drawLine(cx - r, cy + r, cx + r, cy - r, ForeColor());
    }

    void moveRight()
    {
        uint8_t r = cursorPos / 3, c = cursorPos % 3;
        cursorPos = r * 3 + ((c + 1) % 3);
    }
    void moveDown()
    {
        uint8_t r = cursorPos / 3, c = cursorPos % 3;
        cursorPos = ((r + 1) % 3) * 3 + c;
    }

    void placeMark()
    {
        if (gameState == 1)
        {
            resetGame();
            return;
        }
        if (board[cursorPos] != -1)
            return;
        board[cursorPos] = 1;
        if (checkWin(1))
        {
            finishGame(1);
            return;
        }
        if (isDraw())
        {
            finishGame(2);
            return;
        }
        aiMove();
    }

    void aiMove()
    {
        int8_t mv = findWinningMove(0);
        if (mv < 0)
            mv = findWinningMove(1);
        if (mv < 0 && roundCount >= 3)
            mv = findBestMove();
        if (mv < 0)
        {
            uint8_t e[9], n = 0;
            for (uint8_t i = 0; i < 9; i++)
                if (board[i] == -1)
                    e[n++] = i;
            if (n)
                mv = e[random(n)];
        }
        if (mv < 0)
        {
            finishGame(2);
            return;
        }
        board[mv] = 0;
        if (checkWin(0))
        {
            finishGame(0);
            return;
        }
        if (isDraw())
        {
            finishGame(2);
            return;
        }
    }

    int8_t findWinningMove(int8_t p)
    {
        for (uint8_t i = 0; i < 9; i++)
            if (board[i] == -1)
            {
                board[i] = p;
                bool w = checkWin(p);
                board[i] = -1;
                if (w)
                    return i;
            }
        return -1;
    }
    int findBestMove()
    {
        int best = -1000, bm = -1;
        for (uint8_t i = 0; i < 9; i++)
            if (board[i] == -1)
            {
                board[i] = 0;
                int v = minimax(0, false);
                board[i] = -1;
                if (v > best)
                {
                    best = v;
                    bm = i;
                }
            }
        return bm;
    }
    int minimax(int d, bool mx)
    {
        if (checkWin(0))
            return 10 - d;
        if (checkWin(1))
            return d - 10;
        if (isDraw())
            return 0;
        int best = mx ? -1000 : 1000;
        for (uint8_t i = 0; i < 9; i++)
            if (board[i] == -1)
            {
                board[i] = mx ? 0 : 1;
                int v = minimax(d + 1, !mx);
                board[i] = -1;
                best = mx ? max(best, v) : min(best, v);
            }
        return best;
    }
    bool checkWin(int8_t p)
    {
        static const uint8_t w[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
        for (auto &c : w)
            if (board[c[0]] == p && board[c[1]] == p && board[c[2]] == p)
                return true;
        return false;
    }
    bool isDraw()
    {
        for (int i = 0; i < 9; i++)
            if (board[i] == -1)
                return false;
        return true;
    }
    void finishGame(int8_t who)
    {
        gameState = 1;
        winner = who;
        roundCount++;
        if (who == 1)
            playerScore++;
        else if (who == 0)
            aiScore++;
    }
};

TicTacToeGameClass TicTacToeGameClassLoader;

#endif