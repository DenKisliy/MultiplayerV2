// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameInstance.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "../GameFramework/MGameMode.h"
#include "MPlayerHUD.h"
#include "MPlayerState.h"
#include "MGameInstance.h"
#include "GameFramework/Character.h"
#include "../Subsystem/MPlayerInfoSubsystem.h"

#include "MPlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddItemToInventory, FItemTypeInfo, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPickUpItemToInventory, FItemTypeInfo, ItemType, const AActor*, Item);

UCLASS()
class MULTIPLAYER_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	AMPlayerHUD* PlayerHUD;

	FVector SpawnLocationAfterDeath  = FVector(0);

	FTimerHandle CharacterTypeTimer;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETypeOfCharacter TypeOfCharacter = ETypeOfCharacter::None;

	FAddItemToInventory AddItemToInventoryDelegate;

	FPickUpItemToInventory PickUpItemToInventoryDelegate;

public:
	AMPlayerController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay();

	UFUNCTION(Server, Reliable)
	void MessageSubmit();

	UFUNCTION(Client, Reliable)
	void UpdateChat();

	void CharacterDeath();

	UFUNCTION(Server, Reliable)
	void SpawnResurrectionFlower(bool bValue);

	UFUNCTION(BlueprintCallable)
	FPlayerInfoStruct& GetPlayerInfo();

	void ResurrectionCharacter();

	UFUNCTION(Client, Reliable)
	void AddItemToCharacter(const FItemTypeInfo& ItemType);

	UFUNCTION(Client, Reliable)
	void SetPickUpItemToInventory(const FItemTypeInfo& ItemType, const AActor* ItemActor);

	UFUNCTION(Client, Reliable)
	void ShowInfoForCharacter(const FString& Text);

	void SetPlayerHUD();

private:
	UFUNCTION(Server, Reliable)
	void SpawnCharacter(ETypeOfCharacter SelectTypeOfCharacter);

	FString GetPlayerStartByTag();

	void OnSetCharacterType();

protected:
	virtual void OnPossess(APawn* aPawn) override;
};
