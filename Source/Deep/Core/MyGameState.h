// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "E_MatchPhase.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMatchPhaseChanged,
	EMatchPhase, OldPhase,
	EMatchPhase, NewPhase
);

/**
 * 
 */
UCLASS() 
class DEEP_API AMyGameState : public AGameState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MatchPhase)
	EMatchPhase MatchPhase = EMatchPhase::WaitingPlayers;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 CountdownEndTime = 0;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 TotalMatchPoints = 5;
	
	UPROPERTY(BlueprintAssignable, Category="Match|Events")
	FOnMatchPhaseChanged OnMatchPhaseChanged;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Match|Events")
	void SetMatchPhase(EMatchPhase NewPhase);

	UFUNCTION(BlueprintCallable, Category="Match|Events")
	void SetMatchPointsToEndGame(int32 MatchPoints);

	UFUNCTION(BlueprintCallable)
	void Countdown(float DurationSeconds = 3.f);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPhaseChanged(EMatchPhase NewMatchPhase);
	
protected:
	
	UFUNCTION()
	void OnRep_MatchPhase(EMatchPhase OldPhase);
};
