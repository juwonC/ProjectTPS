// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "Engineutils.h"
#include "GameFramework/Controller.h"

void AKillEmAllGameMode::PawnKilled(APawn* pawnKilled)
{
	Super::PawnKilled(pawnKilled);
	
	//UE_LOG(LogTemp, Warning, TEXT("A pawn was killed"));

	APlayerController* playerController = Cast<APlayerController>(pawnKilled->GetController());
	if (playerController != nullptr)
	{
		//playerController->GameHasEnded(nullptr, false);
		EndGame(false);
	}
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* controller : TActorRange<AController>(GetWorld()))
	{
		//bool bIsPlayerController = controller->IsPlayerController();
		//if (bIsPlayerWinner)
		//{
		//	controller->GameHasEnded(nullptr, bIsPlayerController);
		//}
		//else
		//{
		//	controller->GameHasEnded(nullptr, !bIsPlayerController);
		//}

		bool bIsWinner = controller->IsPlayerController() == bIsPlayerWinner;
		controller->GameHasEnded(controller->GetPawn(), bIsWinner);

	}
}
