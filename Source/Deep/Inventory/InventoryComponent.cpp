// InventoryComponent.cpp
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "ItemInstance.h"
#include "PickUp.h"
#include "ItemDefinition.h"

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	Slots.SetNum(3);
}

void UInventoryComponent::NotifyInvChanged()
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

	GetOwner()->ForceNetUpdate();
	return QtyRemaining != Start;
}

void UInventoryComponent::Server_TryPickup_Implementation(AActor* RecivedActor)
{
	APickUp* Pickup = Cast<APickUp>(RecivedActor);
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (!Pickup || !Pickup->Item || !Pickup->Item->Def || Pickup->Item->Quantity <= 0) return;

	int32 QtyRemaining = Pickup->Item->Quantity;
	const bool bAddedItem = AddItem(Pickup->Item, QtyRemaining);

	if (!bAddedItem)
	{
		Server_PickupReplace_Implementation(Pickup, EquippedSlot);
		return;
	}

	Pickup->Item->SetQuantity(QtyRemaining);

	if (Pickup->Item->Quantity <= 0) Pickup->Destroy();
	else Pickup->ForceNetUpdate();

	//NotifyInvChanged(); 
	GetOwner()->ForceNetUpdate();
}

void UInventoryComponent::Server_PickupReplace_Implementation(APickUp* Pickup, int32 SlotIndex)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (!Pickup || !Pickup->Item || !Pickup->Item->Def || Pickup->Item->Quantity <= 0) return;
	if (SlotIndex < 0 || SlotIndex >= Slots.Num()) return;
	if (!Slots[SlotIndex]) return;

	Server_DropItem_Implementation(SlotIndex);

	int32 Remaining = Pickup->Item->Quantity;
	Remaining = NewStack(Pickup->Item, Remaining);

	Pickup->Item->SetQuantity(Remaining);

	if (Remaining <= 0) Pickup->Destroy();
	else Pickup->ForceNetUpdate();

	GetOwner()->ForceNetUpdate();
}

void UInventoryComponent::Server_SwapItem_Implementation(int32 A, int32 B)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (A < 0 || A > 2 || B < 0 || B > 2) return;

	Swap(Slots[A], Slots[B]);

	NotifyInvChanged();     
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

	NotifyInvChanged();     
	OwnerActor->ForceNetUpdate();
}

void UInventoryComponent::OnRep_Slots()
{
	OnInventoryChanged.Broadcast(); 
}