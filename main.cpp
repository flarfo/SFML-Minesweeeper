#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "WelcomeWindow.h"
#include "Minesweeper.h"
using namespace std;

int main() {
    ifstream cfgFile("../source files/config.cfg");
    string line;

    getline(cfgFile, line);
    int columns = stoi(line);

    getline(cfgFile, line);
    int rows = stoi(line);

    getline(cfgFile, line);
    int mines = stoi(line);

    cfgFile.close();

    ifstream leaderboardFile("../leaderboard.txt");
    if (!leaderboardFile.is_open())
    {
        ofstream lb("../leaderboard.txt");
        lb.close();
    }

    WelcomeWindow window(columns * 32, (rows * 32) + 100, sf::Color::Blue, mines, "Welcome");
    window.WindowLoop();

    return 0;
}