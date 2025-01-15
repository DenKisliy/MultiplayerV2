// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "../../GAS/Abilities/MBaseAbility.h"

#include "MBiteNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMBiteNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	bool bBiteCharacter = false;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
