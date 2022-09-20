// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent), meta = (BlueprintSpawnableComponent))
class PROJECTTPS_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerFire();

	virtual void BeginPlay() override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UCameraComponent* tpsCamComp;

	// Gun SkeletalMesh
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;

	// Add Sniper StaticMesh
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;

public:
	// Bullet Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

	// Switch Guns
	bool bUsingGrenadeGun = true;

	void ChangeToGrenadeGun();
	void ChangeToSniperGun();

	// Sniper Mode UI
	void SniperAim();
	bool bSniperAim = false;

	// Snuper UI Widget Factory
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> sniperUIFactory;

	// Sniper UI Widget Instance
	class UUserWidget* _sniperUI;

	// Bullet Effect
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;

	// Crosshair UI
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> crosshairUIFactory;

	class UUserWidget* _crosshairUI;

	// Camera Shake Blueprint Varaible
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;

	// Bullet Sound
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;
};
