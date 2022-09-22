// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns.h"
#include "Components/SkeletalMeshComponent.h"

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

