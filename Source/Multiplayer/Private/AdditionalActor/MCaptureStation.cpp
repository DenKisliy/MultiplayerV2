// Fill out your copyright notice in the Description page of Project Settings.


#include "AdditionalActor/MCaptureStation.h"

// Sets default values
AMCaptureStation::AMCaptureStation()
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

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMCaptureStation::OnUpdatedComponentOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMCaptureStation::OnUpdatedComponentOverlapEnd);

	PlatformComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformComponent"));
	PlatformComponent->SetupAttachment(CollisionComponent);

	StatueComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatueComponent"));
	StatueComponent->SetupAttachment(CollisionComponent);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);

	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifierComponent"));
	NavModifierComponent->SetIsReplicated(true);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AMCaptureStation::BeginPlay()
{
	Super::BeginPlay();

	if (PlatformComponent->GetStaticMesh())
	{
		CollisionComponent->SetSphereRadius(PlatformComponent->GetStaticMesh()->GetBounds().SphereRadius);
		NavModifierComponent->FailsafeExtent = FVector(PlatformComponent->GetStaticMesh()->GetBounds().SphereRadius);
	}
}

void AMCaptureStation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMCaptureStation, NewStatueMesh);
}

void AMCaptureStation::OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMPlayerCharacter* HitActor = Cast<AMPlayerCharacter>(Other))
	{
		if (PlayersNamesArray.Find(HitActor->GetPlayerName()) == INDEX_NONE)
		{
			if (!bCapture)
			{
				HitActor->InsertInSaveZoneDelegate.Broadcast(HitActor);
				UpdatePlayerStateAfterCapture(true, Other);
			}
			else
			{
				UpdateCaptureStateOfPlayer(HitActor->GetPlayerName(), true);
			}
		}
	}
}

void AMCaptureStation::OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMPlayerCharacter* HitActor = Cast<AMPlayerCharacter>(Other))
	{
		if (!bCapture)
		{
			UpdatePlayerStateAfterCapture(false, Other);
		}
		else
		{
			UpdateCaptureStateOfPlayer(HitActor->GetPlayerName(), false);
		}
	}
}

void AMCaptureStation::UpdateCaptureStateOfPlayer(FString Name, bool bCameIn)
{
	if (bCameIn)
	{
		PlayersNamesArray.Add(Name);
	}
	else
	{
		PlayersNamesArray.Remove(Name);
	}

	ChangeCountOfCapturePlayerDelegate.Broadcast(PlayersNamesArray.Num() > 0 ? PlayersNamesArray.Num() : 0);
}

void AMCaptureStation::UpdatePlayerStateAfterCapture(bool bCameIn, AActor* Player)
{
	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(Player))
	{
		if (AMPlayerState* PlayerState = Cast<AMPlayerState>(Character->GetPlayerState()))
		{
			PlayerState->SetPlayerInSaveZone(bCameIn);
			ActivateGameplayEffectForCharacter(Player, bCameIn);
		}
	}
}

void AMCaptureStation::ActivateGameplayEffectForCharacter(AActor* Player, bool bValue)
{
	if (AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(Player))
	{
		if (IsValid(GameplayEffect))
		{
			if (bValue)
			{
				FGameplayEffectContextHandle EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				FGameplayEffectSpecHandle SpecHandle = Character->GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
				if (SpecHandle.IsValid())
				{
					Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
			else
			{
				Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(GameplayEffect, Character->GetAbilitySystemComponent());
			}
		}
	}
}

void AMCaptureStation::OnRep_NewStatueMesh()
{
	StatueComponent->SetStaticMesh(NewStatueMesh);
}

void AMCaptureStation::CaptureFinish()
{
	bCapture = false;
	if (IsValid(RestoredStatueMesh))
	{
		NewStatueMesh = RestoredStatueMesh;

		NavModifierComponent->SetAreaClass(UNavArea_Null::StaticClass());
		StatueComponent->SetStaticMesh(NewStatueMesh);

		AddItems();
	}
}

void AMCaptureStation::AddItems_Implementation()
{
	if (IsValid(GetWorld()))
	{
		if (IsValid(GetWorld()->GetGameState()))
		{
			for (auto Player : GetWorld()->GetGameState()->PlayerArray)
			{
				for (FString PlayerName : PlayersNamesArray)
				{
					if (Player.Get()->GetPlayerName() == PlayerName)
					{
						if (AMPlayerController* PlayerController = Cast<AMPlayerController>(Player.Get()->GetPlayerController()))
						{
							for (FItemTypeInfo Item : RewardItemsArray)
							{
								PlayerController->AddItemToCharacter(Item);
							}
						}
					}
				}
			}

			PlayersNamesArray.Empty();
		}
	}
}
