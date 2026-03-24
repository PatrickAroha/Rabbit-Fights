#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GMItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundPoint);

class ARabbitCharacterBase;
class AActor;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class DEEP_API UGMItem : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Item")
	FGameplayTag RoundPointType;

	UPROPERTY(BlueprintAssignable, Category="Game Item")
	FRoundPoint OnRoundPoint;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Game Item")
	TObjectPtr<AActor> Carrier = nullptr;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Game Item")
	void MakeRoudPoint();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Game Item")
	void OnCarrierDied(AActor* KillerActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Game Item")
	void OnEquipped();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Game Item")
	void OnUnequipped(AActor* KillerActor);

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};