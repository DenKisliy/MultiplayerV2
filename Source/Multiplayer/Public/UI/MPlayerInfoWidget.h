// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

#include "MPlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NameTextBlock;

public:
	void SetPlayerName(FString PlayerName);

	FString GetPlayerName();
};
