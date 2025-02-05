// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/MBiteNotifyState.h"
#include "../../../Public/Character/MPlayerCharacter.h"
#include "../../../Public/Character/MAICharacter.h"


void UMBiteNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	bBiteCharacter = false;
}

void UMBiteNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!bBiteCharacter)
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(MeshComp->GetOwner());

		FHitResult hitResult;

		bool bHit = UKismetSystemLibrary::SphereTraceSingle(MeshComp, MeshComp->GetSocketLocation("Jaw"), MeshComp->GetSocketLocation("Jaw"),
			20.f, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::None, hitResult, true, FLinearColor::Gray, FLinearColor::Blue, 1.f);

		if (bHit)
		{
			AMPlayerCharacter* CharacterWhichDamaged = Cast<AMPlayerCharacter>(hitResult.GetActor());
			AMAICharacter* PlayerWhichAttack = Cast<AMAICharacter>(MeshComp->GetOwner());

			if (CharacterWhichDamaged && PlayerWhichAttack)
			{
				bBiteCharacter = true;
				TSubclassOf<UMBaseAbility>* Ability = PlayerWhichAttack->DefaultAbilities.FindByPredicate([](const TSubclassOf<UMBaseAbility>& InItem) {
					return InItem.GetDefaultObject()->GetAbilityInputID() == EGDAbilityInputID::Bite;
					});

				if (Ability)
				{
					CharacterWhichDamaged->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
						Ability->GetDefaultObject()->GetAbilityGameplayEffect().GetDefaultObject(), 1.0f, PlayerWhichAttack->GetAbilitySystemComponent()->MakeEffectContext());
				}
			}
		}
	}
}
