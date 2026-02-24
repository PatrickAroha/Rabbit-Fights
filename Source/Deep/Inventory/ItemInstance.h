// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

class UItemDefinition;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DEEP_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:

	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite) TObjectPtr<UItemDefinition> Def; //DA
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite) int32 Quantity = 1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite) float Durability = 10.f;
	
	void Init(UItemDefinition* InDef, int32 InQty);

	//Setup & Replicate
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UWorld* GetWorld() const override
	{
		return GetOuter() ? GetOuter()->GetWorld() : nullptr;
	}
};
