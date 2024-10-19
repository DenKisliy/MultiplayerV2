// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/MBaseAbility.h"

#include "../../../MagicAttackActor/MWindBallActor.h"
#include "../../../GameFramework/MCharacter.h"

#include "MWindBallAbility.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMWindBallAbility : public UMBaseAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	FAbilitySpawnActorStruct SpawnActorStruct;

public:
	UMWindBallAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void SpawnActor();
};
