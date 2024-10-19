// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"
#include "../AI/Character/MAIBaseCharacter.h"
#include "../Character/MBaseCharacter.h"
#include "../GameFramework/MGameState.h"

#include "MSpawnBotManager.generated.h"

UCLASS()
class MULTIPLAYER_API AMSpawnBotManager : public AActor
{
	GENERATED_BODY()
	 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor, meta = (AllowPrivateAccess = true))
	TSubclassOf<AMAIBaseCharacter> AICharacterStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor, meta = (AllowPrivateAccess = true))
	TSubclassOf<AMPatrolPath> PatrolPathStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	TArray<FItemTypeInfo> RewardItemsArray;

private:
	TMap<AMAIBaseCharacter*, AMBaseCharacter* > DetectPlayer;

public:	
	// Sets default values for this actor's properties
	AMSpawnBotManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SpawnBot(ETypeOfTimer TypeOfFinishTimer);

	UFUNCTION()
	void DetectPlayerByBot(AActor* Bot, AActor* Player);

	UFUNCTION()
	void PlayerRemoveSaveZone(AActor* Player);

	UFUNCTION()
	void OnActorDeath(FVector Location);
};
