// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicAttackActor/MWindBallActor.h"
#include "../../Public/Character/MPlayerCharacter.h"

// Sets default values
AMWindBallActor::AMWindBallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
	MovementComponent->InitialSpeed = 500.0f;
	MovementComponent->MaxSpeed = 600.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = true;
	MovementComponent->Bounciness = 0.3f;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->bAutoActivate = false;

	WindBallParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WindBallParticle"));
	WindBallParticleComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionComponent->OnComponentHit.AddDynamic(this, &AMWindBallActor::OnHit);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AMWindBallActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OwnerActor != OtherActor)
	{
		GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);

		if (AMPlayerCharacter* HitActor = Cast<AMPlayerCharacter>(OtherActor))
		{
			HitActor->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(DamageGameplayEffect, 1.0f, HitActor->GetAbilitySystemComponent()->MakeEffectContext());
		}

		Destroy();
	}
}

void AMWindBallActor::SetData(FAbilitySpawnActorStruct Data)
{
	DamageGameplayEffect = Data.GameplayEffectForSpawnActor.GetDefaultObject();
	TimeToDestroy = Data.TimerInterval;
	OwnerActor = Data.OwnerActor;

	StartMove();
}

void AMWindBallActor::StartMove()
{
	MovementComponent->Activate();

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, [this]()
		{
			Destroy();
		}, TimeToDestroy, false);
}
