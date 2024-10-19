#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "MAbilityDataStruct.generated.h"

UENUM(BlueprintType)
enum class EGDAbilityAttackType : uint8
{
	None,
	Physical,
	Magic
};

UENUM(BlueprintType)
enum class EGDAbilityType : uint8
{
	None,
	Attack,
	Buff,
	Healing
};

UENUM(BlueprintType)
enum class EGDAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Acceleration,
	Reduce,
	SwordAttack,
	HandAttack,
	WindBall,
	Berserk,
	HealingArea,
	Bite,
	JumpBite
};

UENUM(BlueprintType)
enum class EAttributeToActivateType : uint8
{
	None,
	Health,
	Mana,
	Stamina
};

USTRUCT(BlueprintType)
struct FAbilityMontageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Probability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage;

	FAbilityMontageStruct()
	{
		Probability = 0;
		SectionName = "Default";
		Montage = nullptr;
	}

	UAnimMontage* GetMontage()
	{
		return Montage;
	}

	FName GetSectionName()
	{
		return SectionName;
	}
};

USTRUCT(BlueprintType)
struct FAbilityInfoStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EGDAbilityInputID InputID;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EGDAbilityAttackType AttackType;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EGDAbilityType Type;

	FAbilityInfoStruct()
	{
		InputID = EGDAbilityInputID::None;
		AttackType = EGDAbilityAttackType::None;
		Type = EGDAbilityType::None;
	}
	
	FAbilityInfoStruct(EGDAbilityInputID NewInputID, EGDAbilityAttackType NewAttackType, EGDAbilityType NewType)
	{
		InputID = NewInputID;
		AttackType = NewAttackType;
		Type = NewType;
	}

};

USTRUCT(BlueprintType)
struct FAbilityGameplayEffectInfoStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Magnitude;

	FAbilityGameplayEffectInfoStruct()
	{
		GameplayEffect = nullptr;
		Tag = FGameplayTag();
		Magnitude = 0.0f;
	}
};

USTRUCT(BlueprintType)
struct FAbilityTimerStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float TimerInterval;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bLoopTimer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<EAttributeToActivateType, float> CostOfActivateAbilityMap;

	FAbilityTimerStruct()
	{
		TimerInterval = 0;
		bLoopTimer = false;
	}
};

USTRUCT(BlueprintType)
struct FAbilitySpawnActorStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float TimerInterval;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> SpawnActor;

	TSubclassOf<UGameplayEffect> GameplayEffectForSpawnActor;

	AActor* OwnerActor;

	FAbilitySpawnActorStruct()
	{
		TimerInterval = 0;
		GameplayEffectForSpawnActor = nullptr;
		OwnerActor = nullptr;
	};
};