// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

EHUDState AMyHUD::GetCurrentState() const
{
	return CurrentState;
}

void AMyHUD::OnStateChanged_Implementation(EHUDState NewState)
{
	CurrentState = NewState;
}