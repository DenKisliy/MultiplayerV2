// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../../UI/PlayerWidget/MTimerWidget.h"
#include "../../UI/PlayerWidget/GAS/Attribute/MAttributesGroupWidget.h"
#include "../../UI/PlayerWidget/Item/MInventoryWidget.h"
#include "../../UI/PlayerWidget/HUD/MInformWidget.h"

#include "../../UI/PlayerWidget/Chat/MSChatWidget.h"

#include "MPlayingHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMPlayingHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMTimerWidget> TimerStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMAttributesGroupWidget> AttributesGroupStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMInventoryWidget> InventoryStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMInformWidget> InformStatic;

	TSharedPtr<MSChatWidget> ChatWidget;

private:
	UMTimerWidget* TimerWidget;

	UMAttributesGroupWidget* AttributesGroupWidget;

	UMInventoryWidget* InventoryWidget;

	UMInformWidget* InformWidget;

public:
	AMPlayingHUD(const FObjectInitializer& ObjectInitializer);

	void SetTimeTimerWidget(bool bMain, int Time);

	UFUNCTION()
	void SetValueForAttribute(EAttributeType Type, float Value);

	void SetRemoveOrAddAttributesGroupWidget(bool bAdd);

	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool bValue);

	void ShowInformText(FString Text);

protected:
	virtual void PostInitializeComponents() override;

	virtual void DrawHUD() override;
};
