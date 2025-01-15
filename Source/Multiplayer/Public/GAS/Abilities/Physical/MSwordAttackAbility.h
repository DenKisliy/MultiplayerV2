// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/MBaseAbility.h"

#include "MSwordAttackAbility.generated.h"

/**
 * 
 */

UCLASS()
class MULTIPLAYER_API UMSwordAttackAbility : public UMBaseAbility
{
	GENERATED_BODY()

public:
	UMSwordAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void CalculateEffect(AActor* ActorWhichAttack, AActor* ActorWhichDamaged) override;
};
