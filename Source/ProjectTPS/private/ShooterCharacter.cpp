// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Guns.h"
#include "Components/CapsuleComponent.h"
#include "ProjectTPSGameModeBase.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	activeIndex = 0;
	UE_LOG(LogTemp, Warning, TEXT("Active Index = %d"), activeIndex);

	for (int32 i = 0; i < 3; i++)
	{
		gun[i] = GetWorld()->SpawnActor<AGuns>(gunClass[i]);

		if (gun[i])
		{
			gun[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			gun[i]->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("Attached Mesh Number  %d"), i);

			if (i != activeIndex)
			{
				gun[i]->SetActorHiddenInGame(true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Gun Attached on %d"), i);
		}
	}
}

bool AShooterCharacter::IsDead() const
{
	if (health <= 0)
	{
		return true;
	}
	
	return false;
}

float AShooterCharacter::GetHealthPercent() const
{
	return health / maxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AShooterCharacter::TurnRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AShooterCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AShooterCharacter::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);

	PlayerInputComponent->BindAxis(TEXT("SwitchWeapons"), this, &AShooterCharacter::SwitchWeapons);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageToApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	damageToApplied = FMath::Min(health, damageToApplied);
	health -= damageToApplied;

	UE_LOG(LogTemp, Display, TEXT("Health Left %f"), health);

	if (IsDead())
	{
		AProjectTPSGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AProjectTPSGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return damageToApplied;
}

//void AShooterCharacter::Turn(float axisValue)
//{
//	AddControllerYawInput(axisValue);
//}

//void AShooterCharacter::LookUpRate(float axisValue)
//{
//	AddControllerPitchInput(axisValue);
//}

void AShooterCharacter::TurnRate(float axisValue)
{
	AddControllerYawInput(axisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpRate(float axisValue)
{
	AddControllerPitchInput(axisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::InputHorizontal(float axisValue)
{
	AddMovementInput(GetActorRightVector() * axisValue);
}

void AShooterCharacter::InputVertical(float axisValue)
{
	AddMovementInput(GetActorForwardVector() * axisValue);
}

void AShooterCharacter::SwitchWeapons(float slot)
{
	int32 slotIndex = static_cast<int32>(slot);

	gun[activeIndex]->SetActorHiddenInGame(true);
	activeIndex += slotIndex;
	activeIndex = FMath::Clamp(activeIndex, 0, 2);
	gun[activeIndex]->SetActorHiddenInGame(false);
}

void AShooterCharacter::Shoot()
{
	gun[activeIndex]->PullTrigger();
}
