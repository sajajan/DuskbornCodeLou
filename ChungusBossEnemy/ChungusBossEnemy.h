// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Duskborn/Bullet/Bullet.h"
#include "Duskborn/Character/Enemy/EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "ChungusBossEnemy.generated.h"


/**
 * 
 */
UCLASS()
class DUSKBORN_API AChungusBossEnemy : public AEnemyCharacter
{
	GENERATED_BODY()

	//-----------------------Variables----------------------------------------------------------------------------------  

	//public variables
	public:
	TArray<float> Offsets;
	
	
	UPROPERTY(EditAnywhere, Category="Duskborn|BehaviorActionState")
	float TriggerDistanceActionState = 30000;
	UPROPERTY(EditAnywhere, Category="Duskborn|BehaviorAlertState")
	float AlertTimeRange = 10;
	
	
	UPROPERTY(EditAnywhere, Category="Duskborn|Movement")
	float MovementMultiplier = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Duskborn|Movement|Collision")
	float CharacterWidth = 200.0f;
	float OriginalMovementSpeed = 0.0f;

	int8 CurrentEnemiesSpawned = 0;
	int8 AmountOfEnemiesToSpawn = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Duskborn|DebugInformation")
	bool ShowDebugInformation = false;
	bool IsSecondPhase = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsShootingRockets = false;

	//protected variables
	protected:

	//private variables
	private:

	
	//-----------------------Functions----------------------------------------------------------------------------------

	//public functions  
	public: // Sets default values for this character's properties
	AChungusBossEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void FollowSpline(int Direction);
	UFUNCTION(BlueprintImplementableEvent)
	void StopMoving();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnRocket();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnBomb();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnWalker();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnDrone();
	UFUNCTION(BlueprintImplementableEvent)
	void OpenBlend();
	UFUNCTION(BlueprintImplementableEvent)
	void ChargeBombAttack();

	UFUNCTION(BlueprintCallable)
	FVector RocketMovement(FVector Heading);
	

	// Player detection functions
	bool IsPlayerInActionRange();
	bool IsPlayerInIdleRange();

	// Movement Functions
	void MovementAction(float DeltaTime);
	virtual void Die() override;
	
	// Helpers

	
	//protected functions
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//private functions  
	private:
};
