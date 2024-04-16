#include "BaseWindow.h"
#include "GameWindow.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>
#include <iostream>
#pragma once
using namespace std;

class WelcomeWindow : public BaseWindow
{
private:
    void CenterTextPos(sf::Text &text, int offsetX = 0, int offsetY = 0);
    sf::Text userText;
    sf::Text enterText;
    sf::Text welcomeText;
    int frameCount = 0;
    int mineCount;
public:
    WelcomeWindow(int _width, int _height, sf::Color _color, int _mineCount, string _name);
    ~WelcomeWindow() = default;
    void Close(bool end = false);
    void WindowLoop();
};

WelcomeWindow::WelcomeWindow(int _width, int _height, sf::Color _color, int _mineCount, string _name)  : BaseWindow(_width, _height, _color, _name)
{
    mineCount = _mineCount;
    displayName = "|";

    if (initialized)
    {
        welcomeText.setFont(font);
        welcomeText.setStyle(sf::Text::Underlined);
        welcomeText.setCharacterSize(24);
        welcomeText.setString("WELCOME TO MINESWEEPER!");
        CenterTextPos(welcomeText);
        welcomeText.setPosition((width / 2), (height / 2) - 150);

        enterText.setFont(font);
        enterText.setCharacterSize(20);
        enterText.setString("Enter your name:");
        CenterTextPos(enterText);
        enterText.setPosition((width / 2), (height / 2) - 75);

        userText.setFont(font);
        userText.setFillColor(sf::Color::Yellow);
        userText.setCharacterSize(18);
        userText.setString(displayName);
        userText.setPosition((width / 2), (height / 2) - 45);
    }
    else
    {
        cout << "Error: Font not initialized!" << endl;
        return;
    }
}

void WelcomeWindow::WindowLoop() // return a GameWindow later
{
    while (window.isOpen())
    {
        // This is the worst thing I have ever created.
        if (frameCount % 8000 == 0)
        {
            displayName[displayName.length() - 1] = ' ';
            userText.setString(displayName);
            frameCount &= 8000;
        }
        else if (frameCount % 4000 == 0)
        {
            displayName[displayName.length() - 1] = '|';
            userText.setString(displayName);
        }
        // Too bad.

        sf::Event event;
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case (sf::Event::Closed):
                    window.close();
                    break;
                case (sf::Event::TextEntered):
                    if (event.text.unicode <= 128) // ASCII
                    {
                        char cur = (char)event.text.unicode;
                        if (!isalpha(cur) || displayName.length() >= 11) break;
                        if (displayName.length() == 1) cur = toupper(cur);
                        else cur = tolower(cur);

                        displayName.insert(displayName.length() - 1, 1, cur);
                        userText.setString(displayName);
                        CenterTextPos(userText);
                    }
                    break;
                case (sf::Event::KeyPressed):
                    switch (event.key.code)
                    {
                        case (sf::Keyboard::Key::BackSpace):
                            if (displayName.length() < 2) break;
                            displayName.erase(displayName.length() - 2, 1);
                            userText.setString(displayName);
                            CenterTextPos(userText);
                            break;
                        case (sf::Keyboard::Key::Enter):
                            if (displayName.length() == 1) continue;
                            Close();
                            break;
                    }

                    break;
            }
        }

        window.clear(color);
        window.draw(welcomeText);
        window.draw(enterText);
        window.draw(userText);
        window.display();
        frameCount++;
    }
}

void WelcomeWindow::Close(bool end)
{
    window.close();
    if (end) return;

    GameWindow _window(width, height, color, mineCount, "Game");
    displayName.pop_back();
}

void WelcomeWindow::CenterTextPos(sf::Text &text, int offsetX, int offsetY)
{
    sf::FloatRect textRect = text.getLocalBounds();
    // horizontally center text (y = 0 so height doesn't change when a shorter/taller character is added)
    text.setOrigin(textRect.left + textRect.width / 2.0f + offsetX,offsetY);
}