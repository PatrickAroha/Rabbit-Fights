// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

class UItemDefinition;
class UStaticMeshComponent;

UCLASS()
class DEEP_API APickUp : public AActor
{
	GENERATED_BODY()

public:

	APickUp();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
	UPROPERTY(ReplicatedUsing=OnRep_Def, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDefinition> Def = nullptr;
	
	UPROPERTY(ReplicatedUsing=OnRep_Quantity, EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;
	
	UFUNCTION(BlueprintCallable)
	void SetQuantity(int32 NewQty);
	
	UFUNCTION(BlueprintCallable)
	void SetDef(UItemDefinition* NewDef);
	
	UFUNCTION()
	void OnRep_Def();
	
	UFUNCTION()
	void OnRep_Quantity();

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
