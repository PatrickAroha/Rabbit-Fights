// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeLobby.h"
#include "MyPlayerStateLobby.h"
#include "MyHUDLobby.h"
#include "Net/UnrealNetwork.h"



void AMyPlayerStateLobby::PressedReady()
{

	S_Ready();

}

void AMyPlayerStateLobby::S_Ready_Implementation()
{
	
	if (bReady)
	{
		bReady = false;
	}
	else
	{
		bReady = true;
	}
	
	OnRep_bReady();
	
	if (AMyGameModeLobby* GM = GetWorld()->GetAuthGameMode<AMyGameModeLobby>())
	{
		GM->PlayerReady();
	}
	
}

void AMyPlayerStateLobby::SetReady(bool Ready)
{
	S_SetReady(Ready);
}

void AMyPlayerStateLobby::S_SetReady_Implementation(bool Ready)
{
	bReady = Ready;
	
	OnRep_bReady();
}

void AMyPlayerStateLobby::OnRep_bReady()
{
		OnPSUpdateReady.Broadcast(bReady);
}


void AMyPlayerStateLobby::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AMyPlayerStateLobby, bReady, COND_None, REPNOTIFY_Always);
}
