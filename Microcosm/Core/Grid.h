#pragma once

#include <vector>
#include "GridSquare.h"

class Grid
{
public:
	Grid(int NewGridWidth, int NewGridHeight);
    ~Grid();

	int GetWidth() const { return GridWidth; }
	int GetHeight() const { return GridHeight; }

    GridSquare* GetGridSquare(int X, int Y) const;
    std::vector<GridSquare*> GetSquareNeighbors(GridSquare* square) const;
    GridSquare* GetRandomEmptySquare();

private:
    int GridWidth;
    int GridHeight;
    std::vector<std::vector<GridSquare*>> Squares;
};