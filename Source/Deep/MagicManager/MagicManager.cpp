#include "MagicManager.h"

#include "Deep/GameplayAbilitySystem/Characters/InterfacePlayerInfo.h"
#include "Deep/Inventory/PickUp.h"
#include "Deep/Inventory/ItemDefinition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UMagicManager::UMagicManager()
{
	SetIsReplicatedByDefault(true);
	
	Magics.SetNum(3);

}

UItemInstance* UMagicManager::TryCollectMagic(UItemInstance* MagicItem)
{
	if (!GetOwner() || !MagicItem) return MagicItem;

	int32 Slot = GetFreeSlot();

	// slot livre
	if (Slot != INDEX_NONE)
	{
		UpdateSlot(Slot, MagicItem);
		ChangedSlot = Slot;
		OnMagicAdded.Broadcast(Slot);
		if (Slot == int32(SelectedSlot))
			if (IInterfacePlayerInfo::Execute_CanDoAction(GetOwner()))
				Server_ChangeSlot(SelectedSlot);

		return nullptr;
	}

	// troca
	int32 ReplaceSlot = (int32)SelectedSlot;

	if (ReplaceSlot < 0 || ReplaceSlot > 2) return MagicItem;
	
	UItemInstance* Temp = Magics[ReplaceSlot];

	UpdateSlot(ReplaceSlot, MagicItem);

	ChangedSlot = ReplaceSlot;
	OnMagicAdded.Broadcast(Slot);

	Server_ChangeSlot(SelectedSlot);

	return Temp;
}

void UMagicManager::UpdateSlot(int32 Slot, UItemInstance* NewMagic)
{
	if (Slot < 0 || Slot > 2 || !NewMagic) return;

	// remove se já tiver
	if (MagicHandles[Slot].IsValid())
	{
		RemoveMagicAbility(MagicHandles[Slot], Slot);
	}

	// adiciona nova
	FGameplayAbilitySpecHandle Handle = GiveMagicAbility(NewMagic);
	if (!Handle.IsValid()) return;

	Magics[Slot] = NewMagic;
	MagicHandles[Slot] = Handle;
	
	OnMagicAdded.Broadcast(Slot);
	
}

FGameplayAbilitySpecHandle UMagicManager::GiveMagicAbility(UItemInstance* MagicItem)
{
	if (!GetOwner() || !MagicItem) return FGameplayAbilitySpecHandle();

	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return FGameplayAbilitySpecHandle();

	if (!MagicItem->Def->AbilityClass) return FGameplayAbilitySpecHandle();

	FGameplayAbilitySpec Spec(MagicItem->Def->AbilityClass, 1);

	return ASC->GiveAbility(Spec);
}

void UMagicManager::RemoveMagicAbility(FGameplayAbilitySpecHandle Handle, int32 Slot)
{
	if (!Handle.IsValid()) return;

	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		ASC->ClearAbility(Handle);
	}
	
	Magics[Slot] = nullptr;
	OnMagicAdded.Broadcast(Slot);
	
}

void UMagicManager::Server_RemoveMagicAbility_Implementation(EMagicSlot Slot)
{
	int32 Index = int32(Slot);
	FGameplayAbilitySpecHandle Handle = MagicHandles[Index];
	RemoveMagicAbility(Handle, Index);
}

void UMagicManager::UnequipMagic(EMagicSlot LastSlot)
{
	int32 OldIndex = int32(LastSlot);
	
	if (OldIndex < 0 || OldIndex > 2 || !Magics.IsValidIndex(OldIndex)) return;
	if (!Magics[OldIndex] || !Magics[OldIndex]->Def) return;
	
	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		ASC->RemoveGameplayCue(Magics[OldIndex]->Def->CueMagicHand);
	}
}

void UMagicManager::Server_RSUnequipMagic_Implementation(EMagicSlot Slot)
{
	UnequipMagic(Slot);
}

void UMagicManager::Server_ChangeSlot_Implementation(EMagicSlot Slot)
{
	int32 Index = (int32)Slot;
	if (Index < 0 || Index > 2) return;
	
	EMagicSlot OldSlot = SelectedSlot;
	SelectedSlot = Slot;
	OnSlotSelected.Broadcast(SelectedSlot);

	UnequipMagic(OldSlot);

	if (!Magics.IsValidIndex(Index) || !Magics[Index] || !Magics[Index]->Def) return;
	
	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		FGameplayCueParameters CueParams;
		ASC->AddGameplayCue(Magics[Index]->Def->CueMagicHand, CueParams);
	}
}

void UMagicManager::OnRep_Magics()
{
	OnMagicAdded.Broadcast(ChangedSlot);
}

void UMagicManager::Server_DropMagic_Implementation(uint8 Slot)
{
	if (!GetOwner()) return;
	if (Slot >= 3) return;
	if (!Magics[Slot]) return;

	AActor* OwnerActor = GetOwner();
	UWorld* World = OwnerActor->GetWorld();
	if (!World) return;

	const FVector SpawnLoc = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * 100.f;
	const FRotator SpawnRot = OwnerActor->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = OwnerActor;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TSubclassOf<APickUp> ClassToSpawn = Magics[Slot]->Def->PickupClass;

	if (!ClassToSpawn)
	{
		ClassToSpawn = APickUp::StaticClass();
	}

	if (APickUp* Pickup = World->SpawnActor<APickUp>(ClassToSpawn, SpawnLoc, SpawnRot, Params))
	{
		Pickup->Item = NewObject<UItemInstance>(Pickup);
		Pickup->Item->Def = Magics[Slot]->Def;
		Pickup->Item->SetQuantity(1);
		Pickup->RefreshMesh();

		UnequipMagic(SelectedSlot);
		RemoveMagicAbility(MagicHandles[Slot], Slot);

		Magics[Slot] = nullptr;
		MagicHandles[Slot] = FGameplayAbilitySpecHandle();

		ChangedSlot = Slot;
		OnMagicAdded.Broadcast(Slot);
	}

	OwnerActor->ForceNetUpdate();
}

void UMagicManager::Server_SwapMagics_Implementation()
{
	int32 Index = (int32)SelectedSlot;
	int32 Next = (Index + 1) % 3;

	Swap(Magics[Index], Magics[Next]);
	Swap(MagicHandles[Index], MagicHandles[Next]);
}

void UMagicManager::Server_ActiveMagic_Implementation(EMagicSlot Slot)
{
	int32 Index = (int32)Slot;

	if (!Magics[Index]) return;
	
	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		FGameplayEventData Payload;
		ASC->TryActivateAbility(MagicHandles[Index]);
	}
}

bool UMagicManager::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWrote = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UItemInstance* Item : Magics)
	{
		if (IsValid(Item))
		{
			bWrote |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
		}
	}

	return bWrote;
}

void UMagicManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMagicManager, Magics);
	DOREPLIFETIME(UMagicManager, MagicHandles);
	DOREPLIFETIME(UMagicManager, SelectedSlot);
	DOREPLIFETIME(UMagicManager, ChangedSlot);
}

void UMagicManager::OnRep_SelectedSlot()
{
	OnSlotSelected.Broadcast(SelectedSlot);
}

int32 UMagicManager::GetFreeSlot() const
{
	for (int i = 0; i < 3; i++)
	{
		if (!Magics[i])
		{
			return i;
		}
	}
	return INDEX_NONE;
}
