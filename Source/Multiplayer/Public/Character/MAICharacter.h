// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../AI/Move/MPatrolPath.h"

#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "../Character/MBaseCharacter.h"
#include "../UI/PlayerWidget/GAS/Attribute/MAttributeWidget.h"

#include "MAICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartMove);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDetectPlayer, AActor*, Bot, AActor*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIDeath, FVector, Position);

UCLASS()
class MULTIPLAYER_API AMAICharacter : public AMBaseCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* AIWidgetComponent; 

	FStartMove StartMoveDelegate;

	FDetectPlayer DetectPlayerDelegate;

	FAIDeath AIDeathDelegate;

private:
	AMPatrolPath* PatrolPath = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_HealthValue)
	float HealthValue;

public:
	// Sets default values for this character's properties
	AMAICharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartMove(AMPatrolPath* NewPatrolPath);

	void FindNextPointAndIndex(FVector& NextPoint, int& Index);

	USplineComponent* GetSplineFromPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void InitializeAttributes() override;

private:
	void OnHealthUpdated(const FOnAttributeChangeData& Data);

	void RotateAIWidgetComponent();

	UFUNCTION(Server, Reliable)
	void SetHealthValue(const float& NewHealthValue);

	UFUNCTION()
	void OnRep_HealthValue();

	void SetPercentForHealthWidget(float NewHealthPercent);
};
