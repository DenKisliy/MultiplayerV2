// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

#include "MChatMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn = "true"))
	FString PlayerMessage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBox;

public:
	UMChatMessageWidget(const FObjectInitializer& ObjectInitializer);

	void SetPlayerNameAndPlayerMessage(FString NewPlayerName, FString NewPlayerMessage);

protected:
	virtual void NativeConstruct() override;
};
