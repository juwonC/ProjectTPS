// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	// Waiting Time
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2.0f;

	// Elapsed Time
	float currentTime = 0.0f;

	// Target
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ATPSPlayer* target;

	// Owned Actor
	UPROPERTY()
	class AEnemy* me;

	// Attack Range
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;

	// Attack Delay
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 1.0f;

	// Damaged Alarm Event
	void OnDamageProcess();

	// HP
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3;

	// Damaged Delay Time
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 1.0f;

	// Falling Down Speed
	UPROPERTY(EditAnywhere, Category=FSM)
	float dieSpeed = 50.0f;

	// Anim Blueprint
	UPROPERTY()
	class UEnemyAnim* anim;

	// AIController
	UPROPERTY()
	class AAIController* ai;

	// Patrol Random Position
	FVector randomPos;

	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);
};
