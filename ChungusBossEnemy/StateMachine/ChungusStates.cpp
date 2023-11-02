#pragma once

#include "ChungusStates.h"
#include "CoreMinimal.h"
#include "ChungusStateMachine.h"
#include "Duskborn/Enums.h"
#include "Duskborn/Character/Enemy/EnemyCharacter.h"
#include "Duskborn/Character/Enemy/ChungusBossEnemy/ChungusBossEnemy.h"
#include "Duskborn/Core/StatContainerComponent.h"
#include "Duskborn/Helper/Log.h"
#include "Duskborn/Management/WorldManager.h"
#include "Duskborn/UI/PlayerUI.h"

//Base State
//----------------------------------------------------------------------------------------------------------------------

// void UChungusBaseState::Update(float DeltaTime)
// {
// 	Super::Update(DeltaTime);
// 	if (!CHUNGUS)
// 	{
// 		Log::Print("ChungusBase: no valid Chungus");
// 		return;
// 	}
// 	CHUNGUS->MovementAction(DeltaTime);
// }

//Idle State
//----------------------------------------------------------------------------------------------------------------------

void UChungusIdle::Start()
{
	Super::Start();
	WorldManager::PlayerUI->OnBossDeaggroOrDefeat();
	Log::Print("Entered Idle");
}

void UChungusIdle::End()
{
	Super::End();
}

void UChungusIdle::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	if (!CHUNGUS)
	{
		Log::Print("ChungusIdle: no valid Chungus");
		return;
	}

	if (!CHUNGUS_STATE_MACHINE)
	{
		Log::Print("ChungusIdle: no valid ChungusStateMachine");
		return;
	}
	
	if (CHUNGUS->IsPlayerInActionRange())
	{
		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->ActionState);
		
	}
	if (CHUNGUS->GotHit)
	{
		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->GotHit);
	}
	CHUNGUS->MovementAction(DeltaTime);
}


//Action State
//----------------------------------------------------------------------------------------------------------------------

void UChungusAction::Start()
{
	Super::Start();
	WorldManager::PlayerUI->OnBossAggro("Stormcaller");

}

void UChungusAction::End()
{
	Super::End();
	SecondsSinceLastAttack = 0;
}

void UChungusAction::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	
	if (!CHUNGUS)
	{
		Log::Print("ChungusAction: no valid Chungus");
		return;
	}

	if (!CHUNGUS_STATE_MACHINE)
	{
		Log::Print("ChungusAction: no valid ChungusStateMachine");
		return;
	}

	if (!CHUNGUS->IsPlayerInActionRange())
	{
		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->IdleState);
	}
	
	if ((CHUNGUS->CurrentHealth / CHUNGUS->StatContainer->GetStat(EStat::MaximumHealth)) <= 0.4 && !CHUNGUS->IsSecondPhase)
	{
		CHUNGUS->IsSecondPhase = true;

		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->SpawnAssassins);
	}
	
	//Cast<ADuskbornGameModeBase>(CHUNGUS->GetWorld()->GetAuthGameMode())->EnemiesInLevel 
	
	/*if (SecondsSinceLastEnemySpawn >= RandomEnemySpawnInterval && CHUNGUS->CurrentEnemiesSpawned < CHUNGUS->AmountOfEnemiesToSpawn)
	{
		Log::Print("Spawn");
		RandomEnemySpawnInterval =  FMath::RandRange(10,20);
		SecondsSinceLastEnemySpawn = 0;
		CHUNGUS->CurrentEnemiesSpawned+=2;
		CHUNGUS->SpawnDrone();
		CHUNGUS->SpawnWalker();
	}*/
	
	if (SecondsSinceLastAttack >= 3)
	{
		int8 Random = FMath::RandRange(1,2);

		if (Random == LastAttack)
		{
			SameAttackInARow++;
			if (SameAttackInARow > 2)
			{
				Random--;
				SameAttackInARow = 0;
			}
		}
		else
		{
			SameAttackInARow = 0;
		}
		
		if (Random == 1)
		{
			LastAttack = 1;
			CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->RocketAttackState);
		}
		else
		{
			LastAttack = 2;
			CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->BombAttackState);
		}
		
	}
	
	SecondsSinceLastAttack += DeltaTime;
	SecondsSinceLastEnemySpawn += DeltaTime;
	CHUNGUS->MovementAction(DeltaTime);
}

//Got Hit From Far Away State
//----------------------------------------------------------------------------------------------------------------------

void UChungusGotHit::Start()
{
	Super::Start();
	WorldManager::PlayerUI->OnBossAggro("Stormcaller");
}

void UChungusGotHit::End()
{
	Super::End();
	SecondsSinceLastAttack = 0;
}

void UChungusGotHit::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (!CHUNGUS)
	{
		Log::Print("ChungusGotHit: no valid Chungus");
		return;
	}

	if (!CHUNGUS_STATE_MACHINE)
	{
		Log::Print("ChungusGotHit: no valid ChungusStateMachine");
		return;
	}
	
	if (CHUNGUS->IsPlayerInActionRange())
	{
		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->ActionState);
	}

	if (CHUNGUS->GotHit)
	{
		if (SecondsSinceLastAttack >= 3)
		{
			CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->RocketAttackState);
		}
	}
	else
	{
		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->IdleState);
	}
	SecondsSinceLastAttack+=DeltaTime;
	CHUNGUS->MovementAction(DeltaTime);
}

//Death State
//----------------------------------------------------------------------------------------------------------------------

void UChungusDeath::Start()
{
	Super::Start();
	WorldManager::PlayerUI->OnBossDeaggroOrDefeat();
}

void UChungusDeath::End()
{
	Super::End();
}

void UChungusDeath::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (!CHUNGUS)
	{
		Log::Print("ChungusDeath: no valid Chungus");
		return;
	}
	
	CHUNGUS->StopMoving();
	CHUNGUS->OpenBlend();
	CHUNGUS->GetMesh()->SetSimulatePhysics(true);
	CHUNGUS->GetMesh()->SetEnableGravity(true);
}


//Rocket Attack State
//----------------------------------------------------------------------------------------------------------------------

void UChungusRocketAttack::Start()
{
	Super::Start();
	CHUNGUS->IsShootingRockets = true;
}

void UChungusRocketAttack::End()
{
	Super::End();
}

void UChungusRocketAttack::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (!CHUNGUS)
	{
		Log::Print("ChungusRocket: no valid Chungus");
		return;
	}

	if (!CHUNGUS_STATE_MACHINE)
	{
		Log::Print("ChungusRocket: no valid ChungusStateMachine");
		return;
	}

	if (!CHUNGUS->IsShootingRockets)
	{
		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->ActionState);
	}
	CHUNGUS->MovementAction(DeltaTime);
	
}


//Bomb Attack State
//----------------------------------------------------------------------------------------------------------------------

void UChungusBombAttack::Start()
{
	Super::Start();
	Log::Print("Charge bomb!");
	CHUNGUS->ChargeBombAttack();
	
}

void UChungusBombAttack::End()
{
	Super::End();
	
	SecondsSinceChargeStart = 0;
}

void UChungusBombAttack::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (!CHUNGUS)
	{
		Log::Print("ChungusBomb: no valid Chungus");
		return;
	}

	if (!CHUNGUS_STATE_MACHINE)
	{
		Log::Print("ChungusBomb: no valid ChungusStateMachine");
		return;
	}

	CHUNGUS->MovementAction(DeltaTime);

	if (SecondsSinceChargeStart >= 3)
	{
		CHUNGUS->SpawnBomb();
		CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->ActionState);
	}
	

	SecondsSinceChargeStart += DeltaTime;
}

//Spawn Standard Enemies State
//----------------------------------------------------------------------------------------------------------------------


void UChungusSpawnEnemies::Start()
{
	Super::Start();

	if (!CHUNGUS)
	{
		Log::Print("ChungusEnemySpawn: no valid Chungus");
		return;
	}

	if (!CHUNGUS_STATE_MACHINE)
	{
		Log::Print("ChungusEnemySpawn: no valid ChungusStateMachine");
		return;
	}

	CHUNGUS->CurrentEnemiesSpawned+=2;
}

void UChungusSpawnEnemies::End()
{
	Super::End();
}

void UChungusSpawnEnemies::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

//Spawn Assassins State
//----------------------------------------------------------------------------------------------------------------------


void UChungusSpawnAssassins::Start()
{
	Super::Start();

	Log::Print("Assassins!!!!");
}

void UChungusSpawnAssassins::End()
{
	Super::End();
}

void UChungusSpawnAssassins::Update(float DeltaTime)
{
	//CHUNGUS->StopMoving();
	Super::Update(DeltaTime);
	CHUNGUS_STATE_MACHINE->StateTransition(CHUNGUS_STATE_MACHINE->ActionState);
}




