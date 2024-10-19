// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "../Controller/MAIController.h"

#include "MIsNotReachPointDecorator.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMIsNotReachPointDecorator : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Condition, EditAnywhere)
	int32 Distance = 25;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
