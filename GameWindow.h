#include "BaseWindow.h"
#include "LeaderboardWindow.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <chrono>
#pragma once
using namespace std;

class GameWindow : public BaseWindow
{
private:
    static Minesweeper* minesweeper;
    LeaderboardWindow* leaderboard;
    sf::Texture happyTex;
    sf::Texture winTex;
    sf::Texture loseTex;
    sf::Texture bombViewTex;
    sf::Texture pauseTex;
    sf::Texture playTex;
    sf::Texture leaderboardTex;
    sf::Texture digitsTex;
    sf::Sprite faceSprite;
    sf::Sprite bombViewSprite;
    sf::Sprite pauseSprite;
    sf::Sprite leaderboardSprite;
    sf::Sprite digits1Sprite;
    sf::Sprite digits10Sprite;
    sf::Sprite digits100Sprite;
    bool lmbClicked = false;
    bool rmbClicked = false;
    chrono::high_resolution_clock clock;
    chrono::time_point<chrono::high_resolution_clock> startTime;
    chrono::time_point<chrono::high_resolution_clock> prevTime;
    float elapsedTime = 0;
    int ielapsedTime = 0;
    bool paused = false;
public:
    GameWindow(int _width, int _height, sf::Color _color, int _mineCount, string name);
    ~GameWindow() = default;
    void WindowLoop();
    void Close(bool end = false);
    void initGame(int columns, int rows, int mineCount);
    sf::Texture GetDigitTex(int digit);
};

Minesweeper* GameWindow::minesweeper;


GameWindow::GameWindow(int _width, int _height, sf::Color _color, int _mineCount, string name)  : BaseWindow(_width, _height, _color, name)
{
    leaderboard = new LeaderboardWindow(width / 2, height / 2, sf::Color::Blue, "Leaderboard");
    initGame(width / 32, (height - 100) / 32, _mineCount);

    happyTex.loadFromFile("../source files/images/face_happy.png");
    winTex.loadFromFile("../source files/images/face_win.png");
    loseTex.loadFromFile("../source files/images/face_lose.png");
    bombViewTex.loadFromFile("../source files/images/debug.png");
    pauseTex.loadFromFile("../source files/images/pause.png");
    playTex.loadFromFile("../source files/images/play.png");
    leaderboardTex.loadFromFile("../source files/images/leaderboard.png");
    digitsTex.loadFromFile("../source files/images/digits.png");

    sf::Vector2<float> center(width / 2.0, height / 2.0);

    faceSprite.setOrigin(32, 32);
    faceSprite.setPosition(center.x, center.y + 255);
    faceSprite.setTexture(happyTex);

    bombViewSprite.setOrigin(32, 32);
    bombViewSprite.setPosition(center.x + 128, center.y + 255);
    bombViewSprite.setTexture(bombViewTex);

    pauseSprite.setOrigin(32, 32);
    pauseSprite.setPosition(center.x + 192, center.y + 255);
    pauseSprite.setTexture(pauseTex);

    leaderboardSprite.setOrigin(32, 32);
    leaderboardSprite.setPosition(center.x + 256, center.y + 255);
    leaderboardSprite.setTexture(leaderboardTex);

    sf::IntRect rect(0, 0, 21, 32);
    int digitStartX = 352;
    digits1Sprite.setOrigin(10.5, 16);
    digits1Sprite.setPosition(center.x - digitStartX + 42, center.y + 255);
    digits1Sprite.setTexture(digitsTex);
    digits1Sprite.setTextureRect(rect);

    digits10Sprite.setOrigin(10.5, 16);
    digits10Sprite.setPosition(center.x - digitStartX + 21, center.y + 255);
    digits10Sprite.setTexture(digitsTex);
    digits10Sprite.setTextureRect(rect);

    digits100Sprite.setOrigin(10.5, 16);
    digits100Sprite.setPosition(center.x - digitStartX, center.y + 255);
    digits100Sprite.setTexture(digitsTex);
    digits100Sprite.setTextureRect(rect);

    GameWindow::WindowLoop();
}

void GameWindow::WindowLoop()
{
    while (window.isOpen())
    {
        if (!paused && !minesweeper->Ended())
        {
            if ((int)elapsedTime > ielapsedTime && elapsedTime <= 999)
            {
                int ones = (int)elapsedTime % 10;
                int tens = ((int)elapsedTime / 10) % 10;
                int hundreds = ((int)elapsedTime / 100) % 10;

                digits1Sprite.setTextureRect(sf::IntRect(ones * 21, 0, 21, 32));
                digits10Sprite.setTextureRect(sf::IntRect(tens * 21, 0, 21, 32));
                digits100Sprite.setTextureRect(sf::IntRect(hundreds * 21, 0, 21, 32));
            }

            if (minesweeper->Started())
            {
                ielapsedTime = elapsedTime;
                elapsedTime += (chrono::duration_cast<chrono::milliseconds>((clock.now() - prevTime)).count()) / 1000.0;
                prevTime = clock.now();
            }
        }

        sf::Event event;
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case (sf::Event::Closed):
                    window.close();
                    break;
            }
        }

        if  (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lmbClicked)
        {
            lmbClicked = true;
        }
        else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && lmbClicked)
        {
            sf::Vector2<int> mousePos = sf::Mouse::getPosition(window);

            if (!minesweeper->Ended())
            {
                if (!minesweeper->Started())
                {
                    elapsedTime = 0;
                    ielapsedTime = 0;
                    startTime = clock.now();
                    prevTime = startTime;
                }

                if (!paused)
                {
                    bool lost = minesweeper->Click(mousePos); // click on release
                    if (lost)
                    {
                        faceSprite.setTexture(loseTex);
                    }

                    bool won = minesweeper->CheckWin();
                    if (won)
                    {
                        faceSprite.setTexture(winTex);
                    }
                }
            }

            if (faceSprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                faceSprite.setTexture(happyTex);
                minesweeper->Reset();

                sf::IntRect rect(0, 0, 21, 32);
                digits1Sprite.setTextureRect(rect);
                digits10Sprite.setTextureRect(rect);
                digits100Sprite.setTextureRect(rect);

                pauseSprite.setTexture(pauseTex);
                paused = false;
            }
            else if (bombViewSprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                minesweeper->Debug();
            }
            else if (pauseSprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                paused = !paused;

                if (paused)
                {
                    pauseSprite.setTexture(playTex);
                }
                else
                {
                    pauseSprite.setTexture(pauseTex);
                    prevTime = clock.now();
                }
            }
            else if (leaderboardSprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                leaderboard->Toggle();
            }

            lmbClicked = false;
        }

        if  (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rmbClicked)
        {
            rmbClicked = true;
        }
        else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && rmbClicked && !paused)
        {
            if (!minesweeper->Ended())
            {
                minesweeper->Flag(sf::Mouse::getPosition(window)); // click on release
            }

            rmbClicked = false;
        }

        window.clear(sf::Color::White);

        sf::Vector2<int> size = minesweeper->getSize();
        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                Tile* tile = minesweeper->tiles[y][x];
                window.draw(tile->getTileSprite());

                if ((tile->isRevealed() && (tile->getNeighbors() > 0 || tile->isMine()) || tile->isFlagged()))
                {
                    window.draw(tile->getOverlaySprite());
                }
            }
        }

        window.draw(faceSprite);
        window.draw(bombViewSprite);
        window.draw(pauseSprite);
        window.draw(leaderboardSprite);
        window.draw(digits1Sprite);
        window.draw(digits10Sprite);
        window.draw(digits100Sprite);

        window.display();
    }
}

void GameWindow::Close(bool end)
{
    window.close();
    if (end) return;
}

void GameWindow::initGame(int columns, int rows, int mines)
{
    //delete minesweeper;
    minesweeper = new Minesweeper(columns, rows, mines);
}