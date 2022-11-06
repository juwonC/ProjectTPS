// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGuns::AGuns()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	// Rifle
	rifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));
	rifleMesh->SetupAttachment(root);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultGunMesh(TEXT("SkeletalMesh'/Game/AssetPacks/ShooterGame/Weapons/Rifle.Rifle'"));
	if (DefaultGunMesh.Succeeded())
	{
		// Assign StaticMesh Data
		rifleMesh->SetSkeletalMesh(DefaultGunMesh.Object);

		// Adjust Location
		rifleMesh->SetRelativeLocation(FVector(-8.2f, 5.46f, -2.32f));
		rifleMesh->SetRelativeRotation(FRotator(0, 90, 0));
	}
}

void AGuns::PullTrigger()
{
	//UE_LOG(LogTemp, Warning, TEXT("You've been shot!"))

	if (ammo <= 0)
	{
		return;
	}

	UGameplayStatics::SpawnEmitterAttached(muzzleFlash, rifleMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(muzzleSound, rifleMesh, TEXT("MuzzleFlashSocket"));

	FHitResult hit;
	FVector shotDirection;

	bool bSuccess = GunTrace(hit, shotDirection);

	if (bSuccess)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffect, hit.Location, shotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), impactSound, hit.Location);

		AActor* hitActor = hit.GetActor();
		if (hitActor != nullptr)
		{
			FPointDamageEvent damageEvent(damage, hit, shotDirection, nullptr);
			AController* ownerController = GetOwnerController();
			hitActor->TakeDamage(damage, damageEvent, ownerController, this);
		}
	}

	ammo--;
}

int32 AGuns::SetAmmo(int32 reload)
{
	ammo = reload;
	
	return ammo;
}

int32 AGuns::GetAmmo()
{
	return ammo;
}

// Called when the game starts or when spawned
void AGuns::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGuns::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGuns::GunTrace(FHitResult& hit, FVector& shotDirection)
{
	AController* ownerController = GetOwnerController();
	if (ownerController == nullptr)
	{
		return false;
	}

	FVector location;
	FRotator rotation;
	ownerController->GetPlayerViewPoint(location, rotation);
	
	shotDirection = -rotation.Vector();
	FVector end = location + rotation.Vector() * maxRange;

	//DrawDebugCamera(GetWorld(), location, rotation, 90, 2, FColor::Red, true);
	
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(hit, location, end, ECollisionChannel::ECC_GameTraceChannel1, params);
}

AController* AGuns::GetOwnerController() const
{
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr)
	{
		return nullptr;
	}

	return ownerPawn->GetController();
}

