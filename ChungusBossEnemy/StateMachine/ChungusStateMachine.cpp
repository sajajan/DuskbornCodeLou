// Fill out your copyright notice in the Description page of Project Settings.

#include "ChungusStateMachine.h"
#include "ChungusStates.h"
#include "Duskborn/Character/Enemy/ChungusBossEnemy/ChungusBossEnemy.h"


// Sets default values for this component's properties
UChungusStateMachine::UChungusStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChungusStateMachine::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AChungusBossEnemy>(GetOwner());

	IdleState = NewObject<UChungusIdle>();
	IdleState->Init(this);
	ActionState = NewObject<UChungusAction>();
	ActionState->Init(this);
	DeathState = NewObject<UChungusDeath>();
	DeathState->Init(this);
	RocketAttackState = NewObject<UChungusRocketAttack>();
	RocketAttackState->Init(this);
	BombAttackState = NewObject<UChungusBombAttack>();
	BombAttackState->Init(this);
	SpawnEnemies = NewObject<UChungusSpawnEnemies>();
	SpawnEnemies->Init(this);
	SpawnAssassins = NewObject<UChungusSpawnAssassins>();
	SpawnAssassins->Init(this);
	GotHit = NewObject<UChungusGotHit>();
	GotHit->Init(this);
	CurrentState = IdleState;
	CurrentState->Start();
}


// Called every frame
void UChungusStateMachine::TickComponent(float DeltaTime, ELevelTick TickType,
										FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
