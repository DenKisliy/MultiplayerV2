// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/GameplayStatics.h"
#include "../Character/MAICharacter.h"
#include "../AI/Move/MPatrolPath.h"
#include "../Character/MPlayerCharacter.h"
#include "../Managers/MBaseManager.h"

#include "MSpawnBotManager.generated.h"

UCLASS()
class MULTIPLAYER_API AMSpawnBotManager : public AMBaseManager
{
	GENERATED_BODY()
	 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor, meta = (AllowPrivateAccess = true))
	TSubclassOf<AMAICharacter> AICharacterStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor, meta = (AllowPrivateAccess = true))
	TSubclassOf<AMPatrolPath> PatrolPathStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	TArray<FItemTypeInfo> RewardItemsArray;

private:
	TMap<AMAICharacter*, AMPlayerCharacter* > DetectPlayer;

public:	
	// Sets default values for this actor's properties
	AMSpawnBotManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SpawnBot(ETypeOfTimer TypeOfFinishTimer);

	UFUNCTION()
	void DetectPlayerByBot(AActor* Bot, AActor* Player);

	UFUNCTION()
	void PlayerRemoveSaveZone(AActor* Player);

	UFUNCTION()
	void OnActorDeath(FVector Location);
};
