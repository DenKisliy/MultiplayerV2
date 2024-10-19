// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Move/MPatrolPath.h"

#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "../../GameFramework/MCharacter.h"
#include "../../UI/PlayerWidget/GAS/Attribute/MAttributeWidget.h"

#include "MAIBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartMove);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDetectPlayer, AActor*, Bot, AActor*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIDeath, FVector, Position);

UCLASS()
class MULTIPLAYER_API AMAIBaseCharacter : public AMCharacter
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

public:
	// Sets default values for this character's properties
	AMAIBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

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
};
