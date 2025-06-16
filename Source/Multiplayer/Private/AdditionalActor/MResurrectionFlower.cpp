// Fill out your copyright notice in the Description page of Project Settings.


#include "AdditionalActor/MResurrectionFlower.h"
#include "../../Public/GameFramework/GameState/MMultiplayerGameState.h"

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
	if (IsValid(GetPlayerState()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMResurrectionFlower The Actor's name is %s"), *GetPlayerState()->GetPlayerName());
	}
	if (!HealerPlayer)
	{
		HealerPlayer = Cast<AMPlayerCharacter>(Other);
		if (HealerPlayer)
		{
			IsHaveResurrectionItemInInventory(HealerPlayer->GetPlayerName());
		}
		else
		{
			HealerPlayer = nullptr;
		}
	}
}

void AMResurrectionFlower::OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HealerPlayer && IsValid(GetWorld()))
	{
		if (AMMultiplayerGameState* GameState = Cast<AMMultiplayerGameState>(GetWorld()->GetGameState()))
		{
			GameState->ResurrectionTimer(false);
			GameState->AdditionalTimerDelegate.Clear();
		}
		HealerPlayer = nullptr;
	}
}

void AMResurrectionFlower::OnFinishTimer(ETypeOfAdditionalTimer TypeOfAdditionalTimer)
{
	RemoveResurrectionItemFromInventory(HealerPlayer->GetPlayerName());

	if (IsValid(GetWorld()->GetGameState()))
	{
		for (auto Player : GetWorld()->GetGameState()->PlayerArray)
		{
			if (Player->GetPlayerName() == GetPlayerState()->GetPlayerName())
			{
				if (AMPlayerController* PlayerController = Cast<AMPlayerController>(Player->GetPlayerController()))
				{
					PlayerController->ResurrectionCharacter();
				}
			}
		}
	}
}

void AMResurrectionFlower::IsHaveResurrectionItemInInventory_Implementation(const FString& PlayerName)
{
	if (IsValid(GetWorld()->GetGameState()))
	{
		for (auto Player : GetWorld()->GetGameState()->PlayerArray)
		{
			if (AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Player->GetPlayerController()->GetPawn()))
			{
				if (Player->GetPlayerName() == PlayerName)
				{
					//if (PlayerCharacter->InventoryComponent->IsHaveItem(ItemForResurrectionInfo))
					//{
					if (AMMultiplayerGameState* GameState = Cast<AMMultiplayerGameState>(GetWorld()->GetGameState()))
					{
						GameState->ResurrectionTimer(true);
						GameState->AdditionalTimerDelegate.Clear();
						GameState->AdditionalTimerDelegate.BindDynamic(this, &AMResurrectionFlower::OnFinishTimer);
					}
					/*}
					else
					{
						DeathPlayer = nullptr;
					}*/
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
