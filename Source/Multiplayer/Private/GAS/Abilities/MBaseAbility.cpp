// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/MBaseAbility.h"
#include "../../../Public/Character/MBaseCharacter.h"

FAbilityMontageStruct UMBaseAbility::GetRandomMontageStruct()
{
	TArray<int> probabilityArray;

	if (AbilityMontageArray.Num() > 1)
	{
		int randomValue = rand() % 100 + 1;
		for (int i = 0; i < AbilityMontageArray.Num(); i++)
		{
			probabilityArray.Add(probabilityArray.Num() > 0 ? (probabilityArray.Last() + AbilityMontageArray[i].Probability) : AbilityMontageArray[i].Probability);
		}

		for (int i = 0; i < probabilityArray.Num(); i++)
		{
			if (randomValue < probabilityArray[i] + 1)
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
		FAbilityMontageStruct montageStruct = GetRandomMontageStruct();
		if (montageStruct.GetMontage())
		{
			if (UAnimInstance* animInstance = (Character->GetMesh()) ? Character->GetMesh()->GetAnimInstance() : nullptr)
			{
				animInstance->Montage_JumpToSection(montageStruct.GetSectionName(), montageStruct.GetMontage());
				animInstance->OnMontageEnded.AddDynamic(this, &UMBaseAbility::OnMontageEnded);
				Character->MulticastPlayMontage(montageStruct);
			}
		}
	}
}

void UMBaseAbility::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AbilityTimer, this, &UMBaseAbility::OnTimer, TimerInfo.TimerInterval, TimerInfo.bLoopTimer);
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

