#include "Net/UnrealNetwork.h"
#include "GMItemManager.h"

#include "Engine/ActorChannel.h"


UGMItemManager::UGMItemManager()
{
	SetIsReplicatedByDefault(true);
}

void UGMItemManager::CarrysDie_Implementation(AActor* Killer)
{
	if (Item)
	{
		Item->OnCarrierDied(Killer);
	}
}

void UGMItemManager::RoundPoint_Implementation()
{
	
}

void UGMItemManager::UnequipItem_Implementation(AActor* Killer)
{
	if (Item)
	{
		Item->OnRoundPoint.RemoveDynamic(this, &UGMItemManager::RoundPoint);
		Item->OnUnequipped(Killer);
	}
}

void UGMItemManager::EquipItem_Implementation(UGMItem* NewItem, AActor* NewCarriedItem)
{
	if (!NewItem) return;

	Item = NewItem;
	Item->OnRoundPoint.AddDynamic(this, &UGMItemManager::RoundPoint);
	CarriedItem = NewCarriedItem;
	Item->OnEquipped();
	
}

// -------------- SERVER ----------------------//
void UGMItemManager::RSCarrysDie_Implementation(AActor* Killer)
{
	CarrysDie(Killer);
}

void UGMItemManager::RSEquipItem_Implementation(UGMItem* NewItem, AActor* NewCarriedItem)
{
	EquipItem(NewItem, NewCarriedItem);
}

void UGMItemManager::RSUnequipItem_Implementation(AActor* Killer)
{
	UnequipItem(Killer);
}

void UGMItemManager::RSRoundPoint_Implementation()
{
	RoundPoint();
}

// -------------- Replicate ----------------------//

void UGMItemManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGMItemManager, CarriedItem);
	DOREPLIFETIME(UGMItemManager, Item);
}

bool UGMItemManager::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWrote = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	if (IsValid(Item))
	{
		bWrote |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
	}
	return bWrote;
}
