// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/MAccelerationAbility.h"
#include "../../../Public/Character/MBaseCharacter.h"

UMAccelerationAbility::UMAccelerationAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::Acceleration, EGDAbilityAttackType::None, EGDAbilityType::Buff);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Acceleration")));
}

void UMAccelerationAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsValid(GetWorld()))
	{
		if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(ActorInfo->AvatarActor.Get()))
		{
			ActivateAbilityForCharacter();
			SetSpeedForCharacter(true);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMAccelerationAbility::TimerActiveAbility, TimerInterval, true);
		}
	}
}

void UMAccelerationAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (IsValid(GetWorld()))
	{
		if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
		{
			SetSpeedForCharacter(false);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		}
	}
}

void UMAccelerationAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

bool UMAccelerationAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return IsEnoughStamina(ActorInfo);
}

void UMAccelerationAbility::TimerActiveAbility()
{
	if (IsValid(GetWorld()))
	{
		if (!IsEnoughStamina(CurrentActorInfo))
		{
			SetSpeedForCharacter(false);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		}
		else
		{
			ActivateAbilityForCharacter();
		}
	}
}

bool UMAccelerationAbility::IsEnoughStamina(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (Character->Attributes)
		{
			return Character->Attributes->GetStamina() >= StaminaCostOfEachSecond;
		}
	}

	return false;
}

void UMAccelerationAbility::ActivateAbilityForCharacter()
{
	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		float staminaValue = Character->Attributes->GetStamina();

		Character->Attributes->SetStamina(staminaValue - StaminaCostOfEachSecond);
	}
}

void UMAccelerationAbility::SetSpeedForCharacter(bool bAcceleration)
{
	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		Character->SetCharacterSpeed(bAcceleration ? AccelerationCoefficientSpeed : -1);
	}
}
