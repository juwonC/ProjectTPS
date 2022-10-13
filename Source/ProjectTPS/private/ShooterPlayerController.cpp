// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	//UE_LOG(LogTemp, Warning, TEXT("We have finished"));

	GetWorldTimerManager().SetTimer(restartTimer, this, &APlayerController::RestartLevel, restartDelay);
}
