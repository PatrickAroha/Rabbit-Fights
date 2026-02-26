// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RabbitAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEEP_API URabbitAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	TArray<FGameplayAbilitySpec> LastActivatableAbilities;
	
public:
	// Sets default values for this component's properties
	URabbitAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OnRep_ActivateAbilities() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
