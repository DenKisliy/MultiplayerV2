// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MSetNextPatrolPointTask.h"

UMSetNextPatrolPointTask::UMSetNextPatrolPointTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UMSetNextPatrolPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	if (AMAIController* BotController = Cast<AMAIController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
		{
			if (USplineComponent* Path = BotController->GetOwnerPath())
			{
				FVector tangentVector = Path->FindTangentClosestToWorldLocation(BotController->GetOwnerLocation(), ESplineCoordinateSpace::World);
				tangentVector.Normalize();

				BotController->MoveToLocation(Path->FindLocationClosestToWorldLocation(
					(tangentVector * (BotController->GetOwnerSpeed())) + BotController->GetOwnerLocation(),
					ESplineCoordinateSpace::World));
				NodeResult = EBTNodeResult::Succeeded;
			}
		}
	}

	return NodeResult;
}
