#pragma once
#include "CoreMinimal.h"
#include "GamemodeData.generated.h"

UENUM(BlueprintType)
enum class EMaskType : uint8
{
	NONE,
	Fire,
	Grass,
	Mud,
};