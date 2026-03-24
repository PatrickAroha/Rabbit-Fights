// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GMItem.h"
#include "Components/ActorComponent.h"
#include "GMItemManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DEEP_API UGMItemManager : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UGMItemManager();

	//Vars
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Game Item")
	AActor* CarriedItem = nullptr;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Game Item")
	TObjectPtr<UGMItem> Item = nullptr;

	//Functions
	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void EquipItem(UGMItem* NewItem, AActor* NewCarriedItem);

	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void UnequipItem(AActor* Killer);

	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void CarrysDie(AActor* Killer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
	void RoundPoint();

	//Server
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RSEquipItem(UGMItem* NewItem, AActor* NewCarriedItem);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RSUnequipItem(AActor* Killer);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RSCarrysDie(AActor* Killer);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RSRoundPoint();


	//Replicate
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags);
};
