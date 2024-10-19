// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MAttributeWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"

#include "../Multiplayer.h"

#include "MAttributesGroupWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMAttributesGroupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMAttributeWidget> AttributeWidget;

private:
	UMAttributeWidget* HealthAttribute;
	
	UMAttributeWidget* ManaAttribute;
	
	UMAttributeWidget* StaminaAttribute;

	UVerticalBox* VerticalBox;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetValueForAttribute(EAttributeType Type, float Value);
};
