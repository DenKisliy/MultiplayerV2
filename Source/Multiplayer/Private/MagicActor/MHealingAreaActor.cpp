// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicActor/MHealingAreaActor.h"

// Sets default values
AMHealingAreaActor::AMHealingAreaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;
	RootComponent = CollisionComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMHealingAreaActor::OnUpdatedComponentOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMHealingAreaActor::OnUpdatedComponentOverlapEnd);

	SetReplicates(true);
}

void AMHealingAreaActor::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComponent->SetVariableFloat("Size", (CollisionComponent->GetScaledSphereRadius() + 100) * 2);
	NiagaraComponent->SetVariableVec3("Scale", FVector(CollisionComponent->GetScaledSphereRadius() / 2));
}

void AMHealingAreaActor::SetData(FAbilitySpawnActorStruct Data)
{
	GameplayEffect = Data.GameplayEffectForSpawnActor.GetDefaultObject();
	
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, [this]()
		{
			Destroy();
		}, Data.TimerInterval, false);

	SetEffectAfterSetData();
}

void AMHealingAreaActor::OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMPlayerCharacter* HitActor = Cast<AMPlayerCharacter>(Other))
	{
		if (HealActor.Find(HitActor) == INDEX_NONE)
		{
			HealActor.Add(HitActor);
			if (IsValid(GameplayEffect))
			{
				HitActor->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, HitActor->GetAbilitySystemComponent()->MakeEffectContext());
			}
		}
	}
}

void AMHealingAreaActor::OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMPlayerCharacter* HitActor = Cast<AMPlayerCharacter>(Other))
	{
		if (HealActor.Find(HitActor) != INDEX_NONE)
		{
			HealActor.Remove(HitActor);
		}
	}
}

void AMHealingAreaActor::SetEffectAfterSetData()
{
	for (AMPlayerCharacter* Actor : HealActor)
	{
		Actor->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, Actor->GetAbilitySystemComponent()->MakeEffectContext());
	}
}

