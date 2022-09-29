// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGuns;

UCLASS()
class PROJECTTPS_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//void Turn(float axisValue);
	//void LookUp(float axisValue);
	void TurnRate(float axisValue);
	void LookUpRate(float axisValue);
	void InputHorizontal(float axisValue);
	void InputVertical(float axisValue);

	UPROPERTY(EditAnywhere)
	float rotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGuns> GunClass;

	UPROPERTY()
	AGuns* Gun;
};
