// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EResultOfGame : uint8
{
	None = 0,
	Win,
	Lost
};

UENUM(BlueprintType)
enum class ETypeOfCharacter : uint8
{
	None = 0,
	Greystone,
	Fey,
	Max
};

UENUM(BlueprintType)
enum class ETypeOfAI : uint8
{
	None = 0,
	Wolf
};

UENUM(BlueprintType)
enum class ETypeOfSpawnItem : uint8
{
	AIDeath,
	CaptureStation
};

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Health,
	Mana,
	Stamina
};

UENUM(BlueprintType)
enum class ETypeOfTimer : uint8
{
	None,
	StartMatch,
	CaptureStation
};

UENUM(BlueprintType)
enum class ETypeOfAdditionalTimer : uint8
{
	None,
	ResurrectionPlayer
};


