// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "ProjectTPS.h"

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
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	me->AddMovementInput(dir.GetSafeNormal());

	if (dir.Size() < attackRange)
	{
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		PRINT_LOG(TEXT("Attack!!!"));
		currentTime = 0.0f;
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}

