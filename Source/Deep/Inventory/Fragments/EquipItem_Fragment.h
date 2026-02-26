// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Deep/Inventory/Fragment.h"
#include "EquipWeapon_Fragment.generated.h"

class ABaseItem;
/**
 * 
 */
UCLASS()
class DEEP_API UEquipWeapon_Fragment : public UFragment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Equip")
	FName SocketName = TEXT("hand_rSocket");

	UPROPERTY()
	TObjectPtr<ABaseItem> SpawnedActor;
	
	virtual void Activate(APawn* OwnerPawn, class UItemInstance* Item) override;
	
	virtual void Deactivate(APawn* OwnerPawn, class UItemInstance* Item) override;
};
