// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	EEnemyState animState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	bool bAttackPlay = false;

	// Attack End Event
	UFUNCTION(BlueprintCallable, Category=FSMEvent)
	void OnEndAttackAnimation();

	// Damaged Anim Event
	UFUNCTION(BlueprintImplementableEvent, Category=FSMEvent)
	void PlayDamageAnim(FName sectionName);

	// Die Anim Event
	UFUNCTION(BlueprintImplementableEvent, Category=FSMEvent)
	void PlayDieAnim(FName sectionName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool bDieDone = false;
};
