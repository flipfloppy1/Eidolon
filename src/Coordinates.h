#pragma once
#include <cmath>
#include "raylib.h"

enum TileType : unsigned char
{
    EMPTY,
    DUNGEON,
    ICE
};

enum Direction
{
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST
};

struct InDirection
{
    bool north, south, east, west, northeast, northwest, southeast, southwest;
};

struct TileCoords
{
    int x, y;
    // Returns coords representing these coords moved one space in the direction specified. Does not modify the original.
    const inline TileCoords Move(Direction dir) const
    {
        switch (dir)
        {
        case NORTH:
            return {x, y - 1};
        case SOUTH:
            return {x, y + 1};
        case WEST:
            return {x - 1, y};
        case EAST:
            return {x + 1, y};
        case NORTHWEST:
            return {x - 1, y - 1};
        case NORTHEAST:
            return {x + 1, y - 1};
        case SOUTHWEST:
            return {x - 1, y + 1};
        case SOUTHEAST:
            return {x + 1, y + 1};
        }
        return {x, y};
    }
    const inline TileCoords operator/(int constant) const { return {x / constant, y / constant}; }
    inline bool operator<(TileCoords const& rhs) const
    {
        if (x != rhs.x)
            return (x < rhs.x);
        else
            return y < rhs.y;
    }
    inline TileCoords operator*(int constant) const { return {x * constant, y * constant}; }
    inline TileCoords operator+(TileCoords const& rhs) const { return {x + rhs.x, y + rhs.y}; }
    inline TileCoords operator-(TileCoords const& rhs) const { return {x - rhs.x, y - rhs.y}; }
    inline bool operator==(TileCoords const& rhs) const { return x == rhs.x && y == rhs.y; }
    inline bool operator!=(TileCoords const& rhs) const { return !operator==(rhs); }
    // Distance between points, uses <=> because it resembles a line :)
    inline float operator<=>(TileCoords const& rhs) const { return std::sqrt(((float)this->x - (float)rhs.x) * ((float)this->x - (float)rhs.x) + ((float)this->y - (float)rhs.y) * ((float)this->y - (float)rhs.y)); }
    inline float operator<=>(Vector2 const& rhs) const { return std::sqrt(((float)this->x - rhs.x) * ((float)this->x - rhs.x) + ((float)this->y - rhs.y) * ((float)this->y - rhs.y)); }
};
