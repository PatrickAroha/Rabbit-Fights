// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

#include "Net/UnrealNetwork.h"

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, MatchPhase);
}

void AMyGameState::SetMatchPhase(EMatchPhase NewPhase)
{
	if (!HasAuthority()) return;
	if (MatchPhase == NewPhase) return;

	const EMatchPhase Old = MatchPhase;
	MatchPhase = NewPhase;
	
	OnMatchPhaseChanged.Broadcast(Old, NewPhase);
	
}

void AMyGameState::Countdown(float DurationSeconds)
{
	if (!HasAuthority())
	{
		return;
	}

	const float ServerNow = GetServerWorldTimeSeconds();
	
	CountdownEndTime = FMath::CeilToInt32(ServerNow + DurationSeconds);
}

void AMyGameState::OnRep_MatchPhase(EMatchPhase OldPhase)
{
	OnMatchPhaseChanged.Broadcast(OldPhase, MatchPhase);
}
