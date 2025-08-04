// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "../GameFramework/MPlayerController.h"

#include "MResurrectionFlower.generated.h"

UCLASS()
class MULTIPLAYER_API AMResurrectionFlower : public APawn
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	FItemTypeInfo ItemForResurrectionInfo;

private:
	FString HealerPlayerName = "";

public:	
	// Sets default values for this actor's properties
	AMResurrectionFlower();

	UFUNCTION()
	void OnFinishTimer(ETypeOfAdditionalTimer TypeOfAdditionalTimer);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable)
	void IsHaveResurrectionItemInInventory(const FString& PlayerName);

	UFUNCTION(Server, Reliable)
	void RemoveResurrectionItemFromInventory(const FString& PlayerName);
};
