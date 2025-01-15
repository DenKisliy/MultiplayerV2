// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "../Data/MAbilityDataStruct.h"

#include "MWindBallActor.generated.h"

UCLASS()
class MULTIPLAYER_API AMWindBallActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* WindBallParticleComponent;

private:
	FTimerHandle DestroyTimer;

	UGameplayEffect* DamageGameplayEffect;

	AActor* OwnerActor;

	int TimeToDestroy;

public:	
	// Sets default values for this actor's properties
	AMWindBallActor();

	void SetData(FAbilitySpawnActorStruct Data);

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void StartMove();
};
