#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GE_ExecuteCalculation.generated.h"

UCLASS()
class DEEP_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};