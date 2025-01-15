// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "GameFramework/HUD.h"
#include "../../Data/MPlayerDataStruct.h"
#include "../../Data/MUIDataStruct.h"

#include "../../Subsystem/MPlayerInfoSubsystem.h"

/**
 * 
 */
class MULTIPLAYER_API MSRegistrationWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSRegistrationWidget) : _OwnerHUD() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

	SComboBox<TSharedPtr<FText>> TypeOfCharacterPtr;

	TSharedPtr<SComboBox<TSharedPtr<FText>>> TypeOfCharacterComboBox;

	TSharedPtr<STextBlock> CheckBoxPtr;

	TSharedPtr<FText> SelectedTypeOfCharacter;

	TArray<TSharedPtr<FText>> TypeOfCharacterArray;

	TSharedPtr<FText> DefaultTypeOfCharacter;

public:
	TSharedPtr<SEditableTextBox> LoginBoxPtr;

	TSharedPtr<SEditableTextBox> PasswordBoxPtr;

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

	void SetFocus();

private:
	FReply OnSingUn() const;

	FReply OnCancel() const;

	FReply OnBackToPreviousMenu() const;

	void ShowInformWidget(FText Text, bool bWarning = true, ETypeOfWidget NextWidget = ETypeOfWidget::None, ETypeOfWidget PreviousWidget = ETypeOfWidget::Registration) const;

	FString GetInformMessage(FString Login, FString Password, FString Type) const;

	 void SetStyle(FString Login, FString Password, FString Type) const;

	 void SetDefaultValuesForWidgets();

	 void OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);
};
