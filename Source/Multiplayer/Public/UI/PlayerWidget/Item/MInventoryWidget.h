// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MItemWidget.h"

#include "Components/ScrollBox.h"

#include "MInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WidgetName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(EditAnywhere, Category = "Static Widgets")
	TSubclassOf<UMItemWidget> ItemStaticWidget;

private:
	const int32 CountOfItemInOneRow = 4;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void CreateItemsList();
};
