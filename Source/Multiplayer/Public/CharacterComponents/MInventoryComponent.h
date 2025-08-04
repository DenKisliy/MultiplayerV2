// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

#include "../Items/MItemBase.h"
#include "../GameFramework/MGameMode.h"
#include "Net/UnrealNetwork.h"

#include "MInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUseItem, TSubclassOf<UGameplayEffect>, GameplayEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateInventory);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYER_API UMInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	UDataTable* DataTable;

public:
	FUseItem UseItemDelegate;

	FUpdateInventory UpdateInventoryDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UMItemBase*> ItemArray;

private:
	FItemTypeInfo PickUpItemType;

	AActor* PickUpItemActor;

public:	
	// Sets default values for this component's properties
	UMInventoryComponent();

	bool IsAddItemToInventory(FItemTypeInfo TypeInfo);

	void RemoveItemFromInventory(FItemTypeInfo TypeInfo);

	void UseItem(FItemTypeInfo TypeInfo);

	TArray<UMItemBase*> GetItemsArray();

	void SetPickUpItem(FItemTypeInfo ItemType, const AActor* ItemActor);

	void AddPickUpItemToInventory();
	
	bool IsHaveItem(FItemTypeInfo ItemInfo);

private:
	FItemData* GetDataFromDataTable(FItemTypeInfo ItemType);

	bool IsHaveItemInInventoryAdd(FItemTypeInfo ItemType);

	UFUNCTION(Server, Reliable)
	void DestroyPickUpItem(const AActor* ItemActor);

	UFUNCTION(Server, Reliable)
	void SpawnItemAfterRemove(const FItemTypeInfo& ItemType);
};
