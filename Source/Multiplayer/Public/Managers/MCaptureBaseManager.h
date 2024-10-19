// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"
#include "../AdditionalActor/MCaptureStation.h"
#include "../GameFramework/MGameState.h"

#include "MCaptureBaseManager.generated.h"

UCLASS()
class MULTIPLAYER_API AMCaptureBaseManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TSubclassOf<AMCaptureStation> SpawnStationStatic;

private:
	TArray<AActor*> CaptureStationArray;

	AMCaptureStation* CaptureStation;

	int32 CaptureStationCount = -1;
public:	
	// Sets default values for this actor's properties
	AMCaptureBaseManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSetCaptureStation(int32 CountOfPlayer);

	UFUNCTION()
	void SpawnStation(ETypeOfTimer TypeOfFinishTimer);
};
