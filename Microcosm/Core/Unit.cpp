#include "Unit.h"
#include <algorithm>
#include <iostream>

Unit::Unit(
    int NewGridX, 
    int NewGridY, 
    float NewMovementSpeed, 
    int NewMaxHitPoints, 
    int NewAttackRange, 
    float NewAttackRate,
    float NewDeadBodyTime)
{
	GridX = NewGridX;
	GridY = NewGridY;
	PositionX = static_cast<float>(NewGridX);
	PositionY = static_cast<float>(NewGridY);

	MovementSpeed = NewMovementSpeed;
	HitPoints = NewMaxHitPoints;
	MaxHitPoints = NewMaxHitPoints;
	AttackRange = NewAttackRange;
	AttackRate = NewAttackRate;
	AttackTimer = 0.0f;
	IsAttacking = false;
	DeadBodyTime = NewDeadBodyTime;

	TargetUnit = nullptr;
}

void Unit::MoveTowards(int TargetX, int TargetY) 
{
    int dx = TargetX - GridX;
    int dy = TargetY - GridY;

    float maxMoveX = MovementSpeed;
    float maxMoveY = MovementSpeed;

    // Move on the X-axis
    if (abs(dx) <= maxMoveX) 
    {
        PositionX = TargetX;
    }
    else 
    {
        PositionX += (dx > 0) ? maxMoveX : -maxMoveX;
    }

    // Move on the Y-axis
    if (abs(dy) <= maxMoveY) 
    {
        PositionY = TargetY;
    }
    else 
    {
        PositionY += (dy > 0) ? maxMoveY : -maxMoveY;
    }
	GridX = static_cast<int>(PositionX);
	GridY = static_cast<int>(PositionY);
}


void Unit::Attack(Unit* Target)
{
    if (Target && IsCanAttack())
    {
        Target->TakeDamage(1);
        IsAttacking = true;
        AttackTimer = 0.0f;
    }
}

void Unit::TakeDamage(int DamageAmount)
{
    HitPoints = std::max(0, HitPoints - DamageAmount);
}

bool Unit::IsAlive() const
{
    return HitPoints > 0;
}
bool Unit::IsPandingToDestroy() const
{
    return !IsAlive() && DeadBodyTime <= 0.0f;
}

bool Unit::IsInRange(const Unit* OtherUnit) const
{
    if (!OtherUnit) return false;
    int DistanceX = std::abs(GridX - OtherUnit->GridX);
    int DistanceY = std::abs(GridY - OtherUnit->GridY);
    return (DistanceX <= AttackRange) && (DistanceY <= AttackRange);
}

bool Unit::IsCanAttack() const
{
	return IsAlive() && !IsAttacking;
}

void Unit::TickAttackTimer(float DeltaTime)
{
    if (IsAttacking)
    {
        AttackTimer += DeltaTime;
        if (AttackTimer >= AttackRate)
        {
            IsAttacking = false;
        }
    }
}

void Unit::TickDeadBodyTimer(float DeltaTime)
{
	if (!IsAlive())
	{
		DeadBodyTime -= DeltaTime;
	}
}

int Unit::CalculateDistance(const Unit* OtherUnit) const
{
	if (!OtherUnit) return 0.0f;
	int dx = GridX - OtherUnit->GridX;
    int dy = GridY - OtherUnit->GridY;
	return  static_cast<int>(std::sqrt(dx * dx + dy * dy));
}