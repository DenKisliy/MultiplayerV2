// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../UI/PlayerWidget/MTimerWidget.h"
#include "../UI/PlayerWidget/MResultGameWidget.h"
#include "../UI/PlayerWidget/GAS/Attribute/MAttributesGroupWidget.h"
#include "../UI/PlayerWidget/Item/MInventoryWidget.h"
#include "../UI/PlayerWidget/HUD/MInformWidget.h"

#include "../UI/Menu/MSLoginInWidget.h"
#include "../UI/Menu/MSRegistrationWidget.h"
#include "../UI/Menu/MSMultiplayerMenuWidget.h"
#include "../UI/Multiplayer/MSCreateSessionWidget.h"
#include "../UI/Multiplayer/MSFindSessionWidget.h"
#include "../UI/Inform/MSInformativeWidget.h"
#include "../UI/PlayerWidget/Chat/MSChatWidget.h"
#include "../UI/Menu/MSGameTypeMenuWidget.h"

#include "GameFramework/GameStateBase.h"
#include "../Subsystem/MPlayerInfoSubsystem.h"

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
	TSubclassOf<UMTimerWidget> TimerStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMResultGameWidget> ResultGameStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMAttributesGroupWidget> AttributesGroupStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMInventoryWidget> InventoryStatic;

	UPROPERTY(EditAnywhere, Category = "Widget Static Class")
	TSubclassOf<UMInformWidget> InformStatic;
	
	//Inform widget
	TSharedPtr<MSInformativeWidget> InformativeWidget;
	TSharedPtr<SWidget> InformContainer;

	//Menu widgets
	TSharedPtr<MSLoginInWidget> LoginInWidget;

	TSharedPtr<MSRegistrationWidget> RegistrationWidget;

	TSharedPtr<MSMultiplayerMenuWidget> MultiplayerMenuWidget;

	//Multiplayers widgets
	TSharedPtr<MSCreateSessionWidget> CreateSessionWidget;

	TSharedPtr<MSFindSessionWidget> FindSessionWidget;

	TSharedPtr<MSChatWidget> ChatWidget;

	TSharedPtr<MSGameTypeMenuWidget> GameTypeMenuWidget;

private:
	UMTimerWidget* TimerWidget;

	UMAttributesGroupWidget* AttributesGroupWidget;

	UMInventoryWidget* InventoryWidget;

	UMInformWidget* InformWidget;

	ETypeOfWidget CurrentWidgetType;
public:
	AMPlayerHUD(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	void SetTimeTimerWidget(bool bMain, int Time);

	UFUNCTION()
	void SetValueForAttribute(EAttributeType Type, float Value);

	void SetRemoveOrAddAttributesGroupWidget(bool bAdd);

	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool bValue);

	void ShowInformText(FString Text);

	void CloseWidget(ETypeOfWidget TypeOfWidget);

	void ShowWidget(ETypeOfWidget TypeOfUIWidget);

	void SetFocus(ETypeOfWidget TypeOfWidget);

protected:
	virtual void PostInitializeComponents() override;

	virtual void DrawHUD() override;
};
