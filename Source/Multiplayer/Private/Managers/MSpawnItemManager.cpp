// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MSpawnItemManager.h"

// Sets default values
AMSpawnItemManager::AMSpawnItemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMSpawnItemManager::SpawnItem(FItemTypeInfo ItemType, FVector SpawnLocation)
{
	if (IsValid(ItemStatic) && IsValid(GetWorld()))
	{
		if (FItemData* ItemData = GetDataFromDataTable(ItemType))
		{
			if (AMItemActor* Item = GetWorld()->SpawnActor<AMItemActor>(ItemStatic, SpawnLocation, FRotator(0), FActorSpawnParameters()))
			{
				if (ItemData)
				{
					Item->InitializeItemData(ItemData);
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AMSpawnItemManager::BeginPlay()
{
	Super::BeginPlay();
}

FItemData* AMSpawnItemManager::GetDataFromDataTable(FItemTypeInfo ItemType)
{
	if (DataTable)
	{
		FString ContextString;
		
		for (auto& Name : DataTable->GetRowNames())
		{
			FItemData* Row = DataTable->FindRow<FItemData>(Name, ContextString);
			if (Row)
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

