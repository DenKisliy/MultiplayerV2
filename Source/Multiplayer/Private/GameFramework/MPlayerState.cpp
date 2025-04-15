// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MPlayerState.h"
#include "../../Public/Subsystem/MPlayerInfoSubsystem.h"
#include "../../Public/GameFramework/MPlayerController.h"

AMPlayerState::AMPlayerState()
{
	NetUpdateFrequency = 30.0f;
}

void AMPlayerState::UpdateUserNameByLogin(FString UserLogin)
{
	SetPlayerName(UserLogin);

	bUserNameByLogin = true;
}

void AMPlayerState::SaveResultOfGame_Implementation(EResultOfGame ResultOfGame)
{

}



