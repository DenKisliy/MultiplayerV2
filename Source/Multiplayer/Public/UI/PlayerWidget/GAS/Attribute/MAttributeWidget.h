// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ProgressBar.h"

#include "MAttributeWidget.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class MULTIPLAYER_API UMAttributeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBar;

public:
	void SetColor(FLinearColor Color);

	void SetPercent(float Value);
};
