// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Get Random Time
	float createTime = FMath::RandRange(minTime, maxTime);
	// Register Alarm to Timer Manager
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);

	// Automatically Assign Spawn Points
	FindSpawnPoints();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemy()
{
	// Get Random Position
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);
	// Place and Create Enemy
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));

	// Set Timer
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
}

void AEnemyManager::FindSpawnPoints()
{
	// Using Iterator
	
	//for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	//{
	//	AActor* spawn = *It;

	//	if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoints")))
	//	{
	//		spawnPoints.Add(spawn);
	//	}
	//}


	// Using GetAllActorsOfClass
	
	TArray<AActor*> allActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);

	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoints")))
		{
			spawnPoints.Add(spawn);
		}
	}
}