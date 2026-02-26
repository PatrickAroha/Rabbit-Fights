#include "Deep/Inventory/Fragments/EquipWeapon_Fragment.h"

#include "Deep/Inventory/BaseItem.h"
#include "Deep/Inventory/ItemDefinition.h"
#include "Deep/Inventory/ItemInstance.h"
#include "GameFramework/Character.h"


void UEquipWeapon_Fragment::Activate(APawn* OwnerPawn, UItemInstance* Item)
{
	if (!OwnerPawn || !OwnerPawn->HasAuthority())
		return;

	if (!Item || !Item->Def || !Item->Def->ActorItemClass)
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
		Item->Def->ActorItemClass,
		FTransform::Identity,
		Params
	);

	if (!SpawnedActor)
		return;
	
	USkeletalMeshComponent* Mesh = OwnerPawn->FindComponentByClass<USkeletalMeshComponent>();

	if (Mesh)
	{
		SpawnedActor->AttachToComponent(Mesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName
		);
	}
	
	SpawnedActor->InitFromInstance(Item);
}

void UEquipWeapon_Fragment::Deactivate(APawn* OwnerPawn, UItemInstance* Item)
{
	if (!OwnerPawn || !OwnerPawn->HasAuthority())
		return;

	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Destroy();
		SpawnedActor = nullptr;
	}
}