// Fill out your copyright notice in the Description page of Project Settings.
#include "ItemInstance.h"
#include "ItemInstance.h"
#include "ItemDefinition.h"
#include "Net/UnrealNetwork.h"

void UItemInstance::Init(UItemDefinition* InDef, int32 InQty)
{
	Def = InDef;
	Quantity = InQty;
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemInstance, Def);
	DOREPLIFETIME(UItemInstance, Quantity);
	DOREPLIFETIME(UItemInstance, Durability);
}
