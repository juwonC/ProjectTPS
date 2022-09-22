// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Guns.generated.h"

UCLASS()
class PROJECTTPS_API AGuns : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuns();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* rifleMesh;
};
