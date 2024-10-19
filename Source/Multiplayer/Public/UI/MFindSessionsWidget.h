// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MJoinSessionWidget.h"

#include "MFindSessionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMFindSessionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMJoinSessionWidget> JoinSessionWidget;

private:
	UTextBlock* TextBlockOfInform = nullptr;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateButtonForFindSession(bool bFindSession);

	void ShowInfoText(FString InfoText);

private:
	UFUNCTION()
	void TypeDoesNotSet();

	void CreateInformTextBlock();
};
