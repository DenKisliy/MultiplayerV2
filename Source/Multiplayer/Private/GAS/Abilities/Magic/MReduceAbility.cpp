// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MReduceAbility.h"

UMReduceAbility::UMReduceAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::Reduce, EGDAbilityAttackType::None, EGDAbilityType::Buff);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Reduce")));
}

void UMReduceAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AMCharacter* character = Cast<AMCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);
		SetScaleForCharacter(true);
		StartTimer();
	}
}

void UMReduceAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	SetScaleForCharacter(false);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMReduceAbility::SetScaleForCharacter(bool bScale)
{
	if (AMBaseCharacter* character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		character->SetPlayerScale(bScale ? FVector(ReductionFactor) : FVector(1));
	}
}
