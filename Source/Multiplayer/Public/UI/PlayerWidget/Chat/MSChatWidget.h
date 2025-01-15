// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MSMessageWidget.h"

#include "../../../Data/MUIDataStruct.h"

class MULTIPLAYER_API MSChatWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSChatWidget) : _OwnerHUD() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

	TSharedPtr<SListView<TSharedPtr<FChatMessageData>>> ListViewWidget;

	TArray<TSharedPtr<FChatMessageData>> MessagesArray;

public:
	TSharedPtr<SMultiLineEditableText> ChatInput;

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FChatMessageData> Item, const TSharedRef<STableViewBase>& OwnerTable);

	void UpdateChatBox();

private:
	FReply OnSendMessage() const;
};
