// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MAttackPlayerTask.h"

EBTNodeResult::Type UMAttackPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;
	
	if (!BehaviorTreeComponent && !AbilityTags.IsEmpty())
	{
		BehaviorTreeComponent = &OwnerComp;
		
		if (AMAICharacter* OwnerCharacter = Cast<AMAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			if (UAbilitySystemComponent* AbilitySystem = OwnerCharacter->GetAbilitySystemComponent())
			{
				OwnerCharacter->EndAbilityDelegate.Clear();
				OwnerCharacter->EndAbilityDelegate.AddDynamic(this, &UMAttackPlayerTask::OnEndAbility);

				NodeResult = AbilitySystem->TryActivateAbilitiesByTag(AbilityTags)
					? EBTNodeResult::InProgress : EBTNodeResult::Failed;
			}
		}
	}

	return NodeResult;
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
