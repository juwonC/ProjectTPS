// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectTPSGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API AKillEmAllGameMode : public AProjectTPSGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PawnKilled(APawn* pawnKilled) override;
};
