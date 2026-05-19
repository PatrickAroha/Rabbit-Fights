// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "InterfacePlayerInfo.h"
#include "RabbitCharacterBase.generated.h"

class UBasicAttributeSet;

UCLASS()
class DEEP_API ARabbitCharacterBase : public ACharacter, public IAbilitySystemInterface, public IInterfacePlayerInfo
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ARabbitCharacterBase();
	
	virtual void CheckJumpInput(float DeltaTime) override;
	
	// Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UBasicAttributeSet* BasicAttributeSet;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bMagicMode = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	EGameplayEffectReplicationMode ASCReplicationMode = EGameplayEffectReplicationMode::Mixed;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CanDoAction_Implementation() const override { return bMagicMode; }
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GenerateWidgetOnSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void SendAbilitiesChangedEvent();

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent) void Die();
};