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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// LeftRight Rotation Input
	void Turn(float value);

	// UpDown Rotation Input
	void LookUp(float value);

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

	// Gun SkeletalMesh
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	// Bullet Factory
	UPROPERTY(EditDefaultsOnly, Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

	// Add Sniper StaticMesh
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class UStaticMeshComponent* sniperGunComp;

	// Switch Guns
	bool bUsingGrenadeGun = true;

	void ChangeToGrenadeGun();
	void ChangeToSniperGun();

	// Sniper Mode UI
	void SniperAim();
	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	class UUserWidget* _sniperUI;

	// Bullet Effect
	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	// Crosshair UI
	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	class UUserWidget* _crosshairUI;

	// Running Event
	void InputRun();
};
