// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/MIsNotReachPointDecorator.h"

bool UMIsNotReachPointDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bValue = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent())
	{
		int pointIndex = blackboardComp->GetValueAsInt("PointIndex") == 0 ? 1 : blackboardComp->GetValueAsInt("PointIndex");
		if (AMAIController* controller = Cast<AMAIController>(OwnerComp.GetAIOwner()))
		{
			bValue = FVector::DistXY(controller->GetOwnerLocation(), controller->GetOwnerPath()->GetLocationAtSplinePoint(pointIndex, ESplineCoordinateSpace::World)) < Distance;
			if (bValue)
			{
				blackboardComp->SetValueAsInt(FName("PointIndex"), pointIndex + 1 > controller->GetOwnerPath()->GetNumberOfSplinePoints() ? 1 : pointIndex + 1);
				controller->StopMovement();
			}
		}
	}

	return bValue;
}
