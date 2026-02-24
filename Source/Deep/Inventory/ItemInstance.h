// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DEEP_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override
	{
		return GetOuter() ? GetOuter()->GetWorld() : nullptr;
	}

	UPROPERTY(Replicated) TObjectPtr<class UItemDefinition> Def; // seu DA
	UPROPERTY(Replicated) int32 Quantity = 1;
	UPROPERTY(Replicated) float Durability = 1.f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const override
	{
		Super::GetLifetimeReplicatedProps(Out);
		DOREPLIFETIME(UItemInstance, Def);
		DOREPLIFETIME(UItemInstance, Quantity);
		DOREPLIFETIME(UItemInstance, Durability);
	}
};
