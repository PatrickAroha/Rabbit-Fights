// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerControllerLobby.h"
#include "MyHUDLobby.h"

void AMyPlayerControllerLobby::InitPlayerState()
{
	Super::InitPlayerState();
}

void AMyPlayerControllerLobby::SpawnDefaultHUD()
{
	Super::SpawnDefaultHUD();
}

void AMyPlayerControllerLobby::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (IsLocalController())
	{
		AMyHUDLobby* MyHud = GetHUD<AMyHUDLobby>();
		if (MyHud != nullptr)
		{
		
			MyHud->CreateHudLobby(PlayerState);
		}
	}
}

void AMyPlayerControllerLobby::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && IsLocalController())
	{
		AMyHUDLobby* MyHud = GetHUD<AMyHUDLobby>();
		if (MyHud != nullptr)
		{
			MyHud->CreateHudLobby(PlayerState);
		}
	}

}


