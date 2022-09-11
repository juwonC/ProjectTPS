// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Collision
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	// Set Collision profile
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	// Set Collision size
	collisionComp->SetSphereRadius(13);

	// Create as Root
	RootComponent = collisionComp;

	// Create StaticMesh
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));

	// Set Attachment
	bodyMeshComp->SetupAttachment(collisionComp);

	// Diable collision
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Set StaticMesh size
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	// Projectile Component
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	// Set Update Component
	movementComp->SetUpdatedComponent(collisionComp);

	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;

	// Bullet LifeSpan
	//InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle deathTimer;
	//GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);

	GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([this]()->void
	{ Destroy(); } ), 2.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	Destroy();
}

void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		movementComp->InitialSpeed = speed;
		movementComp->MaxSpeed = speed;
	}
}

