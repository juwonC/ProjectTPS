// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class PROJECTTPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// LeftRight Rotation Input
	void Turn(float value);

	// UpDown Rotation Input
	void LookUp(float value);

	// Speed
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;

	// Direction
	FVector direction;

	// Horizontal Move Input Event
	void InputHorizontal(float value);

	// Vertical Move Input Event
	void InputVertical(float value);

	// Jump Input Event
	void InputJump();

	void Move();
};
