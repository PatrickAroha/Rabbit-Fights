// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "MyPlayerStateLobby.generated.h"

//Dispatcher
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPSUpdateReady, bool, Ready);

UCLASS()
class DEEP_API AMyPlayerStateLobby : public APlayerState
{
	GENERATED_BODY()
	
//Funções

public:
	
	UFUNCTION(BlueprintCallable, Category = "Events")
	void PressedReady();
	
	UFUNCTION(BlueprintCallable, Category = "Events")
	void SetReady(bool Ready);
	
	UFUNCTION()
	void OnRep_bReady();
	
private:
	
	UFUNCTION(Server, Reliable)
	void S_Ready();
	
	UFUNCTION(Server, Reliable)
	void S_SetReady(bool Ready);
	
//Variaveis

public:
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_bReady)
	bool bReady = false;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPSUpdateReady OnPSUpdateReady;

};
