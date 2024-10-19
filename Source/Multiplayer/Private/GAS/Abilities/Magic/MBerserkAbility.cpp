// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MBerserkAbility.h"

UMBerserkAbility::UMBerserkAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::Berserk, EGDAbilityAttackType::None, EGDAbilityType::Buff);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Berserk")));
}

void UMBerserkAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (AMCharacter* character = Cast<AMCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);
		character->ChangeBuffValues(true, PercentageIncrease);

		StartTimer();
	}
}

void UMBerserkAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (AMCharacter* character = Cast<AMCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		character->ChangeBuffValues(false, PercentageIncrease);
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
