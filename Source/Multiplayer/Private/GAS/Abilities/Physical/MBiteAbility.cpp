// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Physical/MBiteAbility.h"
#include "../../../../Public/Character/MBaseCharacter.h"

UMBiteAbility::UMBiteAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::Bite, EGDAbilityAttackType::Physical, EGDAbilityType::Attack);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Bite")));
}

void UMBiteAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(ActorInfo->AvatarActor.Get()))
	{
		PlayMontage();
	}
}

void UMBiteAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		Character->EndAbilityDelegate.Broadcast();
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
