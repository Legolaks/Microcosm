#include "GridSquare.h"

GridSquare::GridSquare(int NewX, int NewY)
{
	X = NewX;
	Y = NewY;
	CurrentUnit = nullptr;
}

void GridSquare::SetCurrentUnit(Unit* NewUnit)
{
    CurrentUnit = NewUnit;
}

void GridSquare::ClearCurrentUnit()
{
    CurrentUnit = nullptr;
}
