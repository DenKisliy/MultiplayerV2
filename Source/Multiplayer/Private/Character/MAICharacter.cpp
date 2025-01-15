// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/MAICharacter.h"

// Sets default values
AMAICharacter::AMAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("AI Widget"));
	AIWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMAICharacter::FindNextPointAndIndex(FVector& NextPoint, int& Index)
{
	if (PatrolPath)
	{
		PatrolPath->FindNextPointAndIndex(NextPoint, Index);
	}
}

USplineComponent* AMAICharacter::GetSplineFromPath()
{
	if (PatrolPath)
	{
		return PatrolPath->SplineComponent;
	}
	return nullptr;
}

// Called when the game starts or when spawned
void AMAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AIWidgetComponent->GetWidget()))
	{
		if (UMAttributeWidget* widget = Cast<UMAttributeWidget>(AIWidgetComponent->GetWidget()))
		{
			widget->SetColor(FLinearColor::Red);
			widget->SetPercent(1.0f);
		}
	}
}

void AMAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateAIWidgetComponent();
}

void AMAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMAICharacter, HealthValue);
}

void AMAICharacter::StartMove(AMPatrolPath* NewPatrolPath)
{
	PatrolPath = NewPatrolPath;
	StartMoveDelegate.Broadcast();
}

void AMAICharacter::InitializeAttributes()
{
	Super::InitializeAttributes();

	if (AbilitySystemComponent && StartupEffects.Num() > 0)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetHealthAttribute()).AddUObject(this, &AMAICharacter::OnHealthUpdated);
	}
}

void AMAICharacter::OnHealthUpdated(const FOnAttributeChangeData& Data)
{
	SetHealthValue(Data.NewValue / Attributes->GetMaxHealth());

	if (Data.NewValue == 0)
	{
		AIDeathDelegate.Broadcast(GetActorLocation());
		Destroy();
	}
}

void AMAICharacter::RotateAIWidgetComponent()
{
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(AIWidgetComponent->GetComponentLocation(),
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation());
	AIWidgetComponent->SetWorldRotation(rotator);
}

void AMAICharacter::SetHealthValue_Implementation(const float& NewHealthValue)
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_ListenServer)
	{
		SetPercentForHealthWidget(NewHealthValue);
	}

	HealthValue = NewHealthValue;
}

void AMAICharacter::OnRep_HealthValue()
{
	SetPercentForHealthWidget(HealthValue);
}

void AMAICharacter::SetPercentForHealthWidget(float NewHealthPercent)
{
	if (UMAttributeWidget* widget = Cast<UMAttributeWidget>(AIWidgetComponent->GetWidget()))
	{
		widget->SetPercent(NewHealthPercent);
	}
}
