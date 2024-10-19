#pragma once

#include "CoreMinimal.h"

#include "MCharacterDataStruct.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInsertInSaveZone, AActor*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndAbility);

USTRUCT(BlueprintType)
struct FSpeedInfoStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpeedCoefficient;

	FSpeedInfoStruct()
	{
		DefaultSpeed = 0;
		SpeedCoefficient = 0;
	}

	void SetDefaultSpeed(float NewSpeed)
	{
		DefaultSpeed = NewSpeed;
	}

	float GetDefaultSpeed()
	{
		return DefaultSpeed;
	}

	float GetNewSpeed(float Coefficient)
	{
		return DefaultSpeed * ( Coefficient == 0 ? SpeedCoefficient : Coefficient );
	}
};