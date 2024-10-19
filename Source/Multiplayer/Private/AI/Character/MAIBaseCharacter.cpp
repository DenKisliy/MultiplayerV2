// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/MAIBaseCharacter.h"

// Sets default values
AMAIBaseCharacter::AMAIBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("AI Widget"));
	AIWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMAIBaseCharacter::FindNextPointAndIndex(FVector& NextPoint, int& Index)
{
	if (PatrolPath)
	{
		PatrolPath->FindNextPointAndIndex(NextPoint, Index);
	}
}

USplineComponent* AMAIBaseCharacter::GetSplineFromPath()
{
	if (PatrolPath)
	{
		return PatrolPath->SplineComponent;
	}
	return nullptr;
}

// Called when the game starts or when spawned
void AMAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (UMAttributeWidget* widget = Cast<UMAttributeWidget>(AIWidgetComponent->GetWidget()))
	{
		widget->SetColor(FLinearColor::Red);
		widget->SetPercent(1.0f);
	}
}

void AMAIBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateAIWidgetComponent();
}

void AMAIBaseCharacter::StartMove(AMPatrolPath* NewPatrolPath)
{
	PatrolPath = NewPatrolPath;
	StartMoveDelegate.Broadcast();
}

void AMAIBaseCharacter::InitializeAttributes()
{
	Super::InitializeAttributes();

	if (AbilitySystemComponent && StartupEffects.Num() > 0)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetHealthAttribute()).AddUObject(this, &AMAIBaseCharacter::OnHealthUpdated);
	}
}

void AMAIBaseCharacter::OnHealthUpdated(const FOnAttributeChangeData& Data)
{
	if (UMAttributeWidget* widget = Cast<UMAttributeWidget>(AIWidgetComponent->GetWidget()))
	{
		widget->SetPercent(Data.NewValue / Attributes->GetMaxHealth());
	}

	if (Data.NewValue == 0)
	{
		AIDeathDelegate.Broadcast(GetActorLocation());
		Destroy();
	}
}

void AMAIBaseCharacter::RotateAIWidgetComponent()
{
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(AIWidgetComponent->GetComponentLocation(),
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation());
	AIWidgetComponent->SetWorldRotation(rotator);
}
