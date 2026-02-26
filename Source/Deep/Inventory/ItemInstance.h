#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

class UFragment;
class UItemDefinition;

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class DEEP_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(ReplicatedUsing=OnRep_Quantity, EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float Durability = 10.f;
	
	UPROPERTY(ReplicatedUsing=OnRep_Def, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDefinition> Def = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category="Fragments")
	TArray<TObjectPtr<UFragment>> Fragments;

public:
	
	void SetQuantity(int32 NewQty);
	virtual void ActivateFragments(APawn* OwnerPawn);
	virtual void DeactivateFragments(APawn* OwnerPawn);
	
	
	//Helper And replicate ----------------------------------------------------------------------------------------------------

	UFUNCTION() void OnRep_Def();
	UFUNCTION() void OnRep_Quantity();
	void NotifyChanged();
	void CopyFrom(const UItemInstance* Src);
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UWorld* GetWorld() const override
	{
		return GetOuter() ? GetOuter()->GetWorld() : nullptr;
	}

};
