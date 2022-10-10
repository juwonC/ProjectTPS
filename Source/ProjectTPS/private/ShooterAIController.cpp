// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"



void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	////MoveToActor(playerPawn, 200);

	//
	if (LineOfSightTo(playerPawn))
	{
	//	SetFocus(playerPawn);
	//	MoveToActor(playerPawn, AcceptanceRadius);

		// Setting Player Location
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), playerPawn->GetActorLocation());

		// Setting Last Known Player Location
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), playerPawn->GetActorLocation());
	}
	else
	{
	//	ClearFocus(EAIFocusPriority::Gameplay);
	//	StopMovement();

		// Clear Player Location
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
}