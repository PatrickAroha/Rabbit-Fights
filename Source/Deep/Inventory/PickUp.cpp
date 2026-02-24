// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "ItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
APickUp::APickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComp);
	
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
}

void APickUp::SetQuantity(int32 NewQty)
{
	if (HasAuthority())
	{
		Quantity = NewQty;
		ForceNetUpdate();
	}
}

void APickUp::OnRep_Def() // Ao Atualizar o Item
{
	if (Def && Def->Mesh)
	{
		MeshComp->SetStaticMesh(Def->Mesh);
	}
}

void APickUp::SetDef(UItemDefinition* NewDef)
{
	if (HasAuthority())
	{
		Def = NewDef;
		OnRep_Def();
		ForceNetUpdate();
	}
}

void APickUp::OnRep_Quantity() // Ao Atualizar a quantidade
{
	
}

void APickUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickUp, Def);
	DOREPLIFETIME(APickUp, Quantity);
}
