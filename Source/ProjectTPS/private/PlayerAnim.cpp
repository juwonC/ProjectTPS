// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Get Owner Pawn
	auto ownerPawn = TryGetPawnOwner();
	// Cast to Player
	auto player = Cast<ATPSPlayer>(ownerPawn);
	if (player)
	{
		// Get Movement Speed
		FVector velocity = player->GetVelocity();
		// Get Player Forward Vector
		FVector forwardVector = player->GetActorForwardVector();
		// Assign value to speed
		speed = FVector::DotProduct(forwardVector, velocity);
		// Assign LeftRight speed
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		// Is Player in Air
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}
