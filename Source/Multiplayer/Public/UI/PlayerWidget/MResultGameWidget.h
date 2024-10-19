// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Blueprint/WidgetTree.h"

#include "../../GameFramework/MPlayerState.h"

#include "MResultGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMResultGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

private:
	UTextBlock* ResultGameTextBlock;

protected:
	virtual void NativeConstruct() override;

public:
	void SetResultOfGame(bool bWin);

	UFUNCTION()
	void CloseWindow();

private:
	UButton* CreateButton(FString NameOfButton, FScriptDelegate ClickDelegate);
};
