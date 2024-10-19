// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

#include "MInformWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMInformWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlock;

public:
	virtual void NativeConstruct() override;

	void SetText(FString Text);
};
