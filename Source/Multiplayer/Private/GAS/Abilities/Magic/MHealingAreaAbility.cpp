// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MHealingAreaAbility.h"

UMHealingAreaAbility::UMHealingAreaAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::HealingArea, EGDAbilityAttackType::None, EGDAbilityType::Healing);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.HealingArea")));
}

void UMHealingAreaAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AMBaseCharacter* character = Cast<AMBaseCharacter>(ActorInfo->AvatarActor.Get()))
	{
		character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);

		SpawnActor();

		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

}

void UMHealingAreaAbility::SpawnActor()
{
	if (AMBaseCharacter* character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		AMHealingAreaActor* healingArea = GetWorld()->SpawnActor<AMHealingAreaActor>(SpawnActorStruct.SpawnActor, character->GetMesh()->GetComponentLocation(),
			character->GetSpawnActorRotation(), FActorSpawnParameters());

		SpawnActorStruct.GameplayEffectForSpawnActor = GetAbilityGameplayEffect();
		healingArea->SetData(SpawnActorStruct);
	}
}
