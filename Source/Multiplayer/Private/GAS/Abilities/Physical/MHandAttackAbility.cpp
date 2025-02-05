// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Physical/MHandAttackAbility.h"
#include "../../../../Public/Character/MBaseCharacter.h"

UMHandAttackAbility::UMHandAttackAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::HandAttack, EGDAbilityAttackType::Physical, EGDAbilityType::Attack);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.HandAttack")));
}

void UMHandAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AMBaseCharacter* Character = Cast<AMBaseCharacter>(ActorInfo->AvatarActor.Get()))
	{
		PlayMontage();
	}
}

bool UMHandAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UMHandAttackAbility::CalculateEffect(AActor* ActorWhichAttack, AActor* ActorWhichDamaged)
{
	if (AMBaseCharacter* CharacterAttack = Cast<AMBaseCharacter>(ActorWhichAttack))
	{
		FGameplayEffectContextHandle EffectContext = CharacterAttack->GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = CharacterAttack->GetAbilitySystemComponent()->MakeOutgoingSpec(GetAbilityGameplayEffect(), 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
			if (Spec != nullptr)
			{
				float MagnitudeValue = CharacterAttack->Attributes->GetPhysicalBuff() > 0 ? 
					(CharacterAttack->Attributes->GetPhysicalBuff() / 100 + 1) * AbilityGameplayEffectInfo.Magnitude : AbilityGameplayEffectInfo.Magnitude;
				Spec->SetSetByCallerMagnitude(AbilityGameplayEffectInfo.Tag, MagnitudeValue);
			}

			if (AMBaseCharacter* CharacterDamaged = Cast<AMBaseCharacter>(ActorWhichDamaged))
			{
				CharacterDamaged->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}