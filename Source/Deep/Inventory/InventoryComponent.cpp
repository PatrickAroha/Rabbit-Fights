// InventoryComponent.cpp
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "ItemInstance.h"
#include "PickUp.h"
#include "ItemDefinition.h" // se existir

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	SetIsReplicatedByDefault(true);
	Slots.SetNum(3);
}

void UInventoryComponent::OnRep_Slots()
{
	// atualizar HUD / equip aqui
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Slots);
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

int32 UInventoryComponent::TryAddStack(UItemDefinition* Def, int32& Qty)
{
	if (!Def || Qty <= 0) return Qty;
	if (!Def->bStackable) return Qty;

	for (UItemInstance* Item : Slots)
	{
		if (Qty <= 0) break;
		if (!Item || Item->Def != Def) continue;

		const int32 Space = FMath::Max(0, Def->MaxStack - Item->Quantity);
		const int32 AddNow = FMath::Min(Space, Qty);
		if (AddNow <= 0) continue;
		
		Item->Quantity += AddNow;
		Qty -= AddNow;
	}

	return Qty;
}

int32 UInventoryComponent::NewStack(UItemDefinition* Def, int32 Qty)
{
	if (!Def || Qty <= 0) return Qty;
	const int32 MaxPerStack = Def->bStackable ? Def->MaxStack : 1;
	
	for (TObjectPtr<UItemInstance>& Slot : Slots)
	{
		if (Qty <= 0) break;
		if (Slot) continue;

		UItemInstance* NewItem = NewObject<UItemInstance>(this);
		const int32 Put = FMath::Min(Qty, MaxPerStack);

		NewItem->Init(Def, Put);
		Slot = NewItem;
		Qty -= Put;
	}
	
	return Qty;
}

bool UInventoryComponent::AddItem(UItemDefinition* Def, int32 Qty, int32& QtyRemaining)
{
	QtyRemaining = Qty;
	if (!GetOwner() || !GetOwner()->HasAuthority()) return false;
	if (!Def || Qty <= 0) return false;

	const int32 Start = Qty;
	
	QtyRemaining = TryAddStack(Def, QtyRemaining);
	if (QtyRemaining > 0)
		QtyRemaining = NewStack(Def, QtyRemaining);
	
	GetOwner()->ForceNetUpdate();
	return QtyRemaining != Start;
}

void UInventoryComponent::Server_TryPickup_Implementation(APickUp* Pickup)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (!Pickup || !Pickup->Def || Pickup->Quantity <= 0) return;
	
	int32 PickUpQty = 0;
	const bool bAddedItem = AddItem(Pickup->Def, Pickup->Quantity, PickUpQty);

	if (!bAddedItem)
	{
		// não entrou nada -> troca direto no slot equipado
		Server_PickupReplace_Implementation(Pickup, EquippedSlot); //trocar depois handItem "EquippedSlot"
		return;
	}
	
	if (PickUpQty <= 0) Pickup->Destroy();
	else Pickup->SetQuantity(PickUpQty);
	
	GetOwner()->ForceNetUpdate();
}

void UInventoryComponent::Server_PickupReplace_Implementation(APickUp* Pickup, int32 SlotIndex)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (!Pickup || SlotIndex < 0 || SlotIndex > 2) return;
	if (!Pickup->Def || Pickup->Quantity <= 0) return;

	UItemInstance* OldHandItem = Slots[SlotIndex];
	
	UItemInstance* NewItem = NewObject<UItemInstance>(this);
	NewItem->Init(Pickup->Def, Pickup->Quantity);
	Slots[SlotIndex] = NewItem;
	
	if (OldHandItem && OldHandItem->Def)
	{
		Pickup->SetDef(OldHandItem->Def);
		Pickup->SetQuantity(OldHandItem->Quantity);
	}
	else
	{
		Pickup->Destroy();
	}

	GetOwner()->ForceNetUpdate();
}

void UInventoryComponent::Server_SwapItem_Implementation(int32 A, int32 B)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (A < 0 || A > 2 || B < 0 || B > 2) return;

	Swap(Slots[A], Slots[B]);
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

	TSubclassOf<APickUp> ClassToSpawn = Item->Def->PickupClass
	? Item->Def->PickupClass
	: TSubclassOf<APickUp>(APickUp::StaticClass());
	APickUp* Pickup = World->SpawnActor<APickUp>(ClassToSpawn, SpawnLoc, SpawnRot, Params);

	if (Pickup)
	{
		Pickup->SetDef(Item->Def);
		Pickup->SetQuantity(Item->Quantity);
	}

	Slots[SlotIndex] = nullptr;
	OwnerActor->ForceNetUpdate();
}