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
				for (AActor* path : FindPathes)
				{
					if (AMPatrolPath* patrolPath = Cast<AMPatrolPath>(path))
					{
						if (!patrolPath->IsSetOwner())
						{
							if (AMAICharacter* AICharacter = GetWorld()->SpawnActor<AMAICharacter>(AICharacterStatic, patrolPath->GetStartPoint(), patrolPath->GetRotatorForStartPoint(), FActorSpawnParameters()))
							{
								patrolPath->SetOwner(AICharacter);
								AICharacter->StartMove(patrolPath);
								AICharacter->DetectPlayerDelegate.AddDynamic(this, &AMSpawnBotManager::DetectPlayerByBot);
								AICharacter->AIDeathDelegate.AddDynamic(this, &AMSpawnBotManager::OnActorDeath);
							}
						}
					}
				}
			}

			if (AMGameState* gameState = Cast<AMGameState>(GetWorld()->GetGameState()))
			{
				FScriptDelegate Delegate;
				Delegate.BindUFunction(this, "SpawnBot");
				gameState->TimerFinishDelegate.Remove(Delegate);
			}
		}
	}
}

void AMSpawnBotManager::DetectPlayerByBot(AActor* Bot, AActor* Player)
{
	if (AMAICharacter* bot = Cast<AMAICharacter>(Bot))
	{
		if (AMPlayerCharacter* player = Cast<AMPlayerCharacter>(Player))
		{
			if (!DetectPlayer.Find(bot))
			{
				if (!player->InsertInSaveZoneDelegate.IsBound())
				{
					player->InsertInSaveZoneDelegate.AddDynamic(this, &AMSpawnBotManager::PlayerRemoveSaveZone);
				}
				DetectPlayer.Add(bot, player);
			}
		}
		else
		{
			if (DetectPlayer.Find(bot))
			{
				DetectPlayer.Remove(bot);
			}
		}
	}
}

void AMSpawnBotManager::PlayerRemoveSaveZone(AActor* Player)
{
	if (AMPlayerCharacter* player = Cast<AMPlayerCharacter>(Player))
	{
		TArray<AMAICharacter*> stalkingBots;
		for (auto value : DetectPlayer)
		{
			if (value.Value == player)
			{
				stalkingBots.Add(value.Key);
			}
		}

		if (stalkingBots.Num() > 0)
		{
			for (AMAICharacter* bot : stalkingBots)
			{
				stalkingBots.Remove(bot);
			}
		}
	}
}

void AMSpawnBotManager::OnActorDeath(FVector Location)
{
	if (AMGameMode* gameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		for (FItemTypeInfo item : RewardItemsArray)
		{
			gameMode->SetSpawnItems(item, Location);
		}
	}
}

