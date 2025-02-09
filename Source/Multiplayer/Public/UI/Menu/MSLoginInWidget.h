// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "GameFramework/HUD.h"
#include "../../Data/MUIDataStruct.h"

#include "../../Subsystem/MPlayerInfoSubsystem.h"

/**
 * 
 */
class MULTIPLAYER_API MSLoginInWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSLoginInWidget) : _OwnerHUD() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

public:
	TSharedPtr<SEditableTextBox> LoginBoxPtr;

	TSharedPtr<SEditableTextBox> PasswordBoxPtr;

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

	void SetFocus();

private:
	FReply OnSingIn() const;

	FReply OnQuit() const;

	FReply OnSignUp() const;

	void ShowInformWidget(FText Text) const;

	FString GetInformMessage(FString Login, FString Password) const;

	void SetStyle(FString Login, FString Password) const;

	void SetDefault() const;

	void SetNextWidget(ETypeOfWidget NextWidgetType) const;

	void OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

	void GetLoginAndPassword(FString& Login, FString& Password) const;
};
