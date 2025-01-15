// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MAttackPlayerTask.h"

EBTNodeResult::Type UMAttackPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type nodeResult = EBTNodeResult::Failed;
	
	if (!BehaviorTreeComponent && !AbilityTags.IsEmpty())
	{
		BehaviorTreeComponent = &OwnerComp;
		
		if (AMAICharacter* ownerCharacter = Cast<AMAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			if (UAbilitySystemComponent* abilitySystem = ownerCharacter->GetAbilitySystemComponent())
			{
				ownerCharacter->EndAbilityDelegate.Clear();
				ownerCharacter->EndAbilityDelegate.AddDynamic(this, &UMAttackPlayerTask::OnEndAbility);

				nodeResult = abilitySystem->TryActivateAbilitiesByTag(AbilityTags)
					? EBTNodeResult::InProgress : EBTNodeResult::Failed;
			}
		}
	}

	return nodeResult;
}

void UMAttackPlayerTask::OnEndAbility()
{
	if (BehaviorTreeComponent)
	{
		UBTTaskNode* TemplateNode = Cast<UBTTaskNode>(BehaviorTreeComponent->FindTemplateNode(this));
		BehaviorTreeComponent->OnTaskFinished(TemplateNode, EBTNodeResult::Succeeded);
		BehaviorTreeComponent = nullptr;
	}
}
