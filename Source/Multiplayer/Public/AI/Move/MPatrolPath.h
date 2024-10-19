// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"

#include "MPatrolPath.generated.h"

UCLASS()
class MULTIPLAYER_API AMPatrolPath : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

private:
	bool bReverseMove = false;

	APawn* OwnerPawn;

public:	
	// Sets default values for this actor's properties
	AMPatrolPath();

	void FindNextPointAndIndex(FVector& NextPoint, int& Index);

	bool IsSetOwner();

	FVector GetStartPoint();

	FRotator GetRotatorForStartPoint();

private:
	void SetOwnerPawn(APawn* NewOwnerPawn);

};
