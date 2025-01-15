// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MPlayerController.h"
#include "../../Public/GameFramework/MPlayerHUD.h"

AMPlayerController::AMPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
}

void AMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	OnSetCharacterType();
}

void AMPlayerController::CharacterDeath()
{
	if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		FPlayerInfoStruct& playerInfo = gameInstance->GetPlayerInfoFromGameInstance();
		playerInfo.SetCharacterDeath(true);

		PlayerHUD->SetRemoveOrAddAttributesGroupWidget(false);
		SpawnResurrectionFlower(true);
	}

}

FPlayerInfoStruct& AMPlayerController::GetPlayerInfo()
{
	FPlayerInfoStruct playerInfo;
	if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		return gameInstance->GetPlayerInfoFromGameInstance();
	}
	
	return playerInfo;
}

void AMPlayerController::ResurrectionCharacter()
{
	if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		if (AMPlayerState* pState = Cast<AMPlayerState>(PlayerState))
		{
			FPlayerInfoStruct& playerInfo = pState->GetPlayerInfo();

			if (playerInfo.CharacterType != ETypeOfCharacter::None)
			{
				TypeOfCharacter = playerInfo.CharacterType;
				playerInfo.SetCharacterDeath(!playerInfo.bCharacterDeath);
				SpawnCharacter(playerInfo.CharacterType);
			}
		}
	}
}

void AMPlayerController::AddItemToCharacter_Implementation(const FItemTypeInfo& ItemType)
{
	AddItemToInventoryDelegate.Broadcast(ItemType);
}

void AMPlayerController::SetPickUpItemToInventory_Implementation(const FItemTypeInfo& ItemType, const AActor* ItemActor)
{
	PickUpItemToInventoryDelegate.Broadcast(ItemType, ItemActor);
}

FString AMPlayerController::GetPlayerStartByTag()
{
	int32 index;
	int findIndex = 0;
	if (GetName().FindLastChar('_', index))
	{
		FString indexText = "";
		for (int i = index + 1; i < GetName().Len(); i++)
		{
			indexText = indexText + GetName()[i];
		}
		return FString::FromInt(FCString::Atoi(*indexText) + 1);
	}

	return FString();
}

void AMPlayerController::OnSetCharacterType()
{
	if (IsLocalController())
	{
		if (IsValid(GetGameInstance()))
		{
			if (IsValid(GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>()))
			{
				int CharacterTypeIndex = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>()->GetCharacterType();

				if (ETypeOfCharacter(CharacterTypeIndex) != ETypeOfCharacter::None)
				{
					UE_LOG(LogTemp, Warning, TEXT("Get CharacterTypeIndex  %d"), CharacterTypeIndex);
					if (AMPlayerState* playerState = Cast<AMPlayerState>(PlayerState))
					{
						playerState->PlayerDeathDelegate.Broadcast(false);
					}

					TypeOfCharacter = ETypeOfCharacter(CharacterTypeIndex);

					SpawnCharacter(TypeOfCharacter);
				}
			}
		}
	}
}

void AMPlayerController::SetPlayerHUD()
{
	if (Cast<AMPlayerHUD>(GetHUD()))
	{
		PlayerHUD = Cast<AMPlayerHUD>(GetHUD());

		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "SubmitMessage");

		//PlayerHUD->SetDelegateForSendMessageEvent(Delegate);

		PlayerHUD->SetRemoveOrAddAttributesGroupWidget(true);
	}
}

void AMPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	//if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	//{
	//	FPlayerInfoStruct& playerInfo = gameInstance->GetPlayerInfoFromGameInstance();
	//	if (playerInfo.bCharacterDeath || !SpawnLocationAfterDeath.IsZero())
	//	{
	//		aPawn->SetActorLocation(SpawnLocationAfterDeath);
	//	}
	//}
}

void AMPlayerController::SpawnCharacter_Implementation(ETypeOfCharacter SelectTypeOfCharacter)
{
	TypeOfCharacter = SelectTypeOfCharacter;
	if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		FPlayerInfoStruct& playerInfo = gameInstance->GetPlayerInfoFromGameInstance();
		playerInfo.SetCharacterType(SelectTypeOfCharacter);
		playerInfo.SetCharacterDeath(false);

		if (GetWorld())
		{
			if (IsValid(GetPawn()))
			{
				GetPawn()->Destroy();
			}

			if (AMPlayerState* playerState = Cast<AMPlayerState>(PlayerState))
			{
				playerState->PlayerDeathDelegate.Broadcast(false);
			}

			if (AActor* playerStart = GetWorld()->GetAuthGameMode()->FindPlayerStart(this, GetPlayerStartByTag()))
			{
				GetWorld()->GetAuthGameMode()->RestartPlayerAtPlayerStart(this, playerStart);
			}
			else
			{
				GetWorld()->GetAuthGameMode()->RestartPlayer(this);
			}
		}
	}
}

void AMPlayerController::MessageSubmit_Implementation()
{
	if (GetWorld()->GetGameState())
	{
		for (auto PlayerStateInfo : GetWorld()->GetGameState()->PlayerArray)
		{
			if (APlayerState* State = Cast<APlayerState>(PlayerStateInfo.Get()))
			{
				if (AMPlayerController* PlayerController = Cast<AMPlayerController>(State->GetPlayerController()))
				{
					PlayerController->UpdateChat();
				}
			}
		}
	}
}

void AMPlayerController::UpdateChat_Implementation()
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(GetHUD()))
	{
		HUD->ChatWidget.Get()->UpdateChatBox();
	}
}

void AMPlayerController::SpawnResurrectionFlower_Implementation(bool bValue)
{
	if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		FPlayerInfoStruct& playerInfo = gameInstance->GetPlayerInfoFromGameInstance();
		playerInfo.SetCharacterDeath(bValue);

		if (IsValid(GetPawn()))
		{
			SpawnLocationAfterDeath = GetPawn()->GetActorLocation();
			GetPawn()->Destroy();
		}

		if (AMPlayerState* playerState = Cast<AMPlayerState>(PlayerState))
		{
			playerState->PlayerDeathDelegate.Broadcast(true);
		}

		GetWorld()->GetAuthGameMode()->RestartPlayer(this);
	}
}

void AMPlayerController::ShowInfoForCharacter_Implementation(const FString& Text)
{
	if (IsValid(PlayerHUD))
	{
		PlayerHUD->ShowInformText(Text);
	}
}
