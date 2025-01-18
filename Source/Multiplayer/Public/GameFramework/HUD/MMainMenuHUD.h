// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../../UI/Menu/MSLoginInWidget.h"
#include "../../UI/Menu/MSRegistrationWidget.h"
#include "../../UI/Menu/MSMultiplayerMenuWidget.h"
#include "../../UI/Menu/MSGameTypeMenuWidget.h"
#include "../../UI/Multiplayer/MSCreateSessionWidget.h"
#include "../../UI/Multiplayer/MSFindSessionWidget.h"
#include "../../UI/Inform/MSInformativeWidget.h"

#include "MMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	TSharedPtr<MSFindSessionWidget> FindSessionWidget;

private:
	//Inform widget
	TSharedPtr<MSInformativeWidget> InformativeWidget;
	TSharedPtr<SWidget> InformContainer;

	//Menu widgets
	TSharedPtr<MSLoginInWidget> LoginInWidget;

	TSharedPtr<MSRegistrationWidget> RegistrationWidget;

	TSharedPtr<MSGameTypeMenuWidget> GameTypeMenuWidget;

	TSharedPtr<MSMultiplayerMenuWidget> MultiplayerMenuWidget;

	//Multiplayers widgets
	TSharedPtr<MSCreateSessionWidget> CreateSessionWidget;

public:
	AMMainMenuHUD(const FObjectInitializer& ObjectInitializer);

	void CloseWidget(ETypeOfWidget TypeOfWidget);

	void ShowWidget(ETypeOfWidget TypeOfUIWidget);

	void SetFocus(ETypeOfWidget TypeOfWidget);

	void ShowInformWidget(FInformWidgetData& InformWidgetData);

protected:
	virtual void PostInitializeComponents() override;
};
