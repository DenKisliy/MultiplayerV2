// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MPlayerState.h"
#include "../../Public/Subsystem/MPlayerInfoSubsystem.h"

AMPlayerState::AMPlayerState()
{
	NetUpdateFrequency = 30.0f;
}

void AMPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMPlayerState::SetPlayerNameByLogin()
{
	if (UMPlayerInfoSubsystem* PlayerInfoSubsystem = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
	{
		if (!PlayerInfoSubsystem->GetLoginOfUser().IsEmpty())
		{
			SetPlayerName(PlayerInfoSubsystem->GetLoginOfUser());
		}
	}
}
