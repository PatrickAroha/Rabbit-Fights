// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfacePlayerInfo.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterfacePlayerInfo : public UInterface
{
	GENERATED_BODY()
};

class DEEP_API IInterfacePlayerInfo
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Player")
	bool CanDoAction() const;
};
