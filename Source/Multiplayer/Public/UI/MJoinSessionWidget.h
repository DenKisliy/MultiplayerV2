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
#include "Blueprint/WidgetTree.h"

#include "../Subsystem/MSessionSubsystem.h"

#include "../GameFramework/MPlayerState.h"

#include "MJoinSessionWidget.generated.h"

/**
 * 
 */

class UBorder;
class UVerticalBox;
class UButton;
class UTextBlock;
class USizeBox;
class UScaleBox;
class UWidgetTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTypeDoesNotSetEvent);

UCLASS()
class MULTIPLAYER_API UMJoinSessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

	FTypeDoesNotSetEvent TypeDoesNotSetEvent;

private:
	FOnlineSessionSearchResult Session;

public:
	void CreateButton(FOnlineSessionSearchResult SessionInfo);

	UFUNCTION()
	void OnClickButton();
};
