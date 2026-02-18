// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEEP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Match")
	int32 DefinePlayersConnected = 0;

	UFUNCTION(BlueprintCallable)
	void DefinePlayersConecteds(int32 PlayersQuantity);
	
};
