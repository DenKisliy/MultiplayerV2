// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MSetNextPatrolPointTask.h"

UMSetNextPatrolPointTask::UMSetNextPatrolPointTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UMSetNextPatrolPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type nodeResult = EBTNodeResult::Failed;

	if (AMAIController* botController = Cast<AMAIController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent())
		{
			if (USplineComponent* path = botController->GetOwnerPath())
			{
				FVector tangentVector = path->FindTangentClosestToWorldLocation(botController->GetOwnerLocation(), ESplineCoordinateSpace::World);
				tangentVector.Normalize();

				botController->MoveToLocation(path->FindLocationClosestToWorldLocation(
					(tangentVector * (botController->GetOwnerSpeed())) + botController->GetOwnerLocation(),
					ESplineCoordinateSpace::World));
				nodeResult = EBTNodeResult::Succeeded;
			}
		}
	}

	return nodeResult;
}
