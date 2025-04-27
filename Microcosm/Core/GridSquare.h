#pragma once
class Unit;

class GridSquare
{
public:
    GridSquare(int NewX, int NewY);

    int GetX() const { return X; }
    int GetY() const { return Y; }
    Unit* GetCurrentUnit() const { return CurrentUnit; }

	bool IsEmpty() const { return CurrentUnit == nullptr; }
    void SetCurrentUnit(Unit* NewUnit);
    void ClearCurrentUnit();

private:
    int X; 
    int Y; 
    Unit* CurrentUnit = nullptr;
};
