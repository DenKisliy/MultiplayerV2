// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "../Controller/MAIController.h"
#include "Math/UnrealMathUtility.h"

#include "MIsDistanceEnoughForAttack.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMIsDistanceEnoughForAttack : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Condition, EditAnywhere, meta = (ClampMin = "0.0"))
	int32 MinDistance = 0;

	UPROPERTY(Category = Condition, EditAnywhere, meta = (ClampMin = "100.0"))
	int32 MaxDistance = 100;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
