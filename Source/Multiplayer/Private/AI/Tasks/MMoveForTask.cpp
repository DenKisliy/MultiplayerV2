// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MMoveForTask.h"
#include "../../../Public/Character/MPlayerCharacter.h"

UMMoveForTask::UMMoveForTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UMMoveForTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	if (AMAIController* BotController = Cast<AMAIController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
		{
			if (AMPlayerCharacter* DetectPlayer = Cast<AMPlayerCharacter>(Blackboard->GetValueAsObject("DetectPlayer")))
			{
				BotController->MoveToLocation(DetectPlayer->GetActorLocation());
				NodeResult = EBTNodeResult::Succeeded;
			}
		}
	}

	return NodeResult;
}
