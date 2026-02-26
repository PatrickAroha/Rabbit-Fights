// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "RabbitGameplayAbility.generated.h"

UCLASS()
class DEEP_API URabbitGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool ShouldShowInAbilitiesBar = false;
};
