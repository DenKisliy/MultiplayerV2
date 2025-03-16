// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MGameInstance.h"

UMGameInstance::UMGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGameInstance::SavePlayerInfo(FPlayerInfoStruct& NewPlayerInfo)
{
	PlayerInfo.UpdatePlayerInfo(NewPlayerInfo);
}

FPlayerInfoStruct& UMGameInstance::GetPlayerInfoFromGameInstance()
{
	return PlayerInfo;
}

void UMGameInstance::Init()
{
	Super::Init(); 
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &UMGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMGameInstance::EndLoadingScreen);
}

void UMGameInstance::BeginLoadingScreen(const FWorldContext& FWorldContext, const FString& MapName)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadingScreen UMGameInstance::BeginLoadingScreen"));
}

void UMGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadingScreen UMGameInstance::EndLoadingScreen"));
}