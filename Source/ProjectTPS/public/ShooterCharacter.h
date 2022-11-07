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
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	int32 GetAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void Shoot();
	void OnReload();

private:
	//void Turn(float axisValue);
	//void LookUp(float axisValue);
	void TurnRate(float axisValue);
	void LookUpRate(float axisValue);
	void InputHorizontal(float axisValue);
	void InputVertical(float axisValue);
	void SwitchWeapons(float slot);

	UPROPERTY(EditAnywhere)
	float rotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float maxHealth = 100;

	UPROPERTY(EditDefaultsOnly)
	int32 maxAmmo = 50;

	UPROPERTY(VisibleAnywhere)
	float health;

	UPROPERTY(EditAnywhere)
	int32 activeIndex;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGuns> gunClass[3];

	UPROPERTY()
	AGuns* gun[3];
};
