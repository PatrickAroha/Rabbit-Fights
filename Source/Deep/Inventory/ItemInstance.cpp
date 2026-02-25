// Fill out your copyright notice in the Description page of Project Settings.
#include "ItemInstance.h"

#include "InventoryComponent.h"
#include "PickUp.h"
#include "Net/UnrealNetwork.h"

void UItemInstance::OnRep_Def()
{
	if (APickUp* PU = Cast<APickUp>(GetOuter()))
	{
		PU->OnRep_Item();
	}
}

void UItemInstance::OnRep_Quantity()
{
	if (UInventoryComponent* Inv = Cast<UInventoryComponent>(GetOuter()))
	{
		Inv->OnInventoryChanged.Broadcast();
	}
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemInstance, Def);
	DOREPLIFETIME(UItemInstance, Quantity);
	DOREPLIFETIME(UItemInstance, Durability);
}

//Helper para atualizar informações
void UItemInstance::CopyFrom(const UItemInstance* Item)
{
	if (!Item) return;
	Def = Item->Def;
	Quantity = Item->Quantity;
	Durability = Item->Durability;
}
