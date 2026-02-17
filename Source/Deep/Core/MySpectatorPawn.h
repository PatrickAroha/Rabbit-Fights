// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "MySpectatorPawn.generated.h"

UCLASS()
class DEEP_API AMySpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMySpectatorPawn();
	
	/** Move camera to next player */
	void ViewNextPlayer();

	/** Move camera to previous player */
	void ViewPrevPlayer();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
