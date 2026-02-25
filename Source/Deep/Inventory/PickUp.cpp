// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "ItemDefinition.h"
#include "ItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/ActorChannel.h"

APickUp::APickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComp);
	
	bReplicates = true;
	AActor::SetReplicateMovement(true);

}

void APickUp::BeginPlay()
{
	Super::BeginPlay();
	RefreshMesh();
}


void APickUp::OnRep_Item()
{
	RefreshMesh();
}

void APickUp::RefreshMesh()
{
	if (Item && Item->Def && Item->Def->Mesh)
	{
		MeshComp->SetStaticMesh(Item->Def->Mesh);
	}
}

bool APickUp::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWrote = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (Item)
	{
		bWrote |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
	}

	return bWrote;
}

void APickUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickUp, Item);
}
