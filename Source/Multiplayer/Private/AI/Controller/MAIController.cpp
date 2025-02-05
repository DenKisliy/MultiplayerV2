// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/MAIController.h"
#include "../../../Public/Character/MPlayerCharacter.h"

AMAIController::AMAIController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sensesight");
	Sight->SightRadius = 100;
	Sight->LoseSightRadius = 150;
	Sight->PeripheralVisionAngleDegrees = 90;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectEnemies = true;

	UAISenseConfig_Hearing* Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	Hearing->HearingRange = 100;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->ConfigureSense(*Hearing);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMAIController::OnPerception);

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AMAIController::SetStartDate()
{
	if (AMAICharacter* Bot = Cast<AMAICharacter>(GetPawn()))
	{
		OwnerCharacter = Bot;
		OwnerCharacter->StartMoveDelegate.AddDynamic(this, &AMAIController::LaunchBehaviorTree);
	}
}

void AMAIController::LaunchBehaviorTree()
{
	if (BTPatrol)
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "LaunchBehaviorTree");
		OwnerCharacter->StartMoveDelegate.Remove(Delegate);
		OwnerCharacter->SetCharacterSpeed(-1);
		RunBehaviorTree(BTPatrol);
	}
}

void AMAIController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (BTStalkingAndAttack)
	{
		if (InitializeBlackboardComponent(BTStalkingAndAttack))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				if (AMPlayerCharacter* Player = Cast<AMPlayerCharacter>(Actor))
				{
					if (AMPlayerState* PlayerCharcterState = Cast<AMPlayerState>(Player->GetPlayerState()))
					{
						if (!PlayerCharcterState->IsInSaveZone())
						{
							BlackboardComponent->SetValueAsObject("DetectPlayer", Actor);
							if (!Player->InsertInSaveZoneDelegate.IsBound())
							{
								Player->InsertInSaveZoneDelegate.AddDynamic(this, &AMAIController::OnDetectPlayerInSaveZone);
							}
							OwnerCharacter->DetectPlayerDelegate.Broadcast(OwnerCharacter, Player);
							OwnerCharacter->SetCharacterSpeed(Player->GetCharacterMovement()->GetMaxSpeed() / OwnerCharacter->GetCharacterMovement()->GetMaxSpeed());
							RunBehaviorTree(BTStalkingAndAttack);
						}
					}
				}
			}
			else
			{
				BlackboardComponent->SetValueAsObject("DetectPlayer", nullptr);
				OwnerCharacter->DetectPlayerDelegate.Broadcast(OwnerCharacter, nullptr);
				OwnerCharacter->SetCharacterSpeed(-1);
				RunBehaviorTree(BTPatrol);
			}
		}
	}
}

bool AMAIController::InitializeBlackboardComponent(UBehaviorTree* BehaviorTree)
{
	if (UBlackboardData* BlackboardData = BTStalkingAndAttack->GetBlackboardAsset())
	{
		BlackboardComponent->InitializeBlackboard(*BlackboardData);
		return true;
	}

	return false;
}

void AMAIController::OnDetectPlayerInSaveZone(AActor* Player)
{
	if (InitializeBlackboardComponent(BTStalkingAndAttack))
	{
		if (AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Player))
		{
			BlackboardComponent->SetValueAsObject("DetectPlayer", nullptr);
			RunBehaviorTree(BTPatrol);
		}
	}
}

void AMAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetStartDate();
}

void AMAIController::FindNextPoint(FVector& NextPoint, int& Index)
{
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->FindNextPointAndIndex(NextPoint, Index);
	}
}

void AMAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AMAIController::GetOwnerLocation()
{
	return OwnerCharacter->GetActorLocation();
}

float AMAIController::GetOwnerSpeed()
{
	return OwnerCharacter->GetCharacterMovement()->GetMaxSpeed();
}

USplineComponent* AMAIController::GetOwnerPath()
{
	return OwnerCharacter->GetSplineFromPath();
}
