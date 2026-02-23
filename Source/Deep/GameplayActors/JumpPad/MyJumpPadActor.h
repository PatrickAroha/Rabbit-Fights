// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyJumpPadActor.generated.h"

class UCapsuleComponent;
class UArrowComponent;

UCLASS()
class DEEP_API AMyJumpPadActor : public AActor
{
	GENERATED_BODY()
public:
	AMyJumpPadActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Trigger;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* LaunchDirection;

	UPROPERTY(EditAnywhere, Category = "JumpPad", meta = (ClampMin="0", UIMin = "0"))
	float UpForce = 1200.f;

	UPROPERTY(EditAnywhere, Category = "JumpPad", meta = (ClampMin="0", UIMin = "0"))
	float ForwardForce = 1500.f;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
