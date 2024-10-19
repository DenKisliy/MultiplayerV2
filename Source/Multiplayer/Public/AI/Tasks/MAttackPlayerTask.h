// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "Math/UnrealMathUtility.h"
#include "../Controller/MAIController.h"

#include "MAttackPlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMAttackPlayerTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Tag, EditAnywhere)
	FGameplayTagContainer AbilityTags;

private:
	UBehaviorTreeComponent* BehaviorTreeComponent;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void OnEndAbility();
};
