// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "../Controller/MAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MSetNextPatrolPointTask.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMSetNextPatrolPointTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMSetNextPatrolPointTask(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
