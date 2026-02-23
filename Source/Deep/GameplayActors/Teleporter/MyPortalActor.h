// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyPortalActor.generated.h"

class UBoxComponent;
class UArrowComponent;

UCLASS(Blueprintable)
class DEEP_API AMyPortalActor : public AActor
{
	GENERATED_BODY()

public:

	AMyPortalActor();

protected:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ExitArrow;

	// Portal de destino
	UPROPERTY(EditInstanceOnly, Category="Portal")
	AMyPortalActor* LinkedPortal;
	
	// ⏱ Cooldown por player (editável por instância)
	UPROPERTY(EditInstanceOnly, Category="Portal", meta=(ClampMin="0", UIMin="0"))
	float CooldownTime = 5.f;

	// Player → tempo em que poderá teleportar novamente
	TMap<TWeakObjectPtr<AActor>, float> CooldownPlayers;
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	bool CanActorTeleport(AActor* Actor) const;

	UFUNCTION()
	void RegisterCooldown(AActor* Actor);

	virtual void BeginPlay() override;
};
