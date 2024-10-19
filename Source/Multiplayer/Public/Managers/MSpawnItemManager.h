// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "../Items/MItemActor.h"

#include "MSpawnItemManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpawnItem, ETypeOfItem, Type, FVector, Position);

UCLASS()
class MULTIPLAYER_API AMSpawnItemManager : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor, meta = (AllowPrivateAccess = true))
	TSubclassOf<AMItemActor> ItemStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	UDataTable* DataTable;

	FSpawnItem SpawnItemDelegate;

public:	
	// Sets default values for this actor's properties
	AMSpawnItemManager();

	UFUNCTION()
	void SpawnItem(FItemTypeInfo TypeInfo, FVector SpawnLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FItemData* GetDataFromDataTable(FItemTypeInfo TypeInfo);
};
