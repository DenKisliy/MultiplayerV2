// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../GameFramework/MCharacter.h"

#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "../UI/MPlayerInfoWidget.h"
#include "../GameFramework/MPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DataTable.h"
#include "Animation/AnimInstance.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "../GAS/Attribute/MAttributeSet.h"
#include "../GAS/Abilities/MBaseAbility.h"

#include "GameFramework/MPlayerHUD.h"
#include "CharacterComponents/MInventoryComponent.h"

#include "MBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class MULTIPLAYER_API AMBaseCharacter : public AMCharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SpawnArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PlayerTagComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UMInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	FInsertInSaveZone InsertInSaveZoneDelegate;

private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerNameWidget)
	FString PlayerNameWidget;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerScaleVector)
	FVector PlayerScaleVector;

	UPROPERTY(ReplicatedUsing = OnRep_WalkSpeed)
	float WalkSpeed;

public:
	// Sets default values for this character's properties
	AMBaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	AMPlayerHUD* GetHUD();

	FString GetPlayerName();

	FString GetButtonTextForInformWidget(FString ButtonName);

	//Multicast function
	UFUNCTION(Server, Reliable)
	void SetPlayerScale(const FVector& NewPlayerScale);

	UFUNCTION(Server, Reliable)
	void SetPlayerSpeed(const float& NewSpeed);

	float GetTargetArmLength();

	FVector GetSpawnActorLocation();

	FRotator GetSpawnActorRotation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void InitializeAttributes() override;

private:
	void InitializeInput(AController* NewController);

	void StartTimer();

	void StartPlayerWidgetTimer(FString PlayerName);

	void RotatePlayerNameWidget();

	void BindAllDelegates();

	void OnHealthUpdated(const FOnAttributeChangeData& Data);

	void OnManaUpdated(const FOnAttributeChangeData& Data);

	void OnStaminaUpdated(const FOnAttributeChangeData& Data);

	void SetLocalPlayerTag();

	//Bind function
	UFUNCTION()
	void SetPlayerTagName(FString PlayerName);

	UFUNCTION()
	void OnRep_PlayerScaleVector();

	UFUNCTION()
	void OnRep_WalkSpeed();

	UFUNCTION()
	void OnRep_PlayerNameWidget();

	UFUNCTION(Server, Reliable)
	void SetPlayerName(const FString& NewPlayerName);

	UFUNCTION()
	void UseItemGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);

	UFUNCTION()
	void OnAddItemToInventory(FItemTypeInfo Type);

	UFUNCTION()
	void PickUpItemToInventory(FItemTypeInfo ItemType, const AActor* Item);
};
