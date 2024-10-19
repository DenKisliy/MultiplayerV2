// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "../../../Items/MItemBase.h"

#include "MItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Image;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* RemoveButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* UseButton;

private:
	FItemDescription Description = FItemDescription();

	FItemTypeInfo TypeInfo;

	int32 Count;

protected:
	virtual void NativeConstruct() override;

public:
	void SetItemInfo(UMItemBase* ItemData);

private:
	UFUNCTION()
	void RemoveItem();

	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void SetIcon(UTexture2D* Texture);
};
