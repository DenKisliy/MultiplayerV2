// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MCaptureBaseManager.h"
#include "../../Public/GameFramework/MGameMode.h"
#include "../../Public/GameFramework/MGameState.h"

// Sets default values
AMCaptureBaseManager::AMCaptureBaseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMCaptureBaseManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetWorld()))
	{
		if (AMGameState* GameState = Cast<AMGameState>(GetWorld()->GetGameState()))
		{
			GameState->TimerFinishDelegate.AddDynamic(this, &AMCaptureBaseManager::SpawnStation);
			SetFinishBind();
		}
	}
}

void AMCaptureBaseManager::OnSetCaptureStation(int CountOfPlayer)
{
	if (IsValid(GetWorld()))
	{
		if (AMGameState* GameState = Cast<AMGameState>(GetWorld()->GetGameState()))
		{
			GameState->TimerAccelerationFactorDelegate.Broadcast((CountOfPlayer == 0) ? 0 : (1.0f / CountOfPlayer));
		}
	}
}

void AMCaptureBaseManager::SpawnStation(ETypeOfTimer TypeOfFinishTimer)
{
	GetCaptureStationArray();

	if (CaptureStationArray.Num() > 0)
	{
		if (TypeOfFinishTimer == ETypeOfTimer::CaptureStation)
		{
			CaptureStation->CaptureFinish();
			CaptureStationCount = CaptureStationCount == -1 ? 1 : CaptureStationCount + 1;
		}

		if (SpawnStationStatic)
		{
			if (CaptureStation)
			{
				CaptureStation->ChangeCountOfCapturePlayerDelegate.Clear();
				CaptureStation = nullptr;
			}

			CaptureStation = GetWorld()->SpawnActor<AMCaptureStation>(SpawnStationStatic, CaptureStationArray[0]->GetActorLocation(), 
				CaptureStationArray[0]->GetActorRotation(), FActorSpawnParameters());
			CaptureStation->ChangeCountOfCapturePlayerDelegate.AddDynamic(this, &AMCaptureBaseManager::OnSetCaptureStation);
			CaptureStationArray.RemoveAt(0);
		}
		
		TArray<AActor*> StationCheck;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CaptureStation"), StationCheck);

		if (CaptureStationCount >= 1 && StationCheck.Num() == CaptureStationCount)
		{
			if (AMGameState* GameState = Cast<AMGameState>(GetWorld()->GetGameState()))
			{
				GameState->SaveResultOfGame(EResultOfGame::Win);
				return;
			}
		}
	}
}

void AMCaptureBaseManager::GetCaptureStationArray()
{
	if (CaptureStationArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CaptureStation"), CaptureStationArray);

		if (CaptureStationArray.Num() > 0)
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

