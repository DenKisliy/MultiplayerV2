// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NavModifierComponent.h"
#include "NavAreas/NavArea_Null.h"

#include "../Character/MBaseCharacter.h"

#include "NavigationSystem.h"
#include "Math/MathFwd.h"

#include "MCaptureStation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeCountOfCapturePlayer, int, CountOfPlayer);

UCLASS()
class MULTIPLAYER_API AMCaptureStation : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StatueComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlatformComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNavModifierComponent* NavModifierComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CaptureElements, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMesh> RestoredStatueMesh;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_NewStatueMesh)
	TObjectPtr<UStaticMesh> NewStatueMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	TArray<FItemTypeInfo> RewardItemsArray;

	FChangeCountOfCapturePlayer ChangeCountOfCapturePlayerDelegate;

private:
	bool bCapture = true;

	int CountOfPlayer = 0;

	TArray<AMBaseCharacter*> InsertPlayerArray;

public:	
	// Sets default values for this actor's properties
	AMCaptureStation();

	void CaptureFinish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void ActivateGameplayEffectForCharacter(AMBaseCharacter* Character, bool bValue);

	UFUNCTION()
	void OnRep_NewStatueMesh();

	UFUNCTION()
	void OnUpdatedComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnUpdatedComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable)
	void AddItems();
};
