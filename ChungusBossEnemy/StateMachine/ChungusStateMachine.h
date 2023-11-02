// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChungusStates.h"
#include "Duskborn/Character/Enemy/StateMachine/EnemyStateMachine.h"
#include "ChungusStateMachine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSKBORN_API UChungusStateMachine : public UEnemyStateMachine

{
	GENERATED_BODY()
	
	//-----------------------Variables----------------------------------------------------------------------------------  

	//public variables
	public:
	UPROPERTY()
	UChungusIdle* IdleState = nullptr;
	UPROPERTY()
	UChungusAction* ActionState = nullptr;
	UPROPERTY()
	UChungusDeath* DeathState = nullptr;
	UPROPERTY()
	UChungusRocketAttack* RocketAttackState = nullptr;
	UPROPERTY()
	UChungusBombAttack* BombAttackState = nullptr;
	UPROPERTY()
	UChungusSpawnEnemies* SpawnEnemies = nullptr;
	UPROPERTY()
	UChungusSpawnAssassins* SpawnAssassins = nullptr;
	UPROPERTY()
	UChungusGotHit* GotHit = nullptr;
	//protected variables
	protected:
	//private variables
	private:
	//-----------------------Functions----------------------------------------------------------------------------------

	//public functions  
	public:
	UChungusStateMachine();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	//protected functions
	protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//private functions  
	private:
};
