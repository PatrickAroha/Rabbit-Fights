// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerControllerLobby.generated.h"

/**
 * 
 */
UCLASS()
class DEEP_API AMyPlayerControllerLobby : public APlayerController
{
	GENERATED_BODY()

	virtual void InitPlayerState() override;
	
	virtual void OnRep_PlayerState() override;
	
	virtual void SpawnDefaultHUD() override;

	virtual void BeginPlay() override;
};
