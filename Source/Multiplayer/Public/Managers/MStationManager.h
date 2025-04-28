// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"
#include "../AdditionalActor/MCaptureStation.h"
#include "../Managers/MBaseManager.h"

#include "MStationManager.generated.h"

UCLASS()
class MULTIPLAYER_API AMStationManager : public AMBaseManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TSubclassOf<AMCaptureStation> SpawnStationStatic;

private:
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TArray<AActor*> CaptureStationArray;

	AMCaptureStation* CaptureStation;

	UPROPERTY(meta = (AllowPrivateAccess = true))
	int32 CaptureStationCount = -1;

	int32 CountOfStation = 0;
public:	
	// Sets default values for this actor's properties
	AMStationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSetCaptureStation(int32 CountOfPlayer);

	UFUNCTION()
	void OnTimeFinished(ETypeOfTimer TypeOfFinishTimer);

	void FindCaptureStationArray();

	void CheckCountOfCaptureStations();

	void SpawnStation();

	void OnStationCaptured();
};
