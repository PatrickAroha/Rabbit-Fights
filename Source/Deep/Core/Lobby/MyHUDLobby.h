// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MyHUDLobby.generated.h"

/**
 * 
 */
UCLASS()
class DEEP_API AMyHUDLobby : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void CreateHudLobby(APlayerState* PS);

};
