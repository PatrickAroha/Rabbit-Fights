#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Fragment.generated.h"

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class DEEP_API UFragment : public UObject
{
	GENERATED_BODY()

public:
	
	virtual void InitActivate(APawn* OwnerPawn, class UItemInstance* Item) {}
	virtual void InitDeactivate(APawn* OwnerPawn, class UItemInstance* Item) {}
	virtual void ActiveItem(APawn* OwnerPawn, class UItemInstance* Item) {}
};
