// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/EditableText.h"
#include "Components/ComboBoxString.h"
#include "Blueprint/WidgetTree.h"

#include "MFindSessionsWidget.h"

#include "../GameFramework/MPlayerState.h"
#include "../Subsystem/MSessionSubsystem.h"

#include "../../Multiplayer.h"
#include "UObject/Class.h"
#include "Types/SlateEnums.h"

#include "MMainMenuWidget.generated.h"

/**
 * 
 */
class UBorder;
class UVerticalBox;
class UButton;
class UTextBlock;
class USizeBox;
class UScaleBox;
class UEditableText;
class UComboBoxString;
class UWidgetTree;

UCLASS()
class MULTIPLAYER_API UMMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMFindSessionsWidget> FindSessionsWidget;

	UPROPERTY(EditAnywhere)
	FTableRowStyle ItemStyle;
	
	UPROPERTY(EditAnywhere)
	FTableRowStyle ComboBoxItemStyle;

private:
	FString SessionName;
	
	UMFindSessionsWidget* FindSessionResultWidget;

	UComboBoxString* ComboBoxWidget;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ValidateSessionNameText(const FText& Text);

	UFUNCTION()
	void ValidatePlayerNameText(const FText& Text);

	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void FindSessions();

	UFUNCTION()
	void LoginIn();
	
	UFUNCTION()
	void SelectCharacter(FString SelectedItem, ESelectInfo::Type SelectionType);

private:
	void CreateButton(FString NameOfButton, FScriptDelegate ClickDelegate);

	void CreateEditableTextBox(FString HintText, FScriptDelegate ChangeDelegate);

	void CreateComboBox(FScriptDelegate SelectDelegate);

	FScriptDelegate CreateDelegate(UObject* Object, FString FunctionName);
};
