// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MReduceAbility.h"
#include "../../../../Public/Character/MPlayerCharacter.h"

UMReduceAbility::UMReduceAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::Reduce, EGDAbilityAttackType::None, EGDAbilityType::Buff);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Reduce")));
}

void UMReduceAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		Character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);
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
	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		Character->SetPlayerScale(bScale ? FVector(ReductionFactor) : FVector(1));
	}
}
