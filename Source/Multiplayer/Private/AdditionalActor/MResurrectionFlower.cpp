// Fill out your copyright notice in the Description page of Project Settings.


#include "AdditionalActor/MResurrectionFlower.h"

// Sets default values
AMResurrectionFlower::AMResurrectionFlower()
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

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SetReplicates(true);
}

void AMResurrectionFlower::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMResurrectionFlower::OnUpdatedComponentOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMResurrectionFlower::OnUpdatedComponentOverlapEnd);
}

void AMResurrectionFlower::OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DeathPlayer)
	{
		DeathPlayer = Cast<AMPlayerCharacter>(Other);
		if (DeathPlayer)
		{
			IsHaveResurrectionItemInInventory(DeathPlayer->GetPlayerName());
		}
		else
		{
			DeathPlayer = nullptr;
		}
	}
}

void AMResurrectionFlower::OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (DeathPlayer && IsValid(GetWorld()))
	{
		if (AMGameState* GameState = Cast<AMGameState>(GetWorld()->GetGameState()))
		{
			GameState->StopResurrectionTimer();
			GameState->AdditionalTimerDelegate.Clear();
		}
		DeathPlayer = nullptr;
	}
}

void AMResurrectionFlower::OnFinishTimer(ETypeOfAdditionalTimer TypeOfAdditionalTimer)
{
	if (AMPlayerController* PlayerController = Cast<AMPlayerController>(Controller))
	{
		RemoveResurrectionItemFromInventory(DeathPlayer->GetPlayerName());
		PlayerController->ResurrectionCharacter();
	}
}

void AMResurrectionFlower::IsHaveResurrectionItemInInventory_Implementation(const FString& PlayerName)
{
	if (IsValid(GetWorld()->GetGameState()))
	{
		for (auto Player : GetWorld()->GetGameState()->PlayerArray)
		{
			if (Player->GetPlayerName() == PlayerName)
			{
				if (AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Player->GetPlayerController()->GetPawn()))
				{
					if (PlayerCharacter->InventoryComponent->IsHaveItem(ItemForResurrectionInfo))
					{
						if (AMGameState* GameState = Cast<AMGameState>(GetWorld()->GetGameState()))
						{
							GameState->StartResurrectionTimer();
							GameState->AdditionalTimerDelegate.Clear();
							GameState->AdditionalTimerDelegate.AddDynamic(this, &AMResurrectionFlower::OnFinishTimer);
						}
					}
					else
					{
						DeathPlayer = nullptr;
					}
				}
			}
		}
	}
}

void  AMResurrectionFlower::RemoveResurrectionItemFromInventory_Implementation(const FString& PlayerName)
{
	if (IsValid(GetWorld()->GetGameState()))
	{
		for (auto Player : GetWorld()->GetGameState()->PlayerArray)
		{
			if (Player->GetPlayerName() == PlayerName)
			{
				if (AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Player->GetPlayerController()->GetPawn()))
				{
					PlayerCharacter->InventoryComponent->UseItem(ItemForResurrectionInfo);
				}
			}
		}
	}
}
