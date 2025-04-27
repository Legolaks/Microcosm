#pragma once

class GridSquare;

class Unit
{
public:
    Unit(int NewGridX, 
        int NewGridY, 
        float NewMovementSpeed, 
        int NewMaxHitPoints, 
        int NewAttackRange, 
        float NewAttackRate,
        float NewDeadBodyTime);
    virtual ~Unit() = default;

    int GetGridX() const { return GridX; }
    int GetGridY() const { return GridY; }
    float GetMovementSpeed() const { return MovementSpeed; }
    int GetHitPoints() const { return HitPoints; }
    int GetMaxHitPoints() const { return MaxHitPoints; }
    int GetAttackRange() const { return AttackRange; }
    float GetAttackRate() const { return AttackRate; }
    bool GetIsAttacking() const { return IsAttacking; }
	Unit* GetTargetUnit() const { return TargetUnit; }

    void SetGridX(int NewGridX) { GridX = NewGridX; PositionX = NewGridX; }
    void SetGridY(int NewGridY) { GridY = NewGridY; PositionY = NewGridY; }
	void SetTargetUnit(Unit* NewTarget) { TargetUnit = NewTarget; }

    void MoveTowards(int TargetX, int TargetY);
    void Attack(Unit* Target);
    void TakeDamage(int DamageAmount);
    bool IsAlive() const;
    bool IsPandingToDestroy() const;
    bool IsInRange(const Unit* OtherUnit) const;
	bool IsCanAttack() const;

    void TickAttackTimer(float DeltaTime);
	void TickDeadBodyTimer(float DeltaTime);

    int CalculateDistance(const Unit* OtherUnit) const;

protected:
    int GridX;
    int GridY;
	float PositionX; // X position in the world
	float PositionY; // Y position in the world
	
	float MovementSpeed; // Speed of the unit
    int HitPoints; // Current health
    int MaxHitPoints; // Initial health
    int AttackRange; // Distance to trigger an attack, and it stops its movement
    float AttackRate; // Time steps per attack
    float AttackTimer; // Time until the next attack
    bool IsAttacking; // Indicates if the unit is currently attacking
	float DeadBodyTime; // Time until the unit disappears after death

	Unit* TargetUnit; // The target unit
};