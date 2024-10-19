// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MItemInterface.h"

#include "Engine/AssetManager.h"

#include "MItemBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnIconTextureLoad, UTexture2D*, Texture);

UCLASS()
class MULTIPLAYER_API UMItemBase : public UObject, public IMItemInterface
{
	GENERATED_BODY()

public:
	FItemDescription Descriotion;

	TSoftObjectPtr<UTexture2D> IconPath;

	FOnIconTextureLoad OnIconTextureLoadDelegate;

	FItemTypeInfo TypeInfo;

	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY()
	int Count;

public:
	UMItemBase();

	virtual void GetIconTexture() override;

	virtual void InitializeItemData(FItemData* NewItemData) override;

	void ChangeCount(bool bAdd);

	bool IsTypeMatches(FItemTypeInfo CheckType);

	int GetCount();
};
