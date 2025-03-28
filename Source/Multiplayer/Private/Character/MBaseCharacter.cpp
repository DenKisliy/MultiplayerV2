// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Character/MBaseCharacter.h"

// Sets default values
AMBaseCharacter::AMBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Attributes = CreateDefaultSubobject<UMAttributeSet>(TEXT("Attributes"));
	
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AMBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpeedInfo.SetDefaultSpeed(GetCharacterMovement()->MaxWalkSpeed);
}

void AMBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitPlayer();
}

void AMBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitPlayer();
}

void AMBaseCharacter::InitPlayer()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		InitializeAbilities();
	}
}

void AMBaseCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && StartupEffects.Num() > 0)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void AMBaseCharacter::InitializeAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UMBaseAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this));
		}
	}
}

void AMBaseCharacter::MulticastPlayMontage_Implementation(FAbilityMontageStruct MontageStruct)
{
	PlayAnimMontage(MontageStruct.GetMontage(), 1, MontageStruct.GetSectionName());
}

void AMBaseCharacter::SetCharacterSpeed(float Coefficient)
{
	GetCharacterMovement()->MaxWalkSpeed = Coefficient == -1 ? SpeedInfo.GetDefaultSpeed() : SpeedInfo.GetNewSpeed(Coefficient);
}

bool AMBaseCharacter::IsEnoughAttributesValues(TMap<EAttributeToActivateType, float> AbilityCostMap)
{
	for (auto elem : AbilityCostMap)
	{
		if (!Attributes->IsEnoughValueOfAttributes(elem.Key, elem.Value))
		{
			return false;
		}
	}

	return true;
}

void AMBaseCharacter::ChangeAttributesValues(TMap<EAttributeToActivateType, float> AbilityCostMap)
{
	for (auto elem : AbilityCostMap)
	{
		Attributes->ChangeValueOfAttributes(elem.Key, elem.Value);
	}
}

void AMBaseCharacter::ChangeBuffValues(bool bActiv, float BuffValue)
{
	Attributes->ChangeBuffValue(bActiv, BuffValue);
}

