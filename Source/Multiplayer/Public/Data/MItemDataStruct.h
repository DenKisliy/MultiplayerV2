#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataTable.h"

#include "MItemDataStruct.generated.h"

UENUM(BlueprintType)
enum class ETypeOfItem : uint8
{
	None = 0,
	Potion,
	Weapon,
	Support
};

UENUM(BlueprintType)
enum class ETypeOfPotion : uint8
{
	None = 0,
	Helth,
	Mana,
	Stamina,
};

USTRUCT(BlueprintType)
struct FItemTypeInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ETypeOfItem Type;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int Index;

	FItemTypeInfo()
	{
		Type = ETypeOfItem::None;
		Index = 0;
	}
};

USTRUCT()
struct FItemAsset
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere)
	FVector MeshScale;

	FItemAsset()
	{
		Icon = nullptr;
		StaticMesh = nullptr;
		MeshScale = FVector(1);
	}
};

USTRUCT()
struct FItemDescription
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FName ItemName;
	
	UPROPERTY(EditAnywhere)
	FName ItemDescription;

	FItemDescription()
	{
		ItemName = "Name";
		ItemDescription = "Description";
	}

	void SetData(FName Name, FName Description)
	{
		ItemName = Name;
		ItemDescription = Description;
	}

	FString GetName()
	{
		return ItemName.ToString();
	}
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	FItemTypeInfo TypeInfo;

	UPROPERTY(EditAnywhere)
	FItemDescription Description;

	UPROPERTY(EditAnywhere)
	FItemAsset Asset;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	FItemData()
	{
		TypeInfo = FItemTypeInfo();
		Description = FItemDescription();
		Asset = FItemAsset();
	}
};