#pragma once
#include "CoreMinimal.h"
#include "HUDTypes.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	Spectating,
	Inactive
};