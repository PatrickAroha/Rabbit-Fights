// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyHUD.h"
#include "MyPlayerController.generated.h"

UCLASS()
class DEEP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void OnRep_Pawn() override;

	
	UFUNCTION() 
	void TryBindGameState();

	UFUNCTION() 
	void TryCreateHUDIfReady(EMatchPhase Old, EMatchPhase New);

	//UFUNCTION(BlueprintCallable)
	//void TryBindOnGameStart();

	
public:
	
	/** Set Player to play. Should be called only on server */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerPlay();

	/** Set Player to spectate. Should be called only on server */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerSpectate();

	/** Notify HUD of a state change so it shows suitable widgets accordingly */
	UFUNCTION(Reliable, Client)
	void ClientHUDStateChanged(EHUDState NewState);

	//Call in BP
	UFUNCTION(BlueprintCallable, Category="Spectate")
	void BP_ViewPrevPlayer();
	
	UFUNCTION(BlueprintCallable, Category="Spectate")
	void BP_ViewNextPlayer();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateMyHUD();

	
	UPROPERTY(Transient)
	bool bHUDCreated = false;
};
