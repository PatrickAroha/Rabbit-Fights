// Fill out your copyright notice in the Description page of Project Settings.


#include "RabbitGameplayAbility.h"

void URabbitGameplayAbility::Tick(float DeltaTime)
{
	TickGA(DeltaTime);
}


ETickableTickType URabbitGameplayAbility::GetTickableTickType() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? ETickableTickType::Never : ETickableTickType::Always;
}

bool URabbitGameplayAbility::IsTickable() const
{
	return IsTickEnabled;
}

TStatId URabbitGameplayAbility::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URabbitGameplayAbility, STATGROUP_Tickables);
}

void URabbitGameplayAbility::SetEnableTick(bool Enable)
{
	IsTickEnabled = Enable;
}
