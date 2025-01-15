// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MBaseManager.generated.h"

UCLASS()
class MULTIPLAYER_API AMBaseManager : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	bool bFinish = false;

public:	
	// Sets default values for this actor's properties
	AMBaseManager();

	bool IsFinishBind();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetFinishBind();
};
