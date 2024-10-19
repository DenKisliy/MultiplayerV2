// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

#include "MItemInterface.h"
#include "Engine/AssetManager.h"

#include "MItemActor.generated.h"

UCLASS()
class MULTIPLAYER_API AMItemActor : public AActor, public IMItemInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	FItemTypeInfo TypeInfo;

	FItemDescription Description;

	TSoftObjectPtr<UStaticMesh> StaticMeshAsset;

private:
	TArray<AActor*> InsertPlayerArray;

	UPROPERTY(ReplicatedUsing = OnRep_StaticMesh)
	UStaticMesh* StaticMesh;
	
	UPROPERTY(ReplicatedUsing = OnRep_ScaleVector)
	FVector ScaleVector;

public:	
	// Sets default values for this actor's properties
	AMItemActor();

	virtual void GetStaticMesh() override;

	virtual void InitializeItemData(FItemData* NewItemData) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_StaticMesh();

	UFUNCTION()
	void OnRep_ScaleVector();

	UFUNCTION(Server, Reliable)
	void SetStaticMesh(const UStaticMesh* NewStaticMesh);

	UFUNCTION(Server, Reliable)
	void SetScale(const FVector& NewScale);

	UFUNCTION(Server, Reliable)
	void ShowInfoText(const FString& PlayerName, const FString& Text);
};
