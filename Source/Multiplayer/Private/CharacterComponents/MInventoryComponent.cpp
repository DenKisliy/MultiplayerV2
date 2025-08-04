// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/MInventoryComponent.h"

// Sets default values for this component's properties
UMInventoryComponent::UMInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UMInventoryComponent::IsAddItemToInventory(FItemTypeInfo TypeInfo)
{
	bool bResult = false;
	if (ItemArray.Num() < 100)
	{
		if (!IsHaveItemInInventoryAdd(TypeInfo))
		{
			UMItemBase* NewItem = NewObject<UMItemBase>(UMItemBase::StaticClass(), FName(*FString::FromInt(ItemArray.Num() + 1)));
			if (IsValid(NewItem))
			{
				if (FItemData* Data = GetDataFromDataTable(TypeInfo))
				{
					NewItem->InitializeItemData(Data);
					ItemArray.Add(NewItem);
					bResult = true;
					UpdateInventoryDelegate.Broadcast();
				}
			}
		}
		else
		{
			bResult = true;
		}
	}
	
	return bResult;
}

void UMInventoryComponent::RemoveItemFromInventory(FItemTypeInfo TypeInfo)
{
	int FindItemIndex = ItemArray.IndexOfByPredicate([this, TypeInfo](const UMItemBase* InItem)
		{
			return InItem->TypeInfo.Index == TypeInfo.Index && InItem->TypeInfo.Type == TypeInfo.Type;
		});

	if (ItemArray.IsValidIndex(FindItemIndex))
	{
		if (ItemArray[FindItemIndex]->GetCount() > 1)
		{
			ItemArray[FindItemIndex]->ChangeCount(false);
		}
		else
		{
			ItemArray.RemoveAt(FindItemIndex);
		}
		SpawnItemAfterRemove(TypeInfo);
	}
}

void UMInventoryComponent::UseItem(FItemTypeInfo TypeInfo)
{
	int FindItemIndex = ItemArray.IndexOfByPredicate([this, TypeInfo](const UMItemBase* InItem)
		{
			return InItem->TypeInfo.Type == TypeInfo.Type && InItem->TypeInfo.Index == TypeInfo.Index;
		});

	if (ItemArray.IsValidIndex(FindItemIndex))
	{
		if (IsValid(ItemArray[FindItemIndex]->GameplayEffect))
		{
			UseItemDelegate.Broadcast(ItemArray[FindItemIndex]->GameplayEffect);
		}

		if (ItemArray[FindItemIndex]->GetCount() > 1)
		{
			ItemArray[FindItemIndex]->ChangeCount(false);
		}
		else
		{
			ItemArray.RemoveAt(FindItemIndex);
		}

		UpdateInventoryDelegate.Broadcast();
	}
}

FItemData* UMInventoryComponent::GetDataFromDataTable(FItemTypeInfo ItemType)
{
	if (DataTable)
	{
		FString ContextString;

		for (auto& Name : DataTable->GetRowNames())
		{
			if (FItemData* Row = DataTable->FindRow<FItemData>(Name, ContextString))
			{
				if (Row->TypeInfo.Type == ItemType.Type && Row->TypeInfo.Index == ItemType.Index)
				{
					return Row;
				}
			}
		}
	}
	return nullptr;
}

TArray<UMItemBase*> UMInventoryComponent::GetItemsArray()
{
	return ItemArray;
}

void UMInventoryComponent::SetPickUpItem(FItemTypeInfo ItemType, const AActor* ItemActor)
{
	PickUpItemType = ItemType;
	PickUpItemActor = ItemType.Type == ETypeOfItem::None && ItemType.Index == 0 ? nullptr : const_cast<AActor*>(ItemActor);
}

void UMInventoryComponent::AddPickUpItemToInventory()
{
	if (PickUpItemType.Type != ETypeOfItem::None && PickUpItemType.Index != 0)
	{
		if (IsAddItemToInventory(PickUpItemType))
		{
			DestroyPickUpItem(PickUpItemActor);
			PickUpItemType = FItemTypeInfo();
		}
	}
}

void UMInventoryComponent::DestroyPickUpItem_Implementation(const AActor* ItemActor)
{
	if (AActor* Item = const_cast<AActor*>(ItemActor))
	{
		Item->Destroy();
	}
}

void UMInventoryComponent::SpawnItemAfterRemove_Implementation(const FItemTypeInfo& Type)
{
	if (IsValid(GetWorld()))
	{
		if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode->SetSpawnItems(Type, GetOwner()->GetActorLocation());
		}
	}
}

bool UMInventoryComponent::IsHaveItemInInventoryAdd(FItemTypeInfo ItemType)
{
	for (UMItemBase* Item : ItemArray)
	{
		if (Item->IsTypeMatches(ItemType))
		{
			Item->ChangeCount(true);
			UpdateInventoryDelegate.Broadcast();
			return true;
		}
	}

	return false;
}

bool UMInventoryComponent::IsHaveItem(FItemTypeInfo ItemInfo)
{
	for (UMItemBase* Item : ItemArray)
	{
		if (Item->IsTypeMatches(ItemInfo))
		{
			return true;
		}
	}

	return false;
}
