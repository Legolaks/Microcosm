#include "Grid.h"
#include "Unit.h"

Grid::Grid(int NewGridWidth, int NewGridHeight)
{
	GridWidth = NewGridWidth;
	GridHeight = NewGridHeight;

	Squares.resize(GridHeight, std::vector<GridSquare*>(GridWidth));

	for (int Y = 0; Y < GridHeight; ++Y)
	{
		for (int X = 0; X < GridWidth; ++X)
		{
			Squares[Y][X] = new GridSquare(X, Y);
		}
	}
}

Grid::~Grid()
{
	for (int Y = 0; Y < GridHeight; ++Y)
	{
		for (int X = 0; X < GridWidth; ++X)
		{
			delete Squares[Y][X];
		}
	}
}

std::vector<GridSquare*> Grid::GetSquareNeighbors(GridSquare* square) const
{
	std::vector<GridSquare*> neighbors;
	int x = square->GetX();
	int y = square->GetY();

	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0))
			{
				continue; // Skip the current square and diagonals
			}

			int newX = x + dx;
			int newY = y + dy;

			if (newX >= 0 && newX < GridWidth && newY >= 0 && newY < GridHeight)
			{
				neighbors.push_back(Squares[newY][newX]);
			}
		}
	}

	return neighbors;
}

GridSquare* Grid::GetGridSquare(int X, int Y) const
{
	if (X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight)
	{
		return Squares[Y][X];
	}
	return nullptr;
}

GridSquare* Grid::GetRandomEmptySquare()
{
	int X = rand() % GridWidth;
	int Y = rand() % GridHeight;

	while (!Squares[Y][X]->IsEmpty())
	{
		X = rand() % GridWidth;
		Y = rand() % GridHeight;
	}

	return Squares[Y][X];
}