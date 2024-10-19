// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "../../Data/MAbilityDataStruct.h"

#include "MBaseAbility.generated.h"

/**
 * 
 */

UCLASS()
class MULTIPLAYER_API UMBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Ability)
	FAbilityInfoStruct AbilityInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	FAbilityGameplayEffectInfoStruct AbilityGameplayEffectInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<FAbilityMontageStruct> AbilityMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	FAbilityTimerStruct TimerInfo;

private:
	FTimerHandle AbilityTimer;

public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	TSubclassOf<UGameplayEffect> GetAbilityGameplayEffect() { return AbilityGameplayEffectInfo.GameplayEffect; }

	EGDAbilityInputID GetAbilityInputID() { return AbilityInfo.InputID; }

	virtual void CalculateEffect(AActor* ActorWhichAttack, AActor* ActorWhichDamaged) {};

protected:
	void PlayMontage();

	void StartTimer();

private:
	FAbilityMontageStruct GetRandomMontageStruct();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnTimer();


};
