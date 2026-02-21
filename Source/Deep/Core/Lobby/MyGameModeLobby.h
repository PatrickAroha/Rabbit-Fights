// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyGameModeLobby.generated.h"

/**
 * 
 */
UCLASS()
class DEEP_API AMyGameModeLobby : public AGameMode
{
	GENERATED_BODY()


public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby")
	void PlayerReady();
	
};
