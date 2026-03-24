#include "GMItem.h"
#include "GMItemManager.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"

void UGMItem::OnCarrierDied_Implementation(AActor* KillerActor){}

void UGMItem::MakeRoudPoint_Implementation()
{
	OnRoundPoint.Broadcast();
}

void UGMItem::OnUnequipped_Implementation(AActor* KillerActor){}

void UGMItem::OnEquipped_Implementation(){}

void UGMItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UGMItem, Carrier);
}