// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/MIsDistanceEnoughForAttack.h"
#include "../../../Public/Character/MPlayerCharacter.h"

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
		AMAICharacter* OwnerCharacter = Cast<AMAICharacter>(controller->GetPawn());
		AMPlayerCharacter* DetectPlayer = Cast<AMPlayerCharacter>(blackboardComp->GetValueAsObject("DetectPlayer"));
		
		if (OwnerCharacter && DetectPlayer)
		{
			float dist = FVector::DistXY(OwnerCharacter->GetActorLocation(), DetectPlayer->GetActorLocation()) -
				OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius() + DetectPlayer->GetCapsuleComponent()->GetUnscaledCapsuleRadius();

			bValue = (dist <= MaxDistance && dist >= MinDistance);
		}
	}

	return bValue;
}
