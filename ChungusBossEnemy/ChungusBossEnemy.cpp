#include "ChungusBossEnemy.h"
// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "ChungusBossEnemy.h"

#include "Duskborn/Bullet/Bullet.h"
#include "Duskborn/Character/Player/PlayerCharacter.h"
#include "Duskborn/Management/WorldManager.h"
#include "StateMachine/ChungusStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Duskborn/Helper/Log.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AChungusBossEnemy::AChungusBossEnemy()
{
	StateMachine = CreateDefaultSubobject<UChungusStateMachine>("Drone State Machine");
}

// Called when the game starts or when spawned
void AChungusBossEnemy::BeginPlay()
{
	Super::BeginPlay();

	Offsets.Add(0);
	Offsets.Add(1);
	Offsets.Add(-1);

	OriginalMovementSpeed = 10;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

FVector AChungusBossEnemy::RocketMovement(FVector Heading)
{
	if (!GetWorld())
	{
		Log::Print("World not found in Chungus Rocket Movement");
		return FVector::Zero();
	}
	FVector Forward = Heading;
	float CollisionAvoidanceDistance = 100;

	constexpr float MaxAngle = 180.0f;
	FVector TargetHeading = FVector::Zero();

	int Sign = 1;
	float CurrentAngle = 0.0f;

	bool hit = false;
	//vertical scan
	while (FMath::Abs(CurrentAngle) < MaxAngle * 2)
	{
		FVector UpVector = GetActorUpVector();
		FVector RayDirection = Forward.RotateAngleAxis(CurrentAngle * Sign, UpVector);
		FVector RightVector = RayDirection.Cross(UpVector);
		FVector RayOrigin = GetActorLocation();

		hit = false;
		for (auto Offset : Offsets)
		{
			FVector OffsetOrigin = RayOrigin + RightVector * Offset * CharacterWidth;
			//UE_LOG(LogTemp, Warning, TEXT("%f"), Offset * CharacterWidth);
			FVector RayEndPoint = OffsetOrigin + RayDirection * CollisionAvoidanceDistance;
			FHitResult HitResult;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, OffsetOrigin, RayEndPoint, ECC_GameTraceChannel7))
			{
				if (ShowDebugInformation)
					DrawDebugLine(GetWorld(), OffsetOrigin, RayEndPoint, FColor::Red, false, 0.5);

				hit = true;
				break;
			}
		}
		if (!hit)
		{
			TargetHeading = RayDirection;
			break;
		}


		RightVector = GetActorRightVector();
		RayDirection = Forward.RotateAngleAxis(CurrentAngle * Sign, RightVector);
		RayOrigin = GetActorLocation();

		hit = false;
		for (auto Offset : Offsets)
		{
			FVector OffsetOrigin = RayOrigin + RightVector * Offset * CharacterWidth;
			FVector RayEndPoint = OffsetOrigin + RayDirection * CollisionAvoidanceDistance;
			FHitResult HitResult;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, OffsetOrigin, RayEndPoint, ECC_GameTraceChannel7))
			{
				if (ShowDebugInformation)
					DrawDebugLine(GetWorld(), OffsetOrigin, RayEndPoint, FColor::Red, false, 0.5);

				hit = true;
				break;
			}
		}
		if (!hit)
		{
			TargetHeading = RayDirection;
			break;
		}


		Sign = Sign == 1 ? -1 : 1;
		CurrentAngle += 30.0f;
	}

	return TargetHeading;
}


bool AChungusBossEnemy::IsPlayerInActionRange()
{
	if (this->IsPlayerInTriggerDistance(TriggerDistanceActionState))
	{
		return this->IsPlayerInSight();
	}
	return false;
}

bool AChungusBossEnemy::IsPlayerInIdleRange()
{
	if (this->IsPlayerInTriggerDistance(TriggerDistanceActionState))
	{
		return !(this->IsPlayerInSight());
	}
	return true;
}


void AChungusBossEnemy::MovementAction(float DeltaTime)
{
	FollowSpline(1);
}


void AChungusBossEnemy::Die()
{
	Super::Die();
	StateMachine->StateTransition(Cast<UChungusStateMachine>(StateMachine)->DeathState);
	SpawnExplosion();
}
