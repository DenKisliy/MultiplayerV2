// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Character/MPlayerCharacter.h"
#include "../Data/MAbilityDataStruct.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"

#include "MHealingAreaActor.generated.h"

UCLASS()
class MULTIPLAYER_API AMHealingAreaActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;

private:
	FTimerHandle DestroyTimer;

	UGameplayEffect* GameplayEffect;

	TArray<AMPlayerCharacter*> HealActor;

public:	
	// Sets default values for this actor's properties
	AMHealingAreaActor();

	void SetData(FAbilitySpawnActorStruct Data);

private:
	UFUNCTION()
	void OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetEffectAfterSetData();

protected:
	virtual void BeginPlay() override;
};
