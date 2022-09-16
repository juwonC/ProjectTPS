// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "ProjectTPS.h"
#include "EnemyAnim.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>

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

	// Sync Animation
	anim->animState = mState;
}

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	me->AddMovementInput(dir.GetSafeNormal());

	if (dir.Size() < attackRange)
	{
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
		PRINT_LOG(TEXT("Attack!!!"));
		currentTime = 0.0f;

		anim->bAttackPlay = true;
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;

		anim->animState = mState;
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
	}

	anim->animState = mState;
}

