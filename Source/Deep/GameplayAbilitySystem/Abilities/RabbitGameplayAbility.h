// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "RabbitGameplayAbility.generated.h"

UCLASS()
class DEEP_API URabbitGameplayAbility : public UGameplayAbility, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable)
	void SetEnableTick(bool Enable);

	UFUNCTION(BlueprintImplementableEvent)
	void TickGA(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool ShouldShowInAbilitiesBar = false;

	UPROPERTY(EditAnywhere, Category = "Tick")
	bool IsTickEnabled = false;
};
