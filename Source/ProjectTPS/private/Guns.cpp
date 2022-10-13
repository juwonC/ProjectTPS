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

	UGameplayStatics::SpawnEmitterAttached(muzzleFlash, rifleMesh, TEXT("MuzzleFlashSocket"));

	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (ownerPawn == nullptr)
	{
		return;
	}

	AController* ownerController = ownerPawn->GetController();
	if (ownerController == nullptr)
	{
		return;
	}

	FVector location;
	FRotator rotation;
	ownerController->GetPlayerViewPoint(location, rotation);

	FVector end = location + rotation.Vector() * maxRange;

	//DrawDebugCamera(GetWorld(), location, rotation, 90, 2, FColor::Red, true);
	
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(hit, location, end, ECollisionChannel::ECC_GameTraceChannel1, params);
	if (bSuccess)
	{
		FVector shotDirection = -rotation.Vector();

		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hit.Location, shotDirection.Rotation());
		
		AActor* hitActor = hit.GetActor();
		if (hitActor != nullptr)
		{
			FPointDamageEvent damageEvent(damage, hit, shotDirection, nullptr);
			hitActor->TakeDamage(damage, damageEvent, ownerController, this);
		}
	}
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

