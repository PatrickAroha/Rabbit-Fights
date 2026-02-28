#include "ItemInstance.h"

#include "BaseItem.h"
#include "InventoryComponent.h"
#include "ItemDefinition.h"
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
			Fragment->InitActivate(OwnerPawn, this);
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
			Fragment->InitDeactivate(OwnerPawn, this);
		}
	}
}

// ================= Spawn Item On PickUp In Hand ======================

void UItemInstance::SpawnItem(APawn* OwnerPawn)
{
	if (!OwnerPawn || !OwnerPawn->HasAuthority())
		return;

	if (!Def || !Def->ActorItemClass)
		return;

	if (IsValid(SpawnedActor))
		return;

	UWorld* World = OwnerPawn->GetWorld();
	if (!World)
		return;

	FActorSpawnParameters Params;
	Params.Owner = OwnerPawn;
	Params.Instigator = OwnerPawn;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnedActor = World->SpawnActor<ABaseItem>(
		Def->ActorItemClass,
		FTransform::Identity,
		Params
	);

	if (!SpawnedActor)
		return;

	if (USkeletalMeshComponent* Mesh = OwnerPawn->FindComponentByClass<USkeletalMeshComponent>())
	{
		SpawnedActor->AttachToComponent(
			Mesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("hand_rSocket")
		);
	}

	SpawnedActor->InitFromInstance(this);
}

void UItemInstance::DestroyItem(APawn* OwnerPawn)
{
	if (!OwnerPawn || !OwnerPawn->HasAuthority()) return;

	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Destroy();
		SpawnedActor = nullptr;
	}
}

// ==================== Helper And replicate ======================

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
		Inv->OnRep_Slots();
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
