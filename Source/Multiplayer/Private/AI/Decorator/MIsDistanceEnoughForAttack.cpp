// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/MIsDistanceEnoughForAttack.h"
#include "../../../Public/Character/MPlayerCharacter.h"

bool UMIsDistanceEnoughForAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bValue = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	AMAICharacter* OwnerCharacter = Cast<AMAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AMPlayerCharacter* DetectPlayer = Cast<AMPlayerCharacter>(BlackboardComp->GetValueAsObject("DetectPlayer"));

	if (OwnerCharacter && DetectPlayer)
	{
		float Distance = FVector::DistXY(OwnerCharacter->GetActorLocation(), DetectPlayer->GetActorLocation()) -
			OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius() + DetectPlayer->GetCapsuleComponent()->GetUnscaledCapsuleRadius();

		return (Distance <= MaxDistance && Distance >= MinDistance);
	}

	return bValue;
}
