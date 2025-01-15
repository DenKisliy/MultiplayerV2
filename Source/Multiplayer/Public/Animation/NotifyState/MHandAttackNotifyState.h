// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "MHandAttackNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMHandAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	bool bHitByCharacter = false;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
};
