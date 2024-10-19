// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "../Character/MAIBaseCharacter.h"
#include "../../Character/MBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MAIController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BTPatrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BTStalkingAndAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;

private:
	AMAIBaseCharacter* OwnerCharacter;

public:
	AMAIController(const FObjectInitializer& ObjectInitializer);
	
	void FindNextPoint(FVector& NextPoint, int& Index);

	virtual void Tick(float DeltaTime) override;

	FVector GetOwnerLocation();

	float GetOwnerSpeed();

	USplineComponent* GetOwnerPath();

private:
	void SetStartDate();

	UFUNCTION()
	void LaunchBehaviorTree();
	
	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	bool InitializeBlackboardComponent(UBehaviorTree* BehaviorTree);

	UFUNCTION()
	void OnDetectPlayerInSaveZone(AActor* Player);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
