#include <string>
#include <SFML/Graphics.hpp>
#include "Minesweeper.h"
#pragma once
using namespace std;

class SubWindow
{
protected:
    static sf::Font font;
    static bool initialized;
    static string displayName;
    static sf::RenderWindow window;
    sf::Color color;
    int height;
    int width;
    string name;
    bool open = false;
public:
    SubWindow(int _width, int _height, sf::Color _color, string _name);
    ~SubWindow() = default;
    virtual void Close(bool end) = 0;
    virtual void WindowLoop() = 0;
    void Hide();
    void Show();
};

// static member declarations
sf::Font SubWindow::font;
bool SubWindow::initialized;
string SubWindow::displayName;
sf::RenderWindow SubWindow::window;

SubWindow::SubWindow(int _width, int _height, sf::Color _color, string _name = "Default")
{
    color = _color;
    width = _width;
    height = _height;
    name = _name;

    if (!initialized)
    {
        initialized = font.loadFromFile("../source files/font.ttf");
    }

    window.create(sf::VideoMode(width, height), name, sf::Style::Close);
    window.setVisible(false);
    window.clear(color);
}

void SubWindow::Hide()
{
    open = false;
    window.setVisible(false);
}

void SubWindow::Show()
{
    open = true;
    window.setVisible(true);
}
