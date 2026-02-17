// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "GameFramework/PlayerState.h"

void AMyPlayerController::OnRep_Pawn()
{
	UE_LOG(LogTemp, Warning, TEXT("[OnRep_Pawn] Local=%d | HasAuthority=%d | State=%s"),
		IsLocalController() ? 1 : 0,
		HasAuthority() ? 1 : 0,
		*StateName.ToString());
		
	Super::OnRep_Pawn();

	if (IsInState(NAME_Spectating))
	{
		ServerViewNextPlayer();
	}
}

void AMyPlayerController::SetPlayerPlay()
{
	// Only proceed if we're on the server
	if (!HasAuthority())
	{
		return;
	}

	// Update the state on server
	PlayerState->SetIsSpectator(false);
	ChangeState(NAME_Playing);

	bPlayerIsWaiting = false;

	// Push the state update to the client
	ClientGotoState(NAME_Playing);

	// Update the HUD to remove the spectator screen
	ClientHUDStateChanged(EHUDState::Playing);
}

void AMyPlayerController::SetPlayerSpectate()
{
	// Only proceed if we're on the server
	if (!HasAuthority())
	{
		return;
	}

	// Update the state on server
	PlayerState->SetIsSpectator(true);
	ChangeState(NAME_Spectating);

	StartSpectatingOnly();
	
	bPlayerIsWaiting = true;

	// Push the state update to the client
	ClientGotoState(NAME_Spectating);

	// Update the HUD to add the spectator screen
	ClientHUDStateChanged(EHUDState::Spectating);
}

void AMyPlayerController::BP_ViewPrevPlayer()
{
	ServerViewPrevPlayer();
}

void AMyPlayerController::BP_ViewNextPlayer()
{
	ServerViewNextPlayer();
}

void AMyPlayerController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	if (AMyHUD* HUD = GetHUD<AMyHUD>())
	{
		HUD->OnStateChanged(NewState);
	}
}
