// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Magic/MWindBallAbility.h"
#include "../../../../Public/Character/MPlayerCharacter.h"

UMWindBallAbility::UMWindBallAbility()
{
	AbilityInfo = FAbilityInfoStruct(EGDAbilityInputID::WindBall, EGDAbilityAttackType::Magic, EGDAbilityType::Attack);
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.WindBall")));
}

void UMWindBallAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->ChangeAttributesValues(TimerInfo.CostOfActivateAbilityMap);

		SpawnActor();

		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UMWindBallAbility::SpawnActor()
{
	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		AMWindBallActor* WindBall = GetWorld()->SpawnActor<AMWindBallActor>(SpawnActorStruct.SpawnActor, Character->GetSpawnActorLocation(),
			Character->GetSpawnActorRotation(), FActorSpawnParameters());

		SpawnActorStruct.OwnerActor = GetCurrentActorInfo()->AvatarActor.Get();
		SpawnActorStruct.GameplayEffectForSpawnActor = GetAbilityGameplayEffect();
		WindBall->SetData(SpawnActorStruct);
	}
}
