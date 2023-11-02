
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Duskborn/Character/BaseClasses/State.h"
#include "ChungusStates.generated.h"

#define CHUNGUS_STATE_MACHINE Cast<UChungusStateMachine>(StateMachine)
#define CHUNGUS Cast<AChungusBossEnemy>(Cast<UEnemyStateMachine>(StateMachine)->Enemy)

class AWalkerEnemy;
class ADroneEnemy;

//Idle State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusIdle : public UState
{
	GENERATED_BODY()
	
	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override;
};


//Action State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusAction : public UState
{
	GENERATED_BODY()

	private:
	UPROPERTY()
	float SecondsSinceLastAttack = 0;
	float RandomEnemySpawnInterval =  FMath::RandRange(2, 5);
	float SecondsSinceLastEnemySpawn = 0;
	float SameAttackInARow = 0;
	float LastAttack = 0;

	UPROPERTY()
	TSubclassOf<ADroneEnemy> DroneClass = nullptr;
	UPROPERTY()
	TSubclassOf<AWalkerEnemy> WalkerClass = nullptr;

	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override; 
};

//Got Hit From Far Away State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusGotHit : public UState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float SecondsSinceLastAttack = 0;

	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override; 
};

//Death State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusDeath : public UState
{
	GENERATED_BODY()

	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override; 
};

//Rocket Attack State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusRocketAttack : public UState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float SecondsSinceLastRocket = 0;
	float SecondsSinceLastAttackMovement = 0;
	float AmountOfRocketsFired = 0;
	float AmountOfRocketsToFire = 10;
	
	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override; 
};

//Bomb Attack State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusBombAttack : public UState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float SecondsSinceChargeStart = 0;
	float AmountOfBombsToSpawn = 10;
	
	
	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override; 
};

//Spawn Standard Enemies State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusSpawnEnemies : public UState
{
	GENERATED_BODY()

	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override;
};


//Spawn Assassins State
//----------------------------------------------------------------------------------------------------------------------

UCLASS()
class DUSKBORN_API UChungusSpawnAssassins : public UState
{
	GENERATED_BODY()

	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float DeltaTime) override;
};


