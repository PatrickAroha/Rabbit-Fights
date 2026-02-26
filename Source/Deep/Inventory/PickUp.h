// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

class UItemInstance;
class UStaticMeshComponent;

UCLASS()
class DEEP_API APickUp : public AActor
{
	GENERATED_BODY()

public:

	APickUp();
	void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PickUp")
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Item, Instanced, Category="PickUp")
	TObjectPtr<UItemInstance> Item;

	UFUNCTION()
	void OnRep_Item();
	void RefreshMesh();

	// PickUp.h
    virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
