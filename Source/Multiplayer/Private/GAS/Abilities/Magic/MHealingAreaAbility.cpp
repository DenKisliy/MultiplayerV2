// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MHealingAreaAbility.h"
#include "../../../../Public/Character/MPlayerCharacter.h"
#include "../../../../Public/MagicActor/MHealingAreaActor.h"

UMHealingAreaAbility::UMHealingAreaAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::HealingArea, EGDAbilityAttackType::None, EGDAbilityType::Healing);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.HealingArea")));
}

void UMHealingAreaAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);

		SpawnActor();

		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

}

void UMHealingAreaAbility::SpawnActor()
{
	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		AMHealingAreaActor* HealingArea = GetWorld()->SpawnActor<AMHealingAreaActor>(SpawnActorStruct.SpawnActor, Character->GetMesh()->GetComponentLocation(),
			Character->GetSpawnActorRotation(), FActorSpawnParameters());

		SpawnActorStruct.GameplayEffectForSpawnActor = GetAbilityGameplayEffect();
		HealingArea->SetData(SpawnActorStruct);
	}
}
