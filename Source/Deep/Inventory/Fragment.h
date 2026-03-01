#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Fragment.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class DEEP_API UFragment : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitActivate(APawn* OwnerPawn, UItemInstance* Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitDeactivate(APawn* OwnerPawn, UItemInstance* Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActiveItem(APawn* OwnerPawn, UItemInstance* Item);
};