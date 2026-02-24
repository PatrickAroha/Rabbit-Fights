// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.h"
#include "Components/ActorComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "InventoryComponent.generated.h"


class UItemDefinition;
class UItemInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEEP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventoryComponent()
	{
		SetIsReplicatedByDefault(true);
	}

	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TObjectPtr<UItemInstance> Slots[3];

	UFUNCTION()
	void OnRep_Slots();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const override
	{
		Super::GetLifetimeReplicatedProps(Out);
		DOREPLIFETIME(UInventoryComponent, Slots);
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool bWrote = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		for (UItemInstance* Item : Slots)
		{
			if (IsValid(Item))
			{
				bWrote |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
			}
		}
		return bWrote;
	}

	// Server API
	UFUNCTION(Server, Reliable) void Server_SetSlot(int32 SlotIndex, UItemDefinition* Def, int32 Qty);
	UFUNCTION(Server, Reliable) void Server_ClearSlot(int32 SlotIndex);
	UFUNCTION(Server, Reliable) void Server_SwapSlots(int32 A, int32 B);
};
