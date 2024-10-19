// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MMoveForTask.h"

UMMoveForTask::UMMoveForTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UMMoveForTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type nodeResult = EBTNodeResult::Failed;

	if (AMAIController* botController = Cast<AMAIController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent())
		{
			if (AMBaseCharacter* detectPlayer = Cast<AMBaseCharacter>(blackboard->GetValueAsObject("DetectPlayer")))
			{
				botController->MoveToLocation(detectPlayer->GetActorLocation());
				nodeResult = EBTNodeResult::Succeeded;
			}
		}
	}

	return nodeResult;
}
