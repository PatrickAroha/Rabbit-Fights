#pragma once

#include "CoreMinimal.h"
#include "Engine"
#include "ItemDefinition.generated.h"



UCLASS(BlueprintType)
class DEEP_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stack")
	bool bStackable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stack", meta=(ClampMin="1"))
	int32 MaxStack = 99;
};