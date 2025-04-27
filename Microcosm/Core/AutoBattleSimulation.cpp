#include "AutoBattleSimulation.h"
#include <ctime>
#include <queue>
#include <unordered_map>

AutoBattleSimulation::AutoBattleSimulation(int NewGridWidth, int NewGridHeight, int NewUnitsNum)
{
	GridInstance = new Grid(NewGridWidth, NewGridHeight);

	for (int i = 0; i < NewUnitsNum; ++i)
	{
		RedUnits.push_back(new RedBall());
		BlueUnits.push_back(new BlueBall());
	}
}

void AutoBattleSimulation::Initialize()
{
    srand(static_cast<unsigned int>(time(0)));
    PlaceUnitsRandomly();
}

void AutoBattleSimulation::PlaceUnitsRandomly()
{
	for (RedBall* unit : RedUnits)
	{
		PlaceUnitAtRandomPosition(unit);
	}

	for (BlueBall* unit : BlueUnits)
	{
		PlaceUnitAtRandomPosition(unit);
	}
}

void AutoBattleSimulation::PlaceUnitAtRandomPosition(Unit* unit)
{
	if (unit == nullptr)
	{
		return;
	}
	GridSquare* randomSquare = GridInstance->GetRandomEmptySquare();

	randomSquare->SetCurrentUnit(unit);
	unit->SetGridX(randomSquare->GetX());
	unit->SetGridY(randomSquare->GetY());
}

Unit* AutoBattleSimulation::FindClosestTarget(Unit* unit)
{
	if (unit == nullptr)
	{
		return nullptr;
	}

	int GridWidth = GridInstance->GetWidth();
	int GridHeight = GridInstance->GetHeight();

	Unit* closestTarget = nullptr;
	int closestDistance = std::max(GridWidth, GridHeight);

	for (int Y = 0; Y < GridHeight; ++Y)
	{
		for (int X = 0; X < GridWidth; ++X)
		{
			GridSquare* square = GridInstance->GetGridSquare(X, Y);
			Unit* targetUnit = square->GetCurrentUnit();
			if (targetUnit != nullptr && IsUnitCanAction(targetUnit) && !IsUnitsSameColor(unit, targetUnit))
			{
				int distance = unit->CalculateDistance(targetUnit);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestTarget = targetUnit;
				}
			}
		}
	}

	return closestTarget;
}

void AutoBattleSimulation::Tick(float DeltaTime)
{
	for (RedBall* unit : RedUnits)
	{
		TickUnit(unit, DeltaTime);
	}
	for (BlueBall* unit : BlueUnits)
	{
		TickUnit(unit, DeltaTime);
	}
}

void AutoBattleSimulation::TickUnit(Unit* unit, float DeltaTime)
{
	if (unit == nullptr)
	{
		return;
	}

	if (IsUnitCanAction(unit))
	{
		
		if (unit->GetTargetUnit() != nullptr)
		{
			bool isTargetAlive = unit->GetTargetUnit()->IsAlive();
			bool isTargetPendingToDestroy = unit->GetTargetUnit()->IsPandingToDestroy();
			if (!isTargetAlive || isTargetPendingToDestroy)
			{
				unit->SetTargetUnit(FindClosestTarget(unit));
			}
		}
		if (unit->GetTargetUnit() == nullptr)
		{
			unit->SetTargetUnit(FindClosestTarget(unit));
		}
		MoveUnit(unit);
		CheckAttack(unit, DeltaTime);
		TickUnitsDeadBodies(unit, DeltaTime);
	}
	else
	{
		TickUnitsDeadBodies(unit, DeltaTime);
	}
}
//a very simple lazy movement logic, in future can be updated by using Pathfinder
void AutoBattleSimulation::MoveUnit(Unit* unit)
{
	if (unit == nullptr)
	{
		return;
	}

	Unit* targetUnit = unit->GetTargetUnit();

	if (targetUnit == nullptr || unit->IsInRange(targetUnit))
	{
		return;
	}

	GridSquare* currentSquare = GridInstance->GetGridSquare(unit->GetGridX(), unit->GetGridY());
	GridSquare* targetSquare = GridInstance->GetGridSquare(targetUnit->GetGridX(), targetUnit->GetGridY());

	int unitX = unit->GetGridX();
	int unitY = unit->GetGridY();

	int targetX = targetSquare->GetX();
	int targetY = targetSquare->GetY();

	int dx = 0, dy = 0;

	if (unitX < targetX) dx = 1;
	else if (unitX > targetX) dx = -1;

	if (unitY < targetY) dy = 1;
	else if (unitY > targetY) dy = -1;

	GridSquare* nextSquare = nullptr;

	// Horizontal move
	if (dx != 0)
	{
		nextSquare = GridInstance->GetGridSquare(unitX + dx, unitY);
		if (nextSquare && nextSquare->IsEmpty())
		{
			currentSquare->ClearCurrentUnit();
			nextSquare->SetCurrentUnit(unit);
			unit->SetGridX(nextSquare->GetX());
			unit->SetGridY(nextSquare->GetY());
			return;
		}
	}

	// Vertical movement if horizontal is blocked
	if (dy != 0 && !nextSquare)
	{
		nextSquare = GridInstance->GetGridSquare(unitX, unitY + dy);
		if (nextSquare && nextSquare->IsEmpty())
		{
			// Move in the vertical direction
			currentSquare->ClearCurrentUnit();
			nextSquare->SetCurrentUnit(unit);
			unit->SetGridX(nextSquare->GetX());
			unit->SetGridY(nextSquare->GetY());
			return;
		}
	}

	// If both directions are blocked, try moving sideways
	if (!nextSquare)
	{
		// Check horizontal side movement
		if (dx != 0)
		{
			int sideMove = (dx == 1) ? -1 : 1;
			nextSquare = GridInstance->GetGridSquare(unitX + sideMove, unitY);
			if (nextSquare && nextSquare->IsEmpty())
			{
				// Move sideways
				currentSquare->ClearCurrentUnit();
				nextSquare->SetCurrentUnit(unit);
				unit->SetGridX(nextSquare->GetX());
				unit->SetGridY(nextSquare->GetY());
				return;
			}
		}

		// Check vertical side movement
		if (dy != 0)
		{
			int sideMove = (dy == 1) ? -1 : 1;
			nextSquare = GridInstance->GetGridSquare(unitX, unitY + sideMove);
			if (nextSquare && nextSquare->IsEmpty())
			{
				// Move sideways
				currentSquare->ClearCurrentUnit();
				nextSquare->SetCurrentUnit(unit);
				unit->SetGridX(nextSquare->GetX());
				unit->SetGridY(nextSquare->GetY());
				return;
			}
		}
	}
}

void AutoBattleSimulation::CheckAttack(Unit* unit, float DeltaTime)
{
    if (unit == nullptr || unit->GetTargetUnit() == nullptr)
    {
		return;
    }
    if (unit->IsInRange(unit->GetTargetUnit()))
    {
		unit->TickAttackTimer(DeltaTime);
		unit->Attack(unit->GetTargetUnit());
    }
}

void AutoBattleSimulation::TickUnitsDeadBodies(Unit* unit, float DeltaTime)
{
	if (unit == nullptr)
	{
		return;
	}

	unit->TickDeadBodyTimer(DeltaTime);

	if (unit->IsPandingToDestroy())
	{
		GridSquare* square = GridInstance->GetGridSquare(unit->GetGridX(), unit->GetGridY());
		square->ClearCurrentUnit();
		if (dynamic_cast<RedBall*>(unit))
		{
			RedUnits.erase(std::remove(RedUnits.begin(), RedUnits.end(), unit), RedUnits.end());
		}
		else if (dynamic_cast<BlueBall*>(unit))
		{
			BlueUnits.erase(std::remove(BlueUnits.begin(), BlueUnits.end(), unit), BlueUnits.end());
		}
		delete unit;
	}
}

bool AutoBattleSimulation::IsUnitValid(Unit* unit) const
{
	return unit != nullptr && !unit->IsPandingToDestroy();
}

bool AutoBattleSimulation::IsUnitCanAction(Unit* unit) const
{
	return IsUnitValid(unit) && unit->IsAlive();
}
bool AutoBattleSimulation::IsUnitsSameColor(Unit* unit1, Unit* unit2) const
{
	if (unit1 == nullptr || unit2 == nullptr)
	{
		return false;
	}

	return (dynamic_cast<RedBall*>(unit1) && dynamic_cast<RedBall*>(unit2)) ||
		(dynamic_cast<BlueBall*>(unit1) && dynamic_cast<BlueBall*>(unit2));
}