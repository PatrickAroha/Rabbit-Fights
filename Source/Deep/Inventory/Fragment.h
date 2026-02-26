// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Fragment.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class DEEP_API UFragment : public UObject
{
	GENERATED_BODY()

public:
	
	virtual void Activate(APawn* OwnerPawn, class UItemInstance* Item) {}
	virtual void Deactivate(APawn* OwnerPawn, class UItemInstance* Item) {}
};
