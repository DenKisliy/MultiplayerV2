// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MPlayerController.h"
#include "../Public/GameFramework/HUD/MPlayingHUD.h"

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
	if (UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		FPlayerInfoStruct& PlayerInfo = GameInstance->GetPlayerInfoFromGameInstance();
		PlayerInfo.SetCharacterDeath(true);

		if (AMPlayingHUD* HUD = Cast<AMPlayingHUD>(GetHUD()))
		{
			HUD->SetRemoveOrAddAttributesGroupWidget(false);
		}
		SpawnResurrectionFlower(true);
	}

}

FPlayerInfoStruct& AMPlayerController::GetPlayerInfo()
{
	FPlayerInfoStruct PlayerInfo = FPlayerInfoStruct();

	if (UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		return GameInstance->GetPlayerInfoFromGameInstance();
	}
	
	return PlayerInfo;
}

void AMPlayerController::ResurrectionCharacter_Implementation()
{
	if (TypeOfCharacter != ETypeOfCharacter::None)
	{
		SpawnCharacter(TypeOfCharacter);
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
	int32 Index;
	int FindIndex = 0;
	if (GetName().FindLastChar('_', Index))
	{
		FString IndexText = "";
		for (int i = Index + 1; i < GetName().Len(); i++)
		{
			IndexText = IndexText + GetName()[i];
		}
		return FString::FromInt(FCString::Atoi(*IndexText) + 1);
	}

	return FString();
}

void AMPlayerController::OnSetCharacterType()
{
	if (TypeOfCharacter == ETypeOfCharacter::None)
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
						if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(PlayerState))
						{
							CharacterPS->PlayerDeathDelegate.Broadcast(false);
						}

						TypeOfCharacter = ETypeOfCharacter(CharacterTypeIndex);

						SpawnCharacter(TypeOfCharacter);
					}
				}
			}
		}
	}
}

void AMPlayerController::SetPlayerHUD()
{
	if (AMPlayingHUD* HUD = Cast<AMPlayingHUD>(GetHUD()))
	{
		HUD->SetRemoveOrAddAttributesGroupWidget(true);
	}
}

void AMPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
}

void AMPlayerController::SetPlayerNameAfterSpawn()
{
	if (IsLocalController())
	{
		if (IsValid(GetGameInstance()) && IsValid(PlayerState))
		{
			if (UMPlayerInfoSubsystem* PlayerInfoSubsystem = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
			{
				PlayerState->SetPlayerName(PlayerInfoSubsystem->GetLoginOfUser());
			}
		}
	}
}

void AMPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	OnSetCharacterType();

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
	if (UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		FPlayerInfoStruct& PlayerInfo = GameInstance->GetPlayerInfoFromGameInstance();
		PlayerInfo.SetCharacterType(SelectTypeOfCharacter);
		PlayerInfo.SetCharacterDeath(false);

		if (GetWorld())
		{
			if (IsValid(GetPawn()))
			{
				GetPawn()->Destroy();
			}

			if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(PlayerState))
			{
				CharacterPS->PlayerDeathDelegate.Broadcast(false);
			}

			if (AActor* PlayerStart = GetWorld()->GetAuthGameMode()->FindPlayerStart(this, GetPlayerStartByTag()))
			{
				GetWorld()->GetAuthGameMode()->RestartPlayerAtPlayerStart(this, PlayerStart);
			}
			else
			{
				GetWorld()->GetAuthGameMode()->RestartPlayer(this);
			}
		}
	}
}

void AMPlayerController::SpawnResurrectionFlower_Implementation(bool bValue)
{
	if (UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		FPlayerInfoStruct& PlayerInfo = GameInstance->GetPlayerInfoFromGameInstance();
		PlayerInfo.SetCharacterDeath(bValue);

		if (IsValid(GetPawn()))
		{
			SpawnLocationAfterDeath = GetPawn()->GetActorLocation();
			GetPawn()->Destroy();
		}

		if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(PlayerState))
		{
			CharacterPS->PlayerDeathDelegate.Broadcast(true);
		}

		GetWorld()->GetAuthGameMode()->RestartPlayer(this);
	}
}

void AMPlayerController::ShowInfoForCharacter_Implementation(const FString& Text)
{
	/*if (IsValid(PlayerHUD))
	{
		PlayerHUD->ShowInformText(Text);
	}*/
}

void AMPlayerController::MessageSubmit_Implementation()
{
	if (IsValid(GetWorld()->GetGameState()))
	{
		for (auto PlayerStateInfo : GetWorld()->GetGameState()->PlayerArray)
		{
			if (AMPlayerController* PlayerController = Cast<AMPlayerController>(PlayerStateInfo.Get()->GetPlayerController()))
			{
				PlayerController->OnUpdateChat();
			}
		}
	}
}

void AMPlayerController::OnUpdateChat_Implementation()
{
	UpdateChatWidgetDelegate.ExecuteIfBound();
}

