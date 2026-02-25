#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

class UItemDefinition;

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class DEEP_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(ReplicatedUsing=OnRep_Def, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDefinition> Def = nullptr;
	
	UPROPERTY(ReplicatedUsing=OnRep_Quantity, EditAnywhere, BlueprintReadWrite) int32 Quantity = 1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite) float Durability = 10.f;

	//Setup & Replicate
	UFUNCTION() void OnRep_Def();
	UFUNCTION() void OnRep_Quantity();
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UWorld* GetWorld() const override
	{
		return GetOuter() ? GetOuter()->GetWorld() : nullptr;
	}

	//Helper
	void CopyFrom(const UItemInstance* Src);
};
