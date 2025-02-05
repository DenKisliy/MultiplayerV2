// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MSpawnBotManager.h"
#include "../../Public/GameFramework/MGameMode.h"

// Sets default values
AMSpawnBotManager::AMSpawnBotManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMSpawnBotManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetWorld()))
	{
		if (AMGameState* GameState = Cast<AMGameState>(GetWorld()->GetGameState()))
		{
			GameState->TimerFinishDelegate.AddDynamic(this, &AMSpawnBotManager::SpawnBot);
			SetFinishBind();
		}
	}
}

void AMSpawnBotManager::SpawnBot(ETypeOfTimer TypeOfFinishTimer)
{
	if (TypeOfFinishTimer == ETypeOfTimer::StartMatch)
	{
		if (IsValid(PatrolPathStatic) && IsValid(AICharacterStatic))
		{
			TArray<AActor*> FindPathes;
			UGameplayStatics::GetAllActorsOfClass(this, PatrolPathStatic, FindPathes);

			if (FindPathes.Num() > 0)
			{
				for (AActor* Path : FindPathes)
				{
					if (AMPatrolPath* PatrolPath = Cast<AMPatrolPath>(Path))
					{
						if (!PatrolPath->IsSetOwner())
						{
							if (AMAICharacter* AICharacter = GetWorld()->SpawnActor<AMAICharacter>(AICharacterStatic, 
								PatrolPath->GetStartPoint(), PatrolPath->GetRotatorForStartPoint(), FActorSpawnParameters()))
							{
								PatrolPath->SetOwner(AICharacter);
								AICharacter->StartMove(PatrolPath);
								AICharacter->DetectPlayerDelegate.AddDynamic(this, &AMSpawnBotManager::DetectPlayerByBot);
								AICharacter->AIDeathDelegate.AddDynamic(this, &AMSpawnBotManager::OnActorDeath);
							}
						}
					}
				}
			}

			if (IsValid(GetWorld()))
			{
				if (AMGameState* GameState = Cast<AMGameState>(GetWorld()->GetGameState()))
				{
					FScriptDelegate Delegate;
					Delegate.BindUFunction(this, "SpawnBot");
					GameState->TimerFinishDelegate.Remove(Delegate);
				}
			}
		}
	}
}

void AMSpawnBotManager::DetectPlayerByBot(AActor* Bot, AActor* Player)
{
	if (AMAICharacter* AICharacter = Cast<AMAICharacter>(Bot))
	{
		if (AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Player))
		{
			if (!DetectPlayer.Find(AICharacter))
			{
				if (!PlayerCharacter->InsertInSaveZoneDelegate.IsBound())
				{
					PlayerCharacter->InsertInSaveZoneDelegate.AddDynamic(this, &AMSpawnBotManager::PlayerRemoveSaveZone);
				}
				DetectPlayer.Add(AICharacter, PlayerCharacter);
			}
		}
		else
		{
			if (DetectPlayer.Find(AICharacter))
			{
				DetectPlayer.Remove(AICharacter);
			}
		}
	}
}

void AMSpawnBotManager::PlayerRemoveSaveZone(AActor* Player)
{
	if (AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Player))
	{
		TArray<AMAICharacter*> StalkingBots;
		for (auto Value : DetectPlayer)
		{
			if (Value.Value == Player)
			{
				StalkingBots.Add(Value.Key);
			}
		}

		if (StalkingBots.Num() > 0)
		{
			for (AMAICharacter* Bot : StalkingBots)
			{
				StalkingBots.Remove(Bot);
			}
		}
	}
}

void AMSpawnBotManager::OnActorDeath(FVector Location)
{
	if (IsValid(GetWorld()))
	{
		if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			for (FItemTypeInfo Item : RewardItemsArray)
			{
				GameMode->SetSpawnItems(Item, Location);
			}
		}
	}
}

