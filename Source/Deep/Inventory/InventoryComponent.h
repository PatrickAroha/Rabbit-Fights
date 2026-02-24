// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemDefinition;
class UActorChannel;
class UItemInstance;
class APickUp;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEEP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventoryComponent();

	//debug
	UPROPERTY(Replicated)
	int32 EquippedSlot = 0;

	
	//Array de item
	UPROPERTY(ReplicatedUsing=OnRep_Slots, BlueprintReadOnly)
	TArray<TObjectPtr<UItemInstance>> Slots;

	// internos
	bool AddItem(UItemDefinition* Def, int32 Qty, int32& QtyRemaining);
	int32 TryAddStack(UItemDefinition* Def, int32& InOutQty);
	int32 NewStack(UItemDefinition* Def, int32 Qty);

	//Funções replicadas
	UFUNCTION(Server, Reliable) void Server_DropItem(int32 SlotIndex);
	UFUNCTION(Server, Reliable) void Server_SwapItem(int32 A, int32 B);
	UFUNCTION(Server, Reliable, BlueprintCallable) void Server_TryPickup(APickUp* Pickup);
	UFUNCTION(Server, Reliable) void Server_PickupReplace(APickUp* Pickup, int32 SlotIndex);


	//Replicate
	UFUNCTION()
	void OnRep_Slots();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
};
