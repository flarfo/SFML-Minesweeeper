#include "SubWindow.h"
#pragma once

class LeaderboardWindow : public SubWindow
{
private:
    string lbPath = "../leaderboard.txt";
    vector<pair<string, int>> leaderboardTimes;
    sf::Text leaderboardText;
    string leaderboardString;
    void ReorderLeaderboard();
public:
    LeaderboardWindow(int _width, int _height, sf::Color _color, string name);
    ~LeaderboardWindow() = default;
    void WindowLoop();
    void Close(bool end = false);
    void Toggle();
    void AddToLeaderboard(pair<string, int> time);
};

LeaderboardWindow::LeaderboardWindow(int _width, int _height, sf::Color _color, std::string name) : SubWindow(_width, _height, _color, name)
{
    leaderboardText.setFont(font);
    leaderboardText.setPosition(width / 2, height / 2);
}

void LeaderboardWindow::WindowLoop()
{
    if (!open) return;

    while (window.isOpen())
    {
        if (!open) return;

        sf::Event event;
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case (sf::Event::Closed):
                    Hide();
                    break;
            }
        }

        window.clear(color);
        window.draw(leaderboardText);
        window.display();
    }
}

void LeaderboardWindow::Close(bool end)
{
    open = false;
    window.close();
}

void LeaderboardWindow::Toggle()
{
    AddToLeaderboard(pair<string, int>("t", rand()));
    open = !open;

    if (open) Show();
    else Hide();
    WindowLoop();
}

void LeaderboardWindow::AddToLeaderboard(pair<string, int> time)
{
    // don't have more than 5 values displayed
    if (leaderboardTimes.size() >= 5)
    {
        for (int i = 0; i < leaderboardTimes.size(); i++)
        {
            if (time.second < leaderboardTimes[i].second) break;
            return;
        }
    }

    leaderboardTimes.push_back(time);
    ReorderLeaderboard();
    sf::FloatRect textRect = leaderboardText.getLocalBounds();
    leaderboardText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}

void LeaderboardWindow::ReorderLeaderboard()
{
    for (int i = 0; i < leaderboardTimes.size(); i++)
    {
        int time = leaderboardTimes[i].second;
        for (int j = 0; j < leaderboardTimes.size(); j++)
        {
            int cur = leaderboardTimes[j].second;
            if (cur > time)
            {
                leaderboardTimes[i].swap(leaderboardTimes[j]);
                continue;
            }
        }
    }

    leaderboardString.clear();
    for (int i = 0; i < leaderboardTimes.size() && i < 5; i++)
    {
        leaderboardString += to_string(i + 1) + ".\t" + leaderboardTimes[i].first + "\t" + to_string(leaderboardTimes[i].second) + "\n";
    }

    leaderboardText.setString(leaderboardString);
}
