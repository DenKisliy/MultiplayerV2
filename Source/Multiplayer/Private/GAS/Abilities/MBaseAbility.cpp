// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/MBaseAbility.h"
#include "../../../Public/Character/MBaseCharacter.h"

FAbilityMontageStruct UMBaseAbility::GetRandomMontageStruct()
{
	TArray<int> ProbabilityArray;

	if (AbilityMontageArray.Num() > 1)
	{
		int RandomValue = rand() % 100 + 1;
		for (int i = 0; i < AbilityMontageArray.Num(); i++)
		{
			ProbabilityArray.Add(ProbabilityArray.Num() > 0 ? (ProbabilityArray.Last() + AbilityMontageArray[i].Probability) : AbilityMontageArray[i].Probability);
		}

		for (int i = 0; i < ProbabilityArray.Num(); i++)
		{
			if (RandomValue < ProbabilityArray[i] + 1)
			{
				return AbilityMontageArray[i];
			}
		}
	}
	else if (AbilityMontageArray.Num() == 1)
	{
		return AbilityMontageArray[0];
	}

	return FAbilityMontageStruct();
}

void UMBaseAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	if (TimerInfo.TimerInterval == 0)
	{
		if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		}
	}
}

bool UMBaseAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (TimerInfo.CostOfActivateAbilityMap.Num() > 0 && TimerInfo.TimerInterval > 0)
	{
		if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(ActorInfo->AvatarActor.Get()))
		{
			return Character->IsEnoughAttributesValues(TimerInfo.CostOfActivateAbilityMap);
		}
	}

	return true;
}

void UMBaseAbility::PlayMontage()
{
	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		FAbilityMontageStruct MontageStruct = GetRandomMontageStruct();
		if (MontageStruct.GetMontage())
		{
			if (UAnimInstance* AnimInstance = (Character->GetMesh()) ? Character->GetMesh()->GetAnimInstance() : nullptr)
			{
				AnimInstance->Montage_JumpToSection(MontageStruct.GetSectionName(), MontageStruct.GetMontage());
				AnimInstance->OnMontageEnded.AddDynamic(this, &UMBaseAbility::OnMontageEnded);
				Character->MulticastPlayMontage(MontageStruct);
			}
		}
	}
}

void UMBaseAbility::StartTimer()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().SetTimer(AbilityTimer, this, &UMBaseAbility::OnTimer, TimerInfo.TimerInterval, TimerInfo.bLoopTimer);
	}
}

void UMBaseAbility::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UMBaseAbility::OnTimer()
{

	if (IsValid(GetWorld()))
	{
		if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
		{
			if (TimerInfo.bLoopTimer)
			{
				if (Character->IsEnoughAttributesValues(TimerInfo.CostOfActivateAbilityMap))
				{
					Character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);
				}
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(AbilityTimer);
				CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
			}
		}
	}
}

