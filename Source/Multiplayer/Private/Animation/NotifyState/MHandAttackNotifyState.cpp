// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/MHandAttackNotifyState.h"
#include "../../../Public/Character/MPlayerCharacter.h"
#include "../../../Public/GAS/Abilities/MBaseAbility.h"

void UMHandAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	bHitByCharacter = false;
}

void UMHandAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!bHitByCharacter)
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(MeshComp->GetOwner());

		FHitResult HitLeftResult;
		FHitResult HitRightResult;

		bool bHitRight = UKismetSystemLibrary::SphereTraceSingle(MeshComp, MeshComp->GetSocketLocation("ik_hand_r_center"), MeshComp->GetSocketLocation("ik_hand_r"),
			12.f, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::None, HitRightResult, true, FLinearColor::Gray, FLinearColor::Blue, 1.f);

		bool bHitLeft = UKismetSystemLibrary::SphereTraceSingle(MeshComp, MeshComp->GetSocketLocation("ik_hand_l_center"), MeshComp->GetSocketLocation("ik_hand_l"),
			12.f, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::None, HitLeftResult, true, FLinearColor::Gray, FLinearColor::Blue, 1.f);

		if (bHitRight || bHitLeft)
		{
			if (AMPlayerCharacter* PlayerWhichAttack = Cast<AMPlayerCharacter>(MeshComp->GetOwner()))
			{
				bHitByCharacter = true;
				TSubclassOf<UMBaseAbility>* Ability = PlayerWhichAttack->DefaultAbilities.FindByPredicate([](const TSubclassOf<UMBaseAbility>& InItem) {
					return InItem.GetDefaultObject()->GetAbilityInputID() == EGDAbilityInputID::HandAttack;
					});

				if (Ability)
				{
					Ability->GetDefaultObject()->CalculateEffect(PlayerWhichAttack, bHitRight ? HitRightResult.GetActor() : HitLeftResult.GetActor());
				}
			}
		}
	}
}