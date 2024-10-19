// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../UI/MChatPanelWidget.h"
#include "../UI/MMainMenuWidget.h"
#include "../UI/PlayerWidget/MTimerWidget.h"
#include "../UI/PlayerWidget/MResultGameWidget.h"
#include "../UI/PlayerWidget/GAS/Attribute/MAttributesGroupWidget.h"
#include "../UI/PlayerWidget/Item/MInventoryWidget.h"
#include "../UI/PlayerWidget/HUD/MInformWidget.h"

#include "GameFramework/GameStateBase.h"

#include "MPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMChatPanelWidget> ChatPanelStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMMainMenuWidget> MainMenuStatic;
	
	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMTimerWidget> TimerStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMResultGameWidget> ResultGameStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMAttributesGroupWidget> AttributesGroupStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMInventoryWidget> InventoryStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMInformWidget> InformStatic;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMChatPanelWidget* ChatPanelWidget;

private:

	UMTimerWidget* TimerWidget;

	UMAttributesGroupWidget* AttributesGroupWidget;

	UMInventoryWidget* InventoryWidget;

	UMInformWidget* InformWidget;

public:
	AMPlayerHUD(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	void SetDelegateForSendMessageEvent(FScriptDelegate Delegate);

	void AddMessage(FString PlayerName, FString MessageText);

	void SetTimeTimerWidget(bool bMain, int Time);

	UFUNCTION()
	void SetValueForAttribute(EAttributeType Type, float Value);

	void SetRemoveOrAddAttributesGroupWidget(bool bAdd);

	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool bValue);

	void ShowInformText(FString Text);
};
