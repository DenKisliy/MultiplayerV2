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
	if (AMBaseCharacter* hitActor = Cast<AMBaseCharacter>(Other))
	{
		if (HealActor.Find(hitActor) == INDEX_NONE)
		{
			HealActor.Add(hitActor);
			if (IsValid(GameplayEffect))
			{
				hitActor->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, hitActor->GetAbilitySystemComponent()->MakeEffectContext());
			}
		}
	}
}

void AMHealingAreaActor::OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMBaseCharacter* hitActor = Cast<AMBaseCharacter>(Other))
	{
		if (HealActor.Find(hitActor) != INDEX_NONE)
		{
			HealActor.Remove(hitActor);
		}
	}
}

void AMHealingAreaActor::SetEffectAfterSetData()
{
	for (AMBaseCharacter* actor : HealActor)
	{
		actor->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, actor->GetAbilitySystemComponent()->MakeEffectContext());
	}
}

