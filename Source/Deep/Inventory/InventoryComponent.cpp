#include "InventoryComponent.h"

#include "Fragment.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "ItemInstance.h"
#include "PickUp.h"
#include "ItemDefinition.h"
#include "Deep/MagicManager/MagicManager.h"

class UEquipItem_Fragment;

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	Slots.SetNum(3);
}


// ======= Inventory Logic =======

int32 UInventoryComponent::TryAddStack(const UItemInstance* Item, int32& Qty)
{
	if (!Item || !Item->Def || Qty <= 0) return Qty;
	if (!Item->Def->bStackable) return Qty;

	for (UItemInstance* Slot : Slots)
	{
		if (Qty <= 0) break;
		if (!Slot || Slot->Def != Item->Def) continue;

		const int32 Space  = FMath::Max(0, Item->Def->MaxStack - Slot->Quantity);
		const int32 AddNow = FMath::Min(Space, Qty);
		if (AddNow <= 0) continue;

		Slot->SetQuantity(Slot->Quantity + AddNow);
		Qty -= AddNow;
	}

	return Qty;
}

int32 UInventoryComponent::NewStack(const UItemInstance* Item, int32 Qty)
{
	if (!Item || !Item->Def || Qty <= 0) return Qty;

	const int32 MaxPerStack = Item->Def->bStackable ? Item->Def->MaxStack : 1;

	for (TObjectPtr<UItemInstance>& Slot : Slots)
	{
		if (Qty <= 0) break;
		if (Slot) continue;

		UItemInstance* NewItem = NewObject<UItemInstance>(this, Item->GetClass());
		const int32 Put = FMath::Min(Qty, MaxPerStack);

		NewItem->Def = Item->Def;
		NewItem->Durability = Item->Durability;
		Slot = NewItem;       
		NewItem->SetQuantity(Put); 


		Qty -= Put;
	}

	return Qty;
}

bool UInventoryComponent::AddItem(const UItemInstance* Item, int32& QtyRemaining)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return false;
	if (!Item || !Item->Def || QtyRemaining <= 0) return false;

	const int32 Start = QtyRemaining;

	QtyRemaining = TryAddStack(Item, QtyRemaining);
	if (QtyRemaining > 0)
	{
		QtyRemaining = NewStack(Item, QtyRemaining);
	}
	
	return QtyRemaining != Start;
}

UItemInstance* UInventoryComponent::TryPickUpItem(UItemInstance* RecivedItem)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return RecivedItem;

	if (!RecivedItem || !RecivedItem->Def || RecivedItem->Quantity <= 0) return RecivedItem;
	
	// Se Magia Manda item Para MagicManager
	if (RecivedItem->Def->ItemCategory == EItemCategory::Magic)
	{
		if (UMagicManager* Magic = GetOwner()->FindComponentByClass<UMagicManager>())
		{
			UItemInstance* Result = Magic->TryCollectMagic(RecivedItem);
			
			if (!Result)
			{
				return nullptr;
			}
			
			return RecivedItem;
		}
	}
	
	int32 QtyRemaining = RecivedItem->Quantity;
	
	const bool bAddedItem = AddItem(RecivedItem, QtyRemaining);

	if (!bAddedItem)
	{
		RecivedItem = PickupReplace(RecivedItem, EquippedSlot);
		return RecivedItem;
	}

	RecivedItem->SetQuantity(QtyRemaining);
	
	Server_ChangeSlot(EquippedSlot); 
	GetOwner()->ForceNetUpdate();
	
	return RecivedItem;
	
}

UItemInstance* UInventoryComponent::PickupReplace(UItemInstance* RecivedItem, int32 SlotIndex)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return RecivedItem;
	if (!RecivedItem || !RecivedItem->Def || RecivedItem->Quantity <= 0) return RecivedItem;
	if (SlotIndex < 0 || SlotIndex >= Slots.Num()) return RecivedItem;
	if (!Slots[SlotIndex]) return RecivedItem;

	Server_DropItem_Implementation(SlotIndex);

	int32 Remaining = RecivedItem->Quantity;
	Remaining = NewStack(RecivedItem, Remaining);

	RecivedItem->SetQuantity(Remaining);

	Server_ChangeSlot(EquippedSlot); 
	GetOwner()->ForceNetUpdate();
	
	return RecivedItem;
}

void UInventoryComponent::Server_SwapItem_Implementation(int32 A, int32 B)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (A < 0 || A > 2 || B < 0 || B > 2) return;

	Swap(Slots[A], Slots[B]);

	Server_ChangeSlot(EquippedSlot);   
	GetOwner()->ForceNetUpdate();
}

void UInventoryComponent::Server_DropItem_Implementation(int32 SlotIndex)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (SlotIndex < 0 || SlotIndex > 2) return;

	UItemInstance* Item = Slots[SlotIndex];
	if (!Item || !Item->Def || Item->Quantity <= 0) return;

	AActor* OwnerActor = GetOwner();
	UWorld* World = OwnerActor->GetWorld();
	if (!World) return;

	const FVector SpawnLoc = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * 100.f;
	const FRotator SpawnRot = OwnerActor->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = OwnerActor;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TSubclassOf<APickUp> ClassToSpawn =
		Item->Def->PickupClass ? Item->Def->PickupClass : TSubclassOf<APickUp>(APickUp::StaticClass());

	APickUp* Pickup = World->SpawnActor<APickUp>(ClassToSpawn, SpawnLoc, SpawnRot, Params);

	if (Pickup)
	{
		Pickup->Item = NewObject<UItemInstance>(Pickup, Item->GetClass());
		Pickup->Item->CopyFrom(Item);
		Pickup->RefreshMesh();
		Pickup->ForceNetUpdate();
	}

	Slots[SlotIndex] = nullptr;

	Server_ChangeSlot(EquippedSlot);    
	OwnerActor->ForceNetUpdate();
}

// ============ Equip Logic ===============

void UInventoryComponent::Server_ChangeSlot_Implementation(int32 NewSlot)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	
	if (!Slots.IsValidIndex(NewSlot) || !Slots.IsValidIndex(EquippedSlot)) return;
	
	UItemInstance* NewItem = Slots[NewSlot];
	
	if (!EquippedItem && !NewItem)
	{
		EquippedSlot = NewSlot;
		OnRep_Slots();
		return;
	}
	
	if (EquippedItem != NewItem)
	{
		if (EquippedItem && EquippedItem->SpawnedActor)
			UnequipItem(EquippedItem);
		if (NewItem)
			EquipItem(NewItem);

		EquippedSlot = NewSlot;
		OnRep_Slots();
		return;
	}
	
	if (EquippedItem == NewItem || (!EquippedItem && NewItem)) 
	{
		if (EquippedItem && EquippedItem->SpawnedActor)
			UnequipItem(EquippedItem);
		
		if (NewItem)
			EquipItem(NewItem);
	}
	
	EquippedSlot = NewSlot;
	OnRep_Slots();
}

void UInventoryComponent::EquipItem(UItemInstance* NewItemSlot)
{
	if (!NewItemSlot) return;
	
	NewItemSlot->SpawnItem(Cast<APawn>(GetOwner()));

	EquippedItem = NewItemSlot;
}

void UInventoryComponent::UnequipItem(UItemInstance* OldItemSlot)
{
	if (!OldItemSlot) return;

	OldItemSlot->DestroyItem(Cast<APawn>(GetOwner()));
}


//============= Replicate ===========

void UInventoryComponent::OnRep_Slots()
{
	OnInventoryChanged.Broadcast(); 
}


void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Slots);
	DOREPLIFETIME(UInventoryComponent, EquippedSlot);
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
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
