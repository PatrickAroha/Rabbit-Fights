#pragma once

#include "CoreMinimal.h"
#include "E_MagicSlot.generated.h"

UENUM(BlueprintType)
enum class EMagicSlot : uint8
{
	Slot1 UMETA(DisplayName = "Slot 1"),
	Slot2 UMETA(DisplayName = "Slot 2"),
	Slot3 UMETA(DisplayName = "Slot 3"),

	MAX UMETA(Hidden)
};