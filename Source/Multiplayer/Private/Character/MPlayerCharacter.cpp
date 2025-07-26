// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Character/MPlayerCharacter.h"
#include "../../Public/GameFramework/HUD/MPlayingHUD.h"
#include "../../Public/Subsystem/MPlayerInfoSubsystem.h"
#include "../../Public/GameFramework/MPlayerState.h"

// Sets default values
AMPlayerCharacter::AMPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 110.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	//400
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SpawnArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	if (SpawnArrowComponent)
	{
		SpawnArrowComponent->ArrowColor = FColor(255, 0, 0);
		SpawnArrowComponent->bTreatAsASprite = true;
		SpawnArrowComponent->bIsScreenSizeScaled = true;
		SpawnArrowComponent->SetupAttachment(RootComponent);
	}

	PlayerNameComponent = CreateDefaultSubobject<UTextRenderComponent>(FName("Player Name"));
	PlayerNameComponent->SetText(FText::FromString(""));
	PlayerNameComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	InventoryComponent = CreateDefaultSubobject<UMInventoryComponent>(TEXT("Inventory"));
}

FString AMPlayerCharacter::GetPlayerName()
{
	return PlayerNameComponent->Text.ToString();
}

FString AMPlayerCharacter::GetButtonTextForInformWidget(FString ButtonName)
{
	FString Result = "";
	TArray<FString> Components;
	const FString DefaultInputPath = FString::Printf(TEXT("%sDefaultInput.ini"), *FPaths::SourceConfigDir());
	GConfig->GetArray(TEXT("/Script/Engine.InputSettings"), TEXT("+ActionMappings"), Components, DefaultInputPath);
	
	FString FindStr = *Components.FindByPredicate([](const FString& InItem)
		{
			return InItem.Contains("PickUp");
		});

	if (!FindStr.IsEmpty())
	{
		for (int32 j = FindStr.Find("Key=") + 4; j < FindStr.Len() - 1; j++)
		{
			Result += FindStr[j];
		}
	}

	return Result;
}

// Called when the game starts or when spawned
void AMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitializeInput(NewController);
}

// Called every frame
void AMPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotatePlayerNameWidget();
}

// Called to bind functionality to input
void AMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::Look);

		if (AbilitySystemComponent)
		{
			FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/Multiplayer"), FName("EGDAbilityInputID"));
			AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("Confirm"),
				FString("Cancel"), AbilityEnumAssetPath, static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel)));
		}

		FInputActionBinding PickUp("PickUp", IE_Pressed);

		PickUp.ActionDelegate.GetDelegateForManualSet().BindLambda([this]()
			{
				InventoryComponent->AddPickUpItemToInventory();
			});

		PlayerInputComponent->AddActionBinding(PickUp);
	}
}

void AMPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMPlayerCharacter::OnRep_PlayerNameWidget()
{
	SetPlayerTagName(PlayerNameWidget);
}

void AMPlayerCharacter::SetPlayerCharacterName_Implementation(const FString& NewPlayerName)
{
	if (IsValid(GetWorld()))
	{
		if (GetWorld()->GetNetMode() == ENetMode::NM_ListenServer/* || GetWorld()->GetNetMode() == ENetMode::NM_Standalone*/)
		{
			SetPlayerTagName(NewPlayerName);
		}

		PlayerNameWidget = NewPlayerName;
	}
}

void AMPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPlayerCharacter, PlayerNameWidget);
	DOREPLIFETIME(AMPlayerCharacter, PlayerScaleVector);
	DOREPLIFETIME(AMPlayerCharacter, WalkSpeed);
}

void AMPlayerCharacter::SetPlayerTagName(FString PlayerName)
{
	PlayerNameComponent->SetText(FText::FromString(PlayerName));
}

void AMPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeInput(Controller);
}

void AMPlayerCharacter::OnRep_PlayerScaleVector()
{
	GetCapsuleComponent()->SetRelativeScale3D(PlayerScaleVector);
}

void AMPlayerCharacter::OnRep_WalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMPlayerCharacter::SetPlayerSpeed_Implementation(const float& NewSpeed)
{
	if (IsValid(GetWorld()))
	{
		if (GetWorld()->GetNetMode() == ENetMode::NM_ListenServer)
		{
			GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
		}

		WalkSpeed = NewSpeed;
	}
}

void AMPlayerCharacter::SetPlayerScale_Implementation(const FVector& NewPlayerScale)
{
	if (IsValid(GetWorld()))
	{
		CameraBoom->TargetArmLength = NewPlayerScale == FVector(1) ? CameraBoom->TargetArmLength * GetCapsuleComponent()->GetRelativeScale3D().X :
			CameraBoom->TargetArmLength / GetCapsuleComponent()->GetRelativeScale3D().X;

		if (GetWorld()->GetNetMode() == ENetMode::NM_ListenServer)
		{
			GetCapsuleComponent()->SetRelativeScale3D(NewPlayerScale);
		}

		PlayerScaleVector = NewPlayerScale;
	}
}

void AMPlayerCharacter::InitializeInput(AController* NewController)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(NewController))
	{
		if (PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
			BindAllDelegates();

			if (IsValid(GetPlayerState()))
			{
				UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *GetPlayerState()->GetPlayerName());
			}
		}
	}

}

void AMPlayerCharacter::RotatePlayerNameWidget()
{
	SetUserNameForLocalPlayer();

	PlayerNameComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(PlayerNameComponent->GetComponentLocation(),
			UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation()));
}

void AMPlayerCharacter::InitializeAttributes()
{
	Super::InitializeAttributes();

	if (AbilitySystemComponent && StartupEffects.Num()>0)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetHealthAttribute()).AddUObject(this, &AMPlayerCharacter::OnHealthUpdated);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetManaAttribute()).AddUObject(this, &AMPlayerCharacter::OnManaUpdated);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetStaminaAttribute()).AddUObject(this, &AMPlayerCharacter::OnStaminaUpdated);
	}
}

void AMPlayerCharacter::BindAllDelegates()
{
	if (AMPlayerController* PlayerController = Cast<AMPlayerController>(Controller))
	{
		PlayerController->AddItemToInventoryDelegate.AddDynamic(this, &AMPlayerCharacter::OnAddItemToInventory);
		PlayerController->PickUpItemToInventoryDelegate.AddDynamic(this, &AMPlayerCharacter::PickUpItemToInventory);

		PlayerController->SetPlayerHUD();
	}

	InventoryComponent->UseItemDelegate.AddDynamic(this, &AMPlayerCharacter::UseItemGameplayEffect);
}

void AMPlayerCharacter::OnHealthUpdated(const FOnAttributeChangeData& Data)
{
	OnUpdateAttributeState(EAttributeType::Health, Data.NewValue / Attributes->GetMaxHealth());

	if (Data.NewValue / Attributes->GetMaxHealth() == 0)
	{
		if (AMPlayerController* PlayerController = Cast<AMPlayerController>(Controller))
		{
			PlayerController->CharacterDeath();
		}
	}
}

void AMPlayerCharacter::OnManaUpdated(const FOnAttributeChangeData& Data)
{
	OnUpdateAttributeState(EAttributeType::Mana, Data.NewValue / Attributes->GetMaxMana());
}

void AMPlayerCharacter::OnStaminaUpdated(const FOnAttributeChangeData& Data)
{
	OnUpdateAttributeState(EAttributeType::Stamina, Data.NewValue / Attributes->GetMaxStamina());
}

void AMPlayerCharacter::SetUserNameForLocalPlayer()
{
	if (IsLocallyControlled())
	{
		if (PlayerNameComponent->Text.IsEmpty())
		{
			if (AMPlayerState* PS = Cast<AMPlayerState>(GetController()->PlayerState))
			{
				if (PS->IsUserNameByLogin())
				{
					SetPlayerCharacterName(PS->GetPlayerName());
				}
			}
		}
	}
}

void AMPlayerCharacter::OnUpdateAttributeState(EAttributeType Type, float Value)
{
	if (AMPlayerController* PlayerController = Cast<AMPlayerController>(Controller))
	{
		if (AMPlayingHUD* HUD = Cast<AMPlayingHUD>(PlayerController->GetHUD()))
		{
			HUD->SetValueForAttribute(Type, Value);
		}
	}
}

void AMPlayerCharacter::OnAddItemToInventory(FItemTypeInfo ItemType)
{
	InventoryComponent->IsAddItemToInventory(ItemType);
}

void AMPlayerCharacter::PickUpItemToInventory(FItemTypeInfo ItemType, const AActor* Item)
{
	InventoryComponent->SetPickUpItem(ItemType, Item);
}

void AMPlayerCharacter::UseItemGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (GameplayEffect)
	{
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
