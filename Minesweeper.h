#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/System/Vector2.hpp>
#pragma once
using namespace std;

class Tile
{
private:
    static sf::Texture tileTex;
    static sf::Texture tileRevealedTex;
    static sf::Texture tile1Tex;
    static sf::Texture tile2Tex;
    static sf::Texture tile3Tex;
    static sf::Texture tile4Tex;
    static sf::Texture tile5Tex;
    static sf::Texture tile6Tex;
    static sf::Texture tile7Tex;
    static sf::Texture tile8Tex;
    static sf::Texture mineTex;
    static sf::Texture flagTex;
    static bool initialized;
    bool mine = false;
    bool flagged = false;
    bool revealed = false;
    int neighbors = 0;
    sf::Sprite tileSprite;
    sf::Sprite overlaySprite;
    sf::Vector2<int> position;
public:
    Tile(sf::Vector2<int> pos);
    void reset();
    bool isMine() { return mine; }
    void setMine() { mine = true; }
    bool isFlagged() { return flagged; }
    bool isRevealed() { return revealed; }
    void flag();
    void setFlagTexture() { overlaySprite.setTexture(flagTex); }
    void reveal(int _neighbors);
    void hide();
    int getNeighbors() { return neighbors; }
    sf::Vector2<int> getPos() { return position; }
    sf::Sprite getTileSprite() { return tileSprite; }
    sf::Sprite getOverlaySprite() { return overlaySprite; }
};

sf::Texture Tile::tileTex;
sf::Texture Tile::tileRevealedTex;
sf::Texture Tile::tile1Tex;
sf::Texture Tile::tile2Tex;
sf::Texture Tile::tile3Tex;
sf::Texture Tile::tile4Tex;
sf::Texture Tile::tile5Tex;
sf::Texture Tile::tile6Tex;
sf::Texture Tile::tile7Tex;
sf::Texture Tile::tile8Tex;
sf::Texture Tile::mineTex;
sf::Texture Tile::flagTex;
bool Tile::initialized = false;

Tile::Tile(sf::Vector2<int> pos)
{
    position.x = pos.x;
    position.y = pos.y;

    // Load textures
    if (!initialized)
    {
        tileTex.loadFromFile("../source files/images/tile_hidden.png");
        tileRevealedTex.loadFromFile("../source files/images/tile_revealed.png");
        tile1Tex.loadFromFile("../source files/images/number_1.png");
        tile2Tex.loadFromFile("../source files/images/number_2.png");
        tile3Tex.loadFromFile("../source files/images/number_3.png");
        tile4Tex.loadFromFile("../source files/images/number_4.png");
        tile5Tex.loadFromFile("../source files/images/number_5.png");
        tile6Tex.loadFromFile("../source files/images/number_6.png");
        tile7Tex.loadFromFile("../source files/images/number_7.png");
        tile8Tex.loadFromFile("../source files/images/number_8.png");
        mineTex.loadFromFile("../source files/images/mine.png");
        flagTex.loadFromFile("../source files/images/flag.png");
        initialized = true;
    }

    tileSprite.setPosition(position.x * 32, position.y * 32);
    tileSprite.setTexture(tileTex);
    overlaySprite.setPosition(position.x * 32, position.y * 32);
}

void Tile::reveal(int _neighbors)
{
    revealed = true;
    neighbors = _neighbors;

    // update sprite
    tileSprite.setTexture(tileRevealedTex);

    if (isMine())
    {
        overlaySprite.setTexture(mineTex);
        return;
    }

    switch (neighbors)
    {
        case 1:
            overlaySprite.setTexture(tile1Tex);
            break;
        case 2:
            overlaySprite.setTexture(tile2Tex);
            break;
        case 3:
            overlaySprite.setTexture(tile3Tex);
            break;
        case 4:
            overlaySprite.setTexture(tile4Tex);
            break;
        case 5:
            overlaySprite.setTexture(tile5Tex);
            break;
        case 6:
            overlaySprite.setTexture(tile6Tex);
            break;
        case 7:
            overlaySprite.setTexture(tile7Tex);
            break;
        case 8:
            overlaySprite.setTexture(tile8Tex);
            break;
    }
}

void Tile::hide()
{
    revealed = false;

    tileSprite.setTexture(tileTex);

    if (isFlagged())
    {
        setFlagTexture();
        return;
    }

    overlaySprite.setTexture(tileTex);
}

void Tile::flag()
{
    if (revealed) return;

    flagged = !flagged;

    if (flagged)
    {
        overlaySprite.setTexture(flagTex);
    }
}

void Tile::reset()
{
    revealed = false;
    mine = false;
    flagged = false;
    neighbors = 0;

    tileSprite.setTexture(tileTex);
}

class Minesweeper
{
private:
    bool minesGenerated = false;
    bool ended = false;
    bool started = false;
    bool debugging = false;
    sf::Vector2<int> size;
    int mineCount;
    int Random(int max = 1);
public:
    vector<vector<Tile*>> tiles;
    Minesweeper() = default;
    Minesweeper(int _columns, int _rows, int _mineCount);
    ~Minesweeper() = default;
    void GenerateMines(sf::Vector2<int> clickedPos);
    void Reset();
    sf::Vector2<int> getSize() { return size; }
    int getMineCount() { return mineCount; }
    bool InBounds(sf::Vector2<int> pos);
    bool Click(sf::Vector2<int> pos);
    void Flag(sf::Vector2<int> pos);
    bool Reveal(sf::Vector2<int> pos, bool isClick);
    bool ClearTiles(sf::Vector2<int> pos, bool isClick);
    int GetNeighbors(sf::Vector2<int> pos);
    bool CheckWin();
    bool Ended() { return ended; }
    void Debug();
    bool Started() { return started; }
};

Minesweeper::Minesweeper(int _columns, int _rows, int _mineCount)
{
    size.x = _columns;
    size.y = _rows;
    mineCount = _mineCount;

    for (int y = 0; y < size.y; y++)
    {
        vector<Tile*> vector;
        tiles.emplace_back(vector);
        for (int x = 0; x < size.x; x++)
        {
            sf::Vector2<int> pos(x, y);
            Tile* tile = new Tile(pos);
            tiles[y].push_back(tile);
        }
    }
}

void Minesweeper::Reset()
{
    started = false;
    ended = false;

    for (int y = 0; y < tiles.size(); y++)
    {
        for (int x = 0; x < tiles[y].size(); x++)
        {
            tiles[y][x]->reset();
        }
    }
}

void Minesweeper::Debug()
{
    debugging = !debugging;

    for (int y = 0; y < tiles.size(); y++)
    {
        for (int x = 0; x < tiles[y].size(); x++)
        {
            Tile* tile = tiles[y][x];

            if (tile->isMine())
            {
                if (debugging)
                {
                    tile->reveal(0);
                }
                else
                {
                    tile->hide();
                }
            }
        }
    }
}

void Minesweeper::GenerateMines(sf::Vector2<int> clickedPos)
{
    int numMines = mineCount;
    while (numMines > 0) {
        int x = Random(size.x);
        int y = Random(size.y);

        if (x == clickedPos.x && y == clickedPos.y) continue; // don't allow first click to be a mine

        Tile* tile = tiles[y][x];
        if (!tile->isMine())
        {
            tile->setMine();
            numMines--;
        }
    }
}

bool Minesweeper::Click(sf::Vector2<int> pos)
{
    sf::Vector2<int> gridPos(pos.x / 32, pos.y / 32);

    if (!InBounds(gridPos)) return false;

    if (!started)
    {
        GenerateMines(gridPos);
    }

    started = true;

    ended = ClearTiles(gridPos, true); // true if lost
    return ended;
}

void Minesweeper::Flag(sf::Vector2<int> pos)
{
    // ERROR: CANT UNFLAG MINES

    sf::Vector2<int> gPos(pos.x / 32, pos.y / 32);
    if (!InBounds(gPos)) return;
    tiles[gPos.y][gPos.x]->flag();
}

bool Minesweeper::CheckWin()
{
    for (int i = 0; i < tiles.size(); i++)
    {
        for (int j = 0; j < tiles[i].size(); j++)
        {
            if (tiles[i][j]->isMine() || tiles[i][j]->isRevealed())
            {
                continue;
            }

            return false;
        }
    }

    ended = true;
    return true;
}

bool Minesweeper::ClearTiles(sf::Vector2<int> pos, bool isClick)
{
    if (!InBounds(pos)) return false;

    Tile* tile = tiles[pos.y][pos.x];

    if (tile->isRevealed() || tile->isFlagged()) return false;

    if (Reveal(pos, isClick))
    {
        return true;
    }

    if (tile->isMine()) return false;

    if (tile->getNeighbors() == 0)
    {
        ClearTiles(sf::Vector2<int>(pos.x - 1, pos.y - 1), false);
        ClearTiles(sf::Vector2<int>(pos.x - 1, pos.y), false);
        ClearTiles(sf::Vector2<int>(pos.x - 1, pos.y + 1), false);

        ClearTiles(sf::Vector2<int>(pos.x, pos.y - 1), false);
        ClearTiles(sf::Vector2<int>(pos.x, pos.y + 1), false);

        ClearTiles(sf::Vector2<int>(pos.x + 1, pos.y - 1), false);
        ClearTiles(sf::Vector2<int>(pos.x + 1, pos.y), false);
        ClearTiles(sf::Vector2<int>(pos.x + 1, pos.y + 1), false);
    }

    return false;
}

bool Minesweeper::Reveal(sf::Vector2<int> pos, bool isClick)
{
    Tile* tile = tiles[pos.y][pos.x];
    if (tile->isFlagged() || tile->isRevealed()) return false;

    if (tile->isMine())
    {
        if (isClick)
        {
            return true;
        }

        return false;
    }

    tile->reveal(GetNeighbors(pos));
    return false;
}

int Minesweeper::GetNeighbors(sf::Vector2<int> pos)
{
    int neighbors = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0) continue; // if on clicked block
            sf::Vector2<int> newPos(pos.x + i, pos.y + j);
            if (!InBounds(newPos)) continue;

            if (tiles[newPos.y][newPos.x]->isMine())
            {
                neighbors += 1;
            }
        }
    }

    return neighbors;
}

bool Minesweeper::InBounds(sf::Vector2<int> pos)
{
    if (pos.x >= 0 && pos.x < size.x)
    {
        if (pos.y >= 0 && pos.y < size.y)
        {
            return true;
        }
    }

    return false;
}

int Minesweeper::Random(int max)
{
    return (rand() / double(RAND_MAX)) * max;
}

