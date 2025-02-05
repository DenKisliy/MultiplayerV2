// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/MIsNotReachPointDecorator.h"

bool UMIsNotReachPointDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bValue = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		int pointIndex = BlackboardComp->GetValueAsInt("PointIndex") == 0 ? 1 : BlackboardComp->GetValueAsInt("PointIndex");
		if (AMAIController* Controller = Cast<AMAIController>(OwnerComp.GetAIOwner()))
		{
			bValue = FVector::DistXY(Controller->GetOwnerLocation(), Controller->GetOwnerPath()->GetLocationAtSplinePoint(pointIndex, ESplineCoordinateSpace::World)) < Distance;
			if (bValue)
			{
				BlackboardComp->SetValueAsInt(FName("PointIndex"), pointIndex + 1 > Controller->GetOwnerPath()->GetNumberOfSplinePoints() ? 1 : pointIndex + 1);
				Controller->StopMovement();
			}
		}
	}

	return bValue;
}
