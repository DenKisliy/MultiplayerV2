// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/EditableTextBox.h"
#include "Components/PanelSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Widget.h"

#include "MChatMessageWidget.h"
#include "GameFramework/PlayerController.h"

#include "GameFramework/Controller.h"
#include "../GameFramework/MPlayerState.h"

#include "MChatPanelWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSendMessage, const FString&, PlayerName, const FString&, MessageText);

UCLASS()
class MULTIPLAYER_API UMChatPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMChatMessageWidget> ChatMessageWidget;

	UPROPERTY(EditAnywhere, Category = "Editable Text Style")
	FEditableTextBoxStyle EditableTextStyle;

	FOnSendMessage SendMessageDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEditableTextBox* InputMessageBox;

private:
	UScrollBox* ScrollBox;

protected:
	virtual void NativeConstruct() override;

public:
	void AddMessage(FString PlayerName, FString MessageText);

	UFUNCTION()
	void OnTextEntered(const FText& Text, ETextCommit::Type CommitMethod);

	UWidget* GetInputMessageBox();
};
