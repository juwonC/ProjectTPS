// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "EnemyFSM.h"
#include "Bullet.h"
#include "PlayerAnim.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>

UPlayerFire::UPlayerFire()
{
	// Bullet Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;
	
	// Create Sniper Mode UI Widget Instance
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	// Create Crosshair UI Widget Instance
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// Register Crosshair UI
	_crosshairUI->AddToViewport();

	// Set Default Gun
	ChangeToSniperGun();
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);

	// Switch Guns
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniperGun);

	// SniperMode
	PlayerInputComponent->BindAction(TEXT("SniperMode"), IE_Pressed, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(TEXT("SniperMode"), IE_Released, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::InputFire()
{
	// Play Bullet Sound
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

	// Play Camera Shake
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	// Play Fire Animation
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else
	{
		// LineTrace Start Position
		FVector startPos = tpsCamComp->GetComponentLocation();
		// LineTrace End Position
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		// Collision Info Variable
		FHitResult hitInfo;
		// Collision Option Variable
		FCollisionQueryParams params;
		// Ignore player Collision
		params.AddIgnoredActor(me);

		// Detect LineTrace Collision using Channel Filter
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		if (bHit)
		{
			// Collision Effect
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			// Hit Actor
			auto hitComp = hitInfo.GetComponent();
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;

				hitComp->AddForce(force);
			}

			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}

void UPlayerFire::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;

	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);

	me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;

	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);

	me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::SniperAim()
{
	if (bUsingGrenadeGun)
	{
		return;
	}

	// Pressed Input Process
	if (bSniperAim == false)
	{
		// Activate Sniper Mode
		bSniperAim = true;
		// Add SniperUI
		_sniperUI->AddToViewport();
		// Set Field of View
		tpsCamComp->SetFieldOfView(45.0f);
		// Remove Crosshair UI
		_crosshairUI->RemoveFromParent();
	}
	// Released Process
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
		// Register Crosshair UI
		_crosshairUI->AddToViewport();
	}
}
