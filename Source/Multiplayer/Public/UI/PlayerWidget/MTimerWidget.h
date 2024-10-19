// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "MTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MainTimeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AdditionalTimeText;

public:
	void SetTimeForTimer(bool bMain, int Time);

private:
	FText GetTimeFortext(int Time);
};
