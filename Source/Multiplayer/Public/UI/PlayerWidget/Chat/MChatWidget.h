// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slate.h"
#include "Components/CanvasPanel.h"

#include "MSChatWidget.h"

#include "MChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<MSChatWidget> ChatWidget;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

private:
	UFUNCTION()
	void OnUpdateChat();
};
