// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameInstance.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "../GameFramework/MGameMode.h"
#include "MPlayerState.h"
#include "MGameInstance.h"
#include "GameFramework/Character.h"
#include "../Subsystem/MPlayerInfoSubsystem.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "MPlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddItemToInventory, FItemTypeInfo, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPickUpItemToInventory, FItemTypeInfo, ItemType, const AActor*, Item);
DECLARE_DYNAMIC_DELEGATE(FUpdateChat);

UCLASS()
class MULTIPLAYER_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	FVector SpawnLocationAfterDeath  = FVector(0);

	FTimerHandle CharacterTypeTimer;

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Chat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChatAction;

	/** Chat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETypeOfCharacter TypeOfCharacter = ETypeOfCharacter::None;

	FAddItemToInventory AddItemToInventoryDelegate;

	FPickUpItemToInventory PickUpItemToInventoryDelegate;

	FUpdateChat UpdateChatWidgetDelegate;

public:
	AMPlayerController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay();

	void CharacterDeath();

	UFUNCTION(Server, Reliable)
	void SpawnResurrectionFlower(bool bValue);

	UFUNCTION(BlueprintCallable)
	FPlayerInfoStruct& GetPlayerInfo();

	UFUNCTION(Client, Reliable)
	void ResurrectionCharacter();

	UFUNCTION(Client, Reliable)
	void AddItemToCharacter(const FItemTypeInfo& ItemType);

	UFUNCTION(Client, Reliable)
	void SetPickUpItemToInventory(const FItemTypeInfo& ItemType, const AActor* ItemActor);

	UFUNCTION(Client, Reliable)
	void ShowInfoForCharacter(const FString& Text);

	void SetPlayerHUD();

	UFUNCTION(Server, Reliable)
	void MessageSubmit();

	virtual void InitPlayerState() override;

private:
	UFUNCTION(Server, Reliable)
	void SpawnCharacter(ETypeOfCharacter SelectTypeOfCharacter);

	FString GetPlayerStartByTag();

	void OnSetCharacterType();

	UFUNCTION(Client, Reliable)
	void OnUpdateChat();

	void SetPlayerNameAfterSpawn();

protected:
	virtual void OnPossess(APawn* aPawn) override;

	virtual void SetupInputComponent() override;

	virtual void OnRep_PlayerState() override;

	void InitializeInput();
};
