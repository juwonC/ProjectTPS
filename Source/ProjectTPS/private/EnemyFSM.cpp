// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "ProjectTPS.h"
#include "EnemyAnim.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//Find ATPSPlayer from World
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// Cast to ATPSPlayer type
	target = Cast<ATPSPlayer>(actor);
	// Get Owned Object
	me = Cast<AEnemy>(GetOwner());

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	// Assign AAIController
	ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
		case EEnemyState::Idle:
			IdleState();
			break;

		case EEnemyState::Move:
			MoveState();
			break;

		case EEnemyState::Attack:
			AttackState();
			break;

		case EEnemyState::Damage:
			DamageState();
			break;

		case EEnemyState::Die:
			DieState();
			break;
	}
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > idleDelayTime)
	{
		mState = EEnemyState::Move;
		currentTime = 0.0f;

		// Sync Animation
		anim->animState = mState;

		// First Random Position
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	//me->AddMovementInput(dir.GetSafeNormal());
	//ai->MoveToLocation(destination);

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// Search Path Finding Data Destination
	FPathFindingQuery query;
	FAIMoveRequest req;

	// Recognize Range at Destination
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	// Create Query for Path Finding
	ai->BuildPathfindingQuery(req, query);

	// Get Path Finding result
	FPathFindingResult r = ns->FindPathSync(query);

	// Check Success for Path Finding
	if (r.Result == ENavigationQueryResult::Success)
	{
		// Move to Target
		ai->MoveToLocation(destination);
	}
	else
	{
		// Move Random Position
		auto result = ai->MoveToLocation(randomPos);
		// Arrived at Destination
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// Get New Randon Position
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	if (dir.Size() < attackRange)
	{
		// Stop Path Finding
		ai->StopMovement();
		
		mState = EEnemyState::Attack;

		// Sync Animation
		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		//PRINT_LOG(TEXT("Attack!!!"));
		currentTime = 0.0f;

		anim->bAttackPlay = true;
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;

		anim->animState = mState;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0.0f;

		anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
	//if (anim->bDieDone == false)
	//{
	//	return;
	//}
	
	// constant velocity motion
	// P = P0 + vt
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;

	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		currentTime = 0;

		// Play Damage Anim
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), 0);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		mState = EEnemyState::Die;
		// Deactivate Capsule Collision
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		anim->PlayDieAnim(TEXT("Die"));
	}

	anim->animState = mState;
	ai->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;

	return result;
}

