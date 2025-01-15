// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "../GAS/Attribute/MAttributeSet.h"
#include "../GAS/Abilities/MBaseAbility.h"

#include "../Data/MCharacterDataStruct.h"

#include "MBaseCharacter.generated.h"

UCLASS()
class MULTIPLAYER_API AMBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UMAttributeSet* Attributes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<UMBaseAbility>> DefaultAbilities;

	FEndAbility EndAbilityDelegate;

protected:
	FSpeedInfoStruct SpeedInfo;

public:
	// Sets default values for this character's properties
	AMBaseCharacter();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayMontage(FAbilityMontageStruct MontageStruct);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	void SetCharacterSpeed(float Coefficient);

	bool IsEnoughAttributesValues(TMap<EAttributeToActivateType, float> AbilityCostMap);

	void ChangeAttributesValues(TMap<EAttributeToActivateType, float> AbilityCostMap);

	void ChangeBuffValues(bool bActiv, float BuffValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeAttributes();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

private:
	void InitPlayer();

	void InitializeAbilities();
};
