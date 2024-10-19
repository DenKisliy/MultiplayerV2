// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/MIsDistanceEnoughForAttack.h"

bool UMIsDistanceEnoughForAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bValue = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	const UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
	if (!blackboardComp)
	{
		return false;
	}

	if (AMAIController* controller = Cast<AMAIController>(OwnerComp.GetAIOwner()))
	{
		AMAIBaseCharacter* ownerCharacter = Cast<AMAIBaseCharacter>(controller->GetPawn());
		AMBaseCharacter* detectPlayer = Cast<AMBaseCharacter>(blackboardComp->GetValueAsObject("DetectPlayer"));
		
		if (ownerCharacter && detectPlayer)
		{
			float dist = FVector::DistXY(ownerCharacter->GetActorLocation(), detectPlayer->GetActorLocation()) - 
				ownerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius() + detectPlayer->GetCapsuleComponent()->GetUnscaledCapsuleRadius();

			bValue = (dist <= MaxDistance && dist >= MinDistance);
		}
	}

	return bValue;
}
