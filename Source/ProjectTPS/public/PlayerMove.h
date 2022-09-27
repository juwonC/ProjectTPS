// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// LeftRight Rotation Input
	void Turn(float value);

	// UpDown Rotation Input
	void LookUp(float value);

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	// Walking Speed
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 200;

	// Running Speed
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float runSpeed = 600;

	// Direction
	FVector direction;

	// Horizontal Move Input Event
	void InputHorizontal(float value);

	// Vertical Move Input Event
	void InputVertical(float value);

	// Jump Input Event
	void InputJump();

	void Move();

	// Running Event
	void InputRun();
};
