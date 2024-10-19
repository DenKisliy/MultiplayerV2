// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Physical/MHandAttackAbility.h"

UMHandAttackAbility::UMHandAttackAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::HandAttack, EGDAbilityAttackType::Physical, EGDAbilityType::Attack);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.HandAttack")));
}

void UMHandAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AMCharacter* character = Cast<AMCharacter>(ActorInfo->AvatarActor.Get()))
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
	if (AMCharacter* characterAttack = Cast<AMCharacter>(ActorWhichAttack))
	{
		FGameplayEffectContextHandle EffectContext = characterAttack->GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = characterAttack->GetAbilitySystemComponent()->MakeOutgoingSpec(GetAbilityGameplayEffect(), 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
			if (Spec != nullptr)
			{
				float physicalBuff = characterAttack->Attributes->GetPhysicalBuff();
				float magnitudeValue = physicalBuff > 0 ? (physicalBuff / 100 + 1) * AbilityGameplayEffectInfo.Magnitude : AbilityGameplayEffectInfo.Magnitude;
				Spec->SetSetByCallerMagnitude(AbilityGameplayEffectInfo.Tag, magnitudeValue);
			}

			if (AMCharacter* characterDamaged = Cast<AMCharacter>(ActorWhichDamaged))
			{
				characterDamaged->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}