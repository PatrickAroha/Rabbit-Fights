#include "ItemInstance.h"
#include "InventoryComponent.h"
#include "PickUp.h"
#include "Deep/Inventory/Fragment.h"
#include "Net/UnrealNetwork.h"

void UItemInstance::SetQuantity(int32 NewQty)
{
	if (Quantity == NewQty) return;
	Quantity = NewQty;
	NotifyChanged();
}

void UItemInstance::ActivateFragments(APawn* OwnerPawn)
{
	if (!OwnerPawn || !OwnerPawn->HasAuthority()) return;
	
	for (UFragment* Fragment : Fragments)
	{
		if (Fragment)
		{
			Fragment->Activate(OwnerPawn, this);
		}
	}
}

void UItemInstance::DeactivateFragments(APawn* OwnerPawn)
{
	if (!OwnerPawn || !OwnerPawn->HasAuthority()) return;

	for (UFragment* Fragment : Fragments)
	{
		if (Fragment)
		{
			Fragment->Deactivate(OwnerPawn, this);
		}
	}
}


//Helper And replicate ----------------------------------------------------------------------------------------------------

void UItemInstance::CopyFrom(const UItemInstance* Item)
{
	if (!Item) return;
	Def = Item->Def;
	Quantity = Item->Quantity;
	Durability = Item->Durability;
}
void UItemInstance::NotifyChanged()
{
	if (UInventoryComponent* Inv = GetTypedOuter<UInventoryComponent>())
	{
		Inv->NotifyInvChanged();
	}
	
	if (APickUp* PU = Cast<APickUp>(GetOuter()))
	{
		PU->OnRep_Item();
	}
}
void UItemInstance::OnRep_Def()
{
	NotifyChanged();
}
void UItemInstance::OnRep_Quantity()
{
	NotifyChanged();
}
void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemInstance, Def);
	DOREPLIFETIME(UItemInstance, Quantity);
	DOREPLIFETIME(UItemInstance, Durability);
}
