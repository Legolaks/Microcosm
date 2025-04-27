#pragma once
#include <vector>
#include "RedBall.h"
#include "BlueBall.h"
#include "Grid.h"
#include "GridSquare.h"

class AutoBattleSimulation
{
public:
    AutoBattleSimulation(int NewGridWidth, int NewGridHeight, int NewUnitsNum);

    void Initialize();
    void Tick(float DeltaTime);
	void TickUnit(Unit* unit, float DeltaTime);
	void MoveUnit(Unit* unit);
    void CheckAttack(Unit* unit, float DeltaTime);
	void TickUnitsDeadBodies(Unit* unit, float DeltaTime);
	bool IsUnitsSameColor(Unit* unit1, Unit* unit2) const;

	const std::vector<RedBall*>& GetRedUnits() { return RedUnits; } 
	const std::vector<BlueBall*>& GetBlueUnits() { return BlueUnits; }

private:

	Grid* GridInstance;

    std::vector<RedBall*> RedUnits;
    std::vector<BlueBall*> BlueUnits;

    void PlaceUnitsRandomly();
	void PlaceUnitAtRandomPosition(Unit* unit);
    Unit* FindClosestTarget(Unit* unit);
	bool IsUnitValid(Unit* unit) const;
	bool IsUnitCanAction(Unit* unit) const;
};