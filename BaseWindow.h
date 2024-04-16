#include <string>
#include <SFML/Graphics.hpp>
#include "Minesweeper.h"
#pragma once
using namespace std;

class BaseWindow
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
public:
    BaseWindow(int _width, int _height, sf::Color _color, string _name);
    ~BaseWindow() = default;
    virtual void Close(bool end) = 0;
    virtual void WindowLoop() = 0;
};

// static member declarations
sf::Font BaseWindow::font;
bool BaseWindow::initialized;
string BaseWindow::displayName;
sf::RenderWindow BaseWindow::window;

BaseWindow::BaseWindow(int _width, int _height, sf::Color _color, string _name = "Default")
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
    window.clear(color);
}
