// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MBerserkAbility.h"
#include "../../../../Public/Character/MBaseCharacter.h"

UMBerserkAbility::UMBerserkAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::Berserk, EGDAbilityAttackType::None, EGDAbilityType::Buff);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Berserk")));
}

void UMBerserkAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		Character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);
		Character->ChangeBuffValues(true, PercentageIncrease);

		StartTimer();
	}
}

void UMBerserkAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		Character->ChangeBuffValues(false, PercentageIncrease);
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
