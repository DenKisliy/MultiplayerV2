// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MWindBallAbility.h"

UMWindBallAbility::UMWindBallAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::WindBall, EGDAbilityAttackType::Magic, EGDAbilityType::Attack);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.WindBall")));
}

void UMWindBallAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (AMBaseCharacter* character = Cast<AMBaseCharacter>(ActorInfo->AvatarActor.Get()))
	{
		character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);

		SpawnActor();

		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UMWindBallAbility::SpawnActor()
{
	if (AMBaseCharacter* character = Cast<AMBaseCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		AMWindBallActor* windBall = GetWorld()->SpawnActor<AMWindBallActor>(SpawnActorStruct.SpawnActor, character->GetSpawnActorLocation(),
			character->GetSpawnActorRotation(), FActorSpawnParameters());

		SpawnActorStruct.OwnerActor = GetCurrentActorInfo()->AvatarActor.Get();
		SpawnActorStruct.GameplayEffectForSpawnActor = GetAbilityGameplayEffect();
		windBall->SetData(SpawnActorStruct);
	}
}
