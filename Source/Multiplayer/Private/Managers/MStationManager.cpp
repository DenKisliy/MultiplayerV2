// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MStationManager.h"
#include "../../Public/GameFramework/GameState/MMultiplayerGameState.h"

// Sets default values
AMStationManager::AMStationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMStationManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetWorld()))
	{
		if (AMBaseGameState* GameState = Cast<AMBaseGameState>(GetWorld()->GetGameState()))
		{
			GameState->TimerFinishDelegate.AddUObject(this, &AMStationManager::OnTimeFinished);
			SetFinishBind();
		}
	}
}

void AMStationManager::OnSetCaptureStation(int32 CountOfPlayer)
{
	if (IsValid(GetWorld()))
	{
		if (AMBaseGameState* GameState = Cast<AMBaseGameState>(GetWorld()->GetGameState()))
		{
			GameState->TimerAccelerationFactorDelegate.ExecuteIfBound((CountOfPlayer == 0) ? 0 : (1.0f / CountOfPlayer));
		}
	}
}

void AMStationManager::OnTimeFinished(ETypeOfTimer TypeOfFinishTimer)
{
	FindCaptureStationArray();

	if (TypeOfFinishTimer == ETypeOfTimer::CaptureStation)
	{
		OnStationCaptured();
	}

	SpawnStation();

	CheckCountOfCaptureStations();
}

void AMStationManager::FindCaptureStationArray()
{
	if (CaptureStationArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CaptureStation"), CaptureStationArray);

		if (CaptureStationArray.Num() > 1)
		{
			CaptureStationArray.Sort([](const AActor& Lhs, const AActor& Rhs) {
				TArray<FName> leftTags = Lhs.Tags;
				TArray<FName> rightTags = Rhs.Tags;
				bool result = false;

				if (rightTags.Num() > 0 && leftTags.Num() > 0)
				{
					FName* rightFindElem = nullptr;
					FName* leftFindElem = nullptr;

					auto searchFunction = [](const FName& Item) {
						FString text = Item.ToString();
						return text.Contains("CaptureStation_");
						};

					rightFindElem = rightTags.FindByPredicate(searchFunction);
					leftFindElem = leftTags.FindByPredicate(searchFunction);

					if (rightFindElem && leftFindElem)
					{
						FString rightText = rightFindElem->ToString();
						FString leftText = leftFindElem->ToString();
						rightText.RemoveFromStart("CaptureStation_");
						leftText.RemoveFromStart("CaptureStation_");
						result = FCString::Atoi(*rightText) > FCString::Atoi(*leftText);
					}
				}

				return result;
				});
		}
	}
}

void AMStationManager::CheckCountOfCaptureStations()
{
	TArray<AActor*> StationCheck;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CaptureStation"), StationCheck);

	if (CaptureStationCount >= 1 && StationCheck.Num() == CaptureStationCount)
	{
		if (AMMultiplayerGameState* GameState = Cast<AMMultiplayerGameState>(GetWorld()->GetGameState()))
		{
			GameState->SaveResultOfMatch(EResultOfGame::Win);
			return;
		}
	}
}

void AMStationManager::SpawnStation()
{
	if (CaptureStationArray.Num() > 0)
	{
		if (SpawnStationStatic && IsValid(CaptureStationArray[0]))
		{
			CaptureStation = GetWorld()->SpawnActor<AMCaptureStation>(SpawnStationStatic, CaptureStationArray[0]->GetActorLocation(),
				CaptureStationArray[0]->GetActorRotation(), FActorSpawnParameters());
			CaptureStation->ChangeCountOfCapturePlayerDelegate.AddDynamic(this, &AMStationManager::OnSetCaptureStation);
			CaptureStationArray[0]->Destroy();
			CaptureStationArray.RemoveAt(0);
		}
	}
}

void AMStationManager::OnStationCaptured()
{
	CaptureStation->CaptureFinish();
	CaptureStationCount = CaptureStationCount == -1 ? 1 : CaptureStationCount + 1;
	CaptureStation->ChangeCountOfCapturePlayerDelegate.Clear();
	CaptureStation = nullptr;
}

