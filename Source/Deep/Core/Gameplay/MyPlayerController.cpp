// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "ContentStreaming.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "MyGameState.h"
#include "MyHUD.h"
#include "GameFramework/PlayerState.h"

class AMyGameState;

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;
	
	TryBindGameState();
	
	AMyGameState* GS = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (GS)
	{
		TryCreateHUDIfReady(GS->MatchPhase, GS->MatchPhase);
	}

	StartMapStreamingCheck();
}

void AMyPlayerController::TryBindGameState()
{
	AMyGameState* GS = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (!GS) return;

	GS->OnMatchPhaseChanged.RemoveAll(this);
	GS->OnMatchPhaseChanged.AddDynamic(this, &AMyPlayerController::TryCreateHUDIfReady);
}

void AMyPlayerController::TryCreateHUDIfReady(EMatchPhase Old, EMatchPhase New)
{
	if (bHUDCreated) return;
	
	if (New == EMatchPhase::IntroducingMinigame)
	{
		bHUDCreated = true;
		CreateMyHUD();
	}
}


// Spectator Mode Functions ---------------------------------------------------------------------------------------------------

void AMyPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (IsInState(NAME_Spectating))
	{
		ServerViewNextPlayer();
		OnPossessedPawn();
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

//------------------ CLAUDE + GPT ---------------------------------
void AMyPlayerController::StartMapStreamingCheck()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	IRenderAssetStreamingManager& StreamingManager =
		IStreamingManager::Get().GetRenderAssetStreamingManager();

	// Recalcula texturas e meshes necessárias para a view atual.
	StreamingManager.UpdateResourceStreaming(0.0f, true);

	LastStreamingUpdateId = StreamingManager.GetNumWantingResourcesID();
	StableEvaluationCount = 0;
	StreamingCheckStartTime = World->GetRealTimeSeconds();

	// Rate = 0.05f (nunca 0.0f, senão o timer é limpo em vez de rodar)
	World->GetTimerManager().SetTimer(
		StreamingCheckTimer,
		this,
		&AMyPlayerController::CheckMapStreaming,
		StreamingCheckInterval,
		true,
		StreamingCheckInterval
	);
}

void AMyPlayerController::CheckMapStreaming()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	IRenderAssetStreamingManager& StreamingManager =
		IStreamingManager::Get().GetRenderAssetStreamingManager();

	StreamingManager.UpdateResourceStreaming(StreamingCheckInterval, true);

	const int32 CurrentUpdateId = StreamingManager.GetNumWantingResourcesID();

	// Só conta quando o sistema realmente reavaliou o streaming.
	if (CurrentUpdateId != LastStreamingUpdateId)
	{
		LastStreamingUpdateId = CurrentUpdateId;
	}

	StableEvaluationCount = (StreamingManager.GetNumWantingResources() == 0) ? StableEvaluationCount + 1 : 0;

	const double ElapsedTime = World->GetRealTimeSeconds() - StreamingCheckStartTime;
	const bool bStreamingReady = StableEvaluationCount >= RequiredStableEvaluations;
	const bool bTimedOut = ElapsedTime >= MaxWaitTime;

	if (bStreamingReady || bTimedOut)
	{
		World->GetTimerManager().ClearTimer(StreamingCheckTimer);
		OnMapFullyLoaded(); // dispara o evento do Blueprint
	}
}