// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../../UI/PlayerWidget/MTimerWidget.h"
#include "../../UI/PlayerWidget/GAS/Attribute/MAttributesGroupWidget.h"
#include "../../UI/PlayerWidget/Item/MInventoryWidget.h"
#include "../../UI/PlayerWidget/HUD/MInformWidget.h"
#include "../../UI/PlayerWidget/Chat/MChatWidget.h"

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

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMChatWidget> ChatStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* ChatWidget;

private:
	UMTimerWidget* TimerWidget;

	UMAttributesGroupWidget* AttributesGroupWidget;

	UMInventoryWidget* InventoryWidget;

	UMInformWidget* InformWidget;

	FTimerHandle UpdatePlayerNameTimer;

	FTimerHandle UpdateSetHUDTimer;

public:
	AMPlayingHUD(const FObjectInitializer& ObjectInitializer);

	void SetTimeTimerWidget(bool bMain, int Time);

	UFUNCTION()
	void SetValueForAttribute(EAttributeType Type, float Value);

	void SetRemoveOrAddAttributesGroupWidget(bool bAdd);

	void ShowInformText(FString Text);

	void SetModeAndShowMouse(const FInputModeDataBase& GameMode, bool bShowMouse, UUserWidget* WidgetForFocus = nullptr);

protected:
	virtual void PostInitializeComponents() override;

	virtual void DrawHUD() override;

private:
	bool IsStandAloneMode();

	void UpdatePlyerName();

	void ShowInventory();

	void CreateChat();

	void OnHUDSet();
};
