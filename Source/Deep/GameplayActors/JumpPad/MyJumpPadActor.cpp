// Fill out your copyright notice in the Description page of Project Settings.


#include "MyJumpPadActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"

AMyJumpPadActor::AMyJumpPadActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// Valores padrão (editáveis no BP)
	Trigger->InitCapsuleSize(100.f, 100.f);

	LaunchDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchDirection"));
	LaunchDirection->SetupAttachment(Root);
}

void AMyJumpPadActor::OnOverlapBegin(   UPrimitiveComponent* OverlappedComp,
										AActor* OtherActor,
										UPrimitiveComponent* OtherComp,
										int32 OtherBodyIndex,
										bool bFromSweep,
										const FHitResult& SweepResult
)
{
	if (!HasAuthority()) return;

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character) return;

	const FVector LaunchVelocity = (LaunchDirection->GetForwardVector() * ForwardForce) + (FVector::UpVector * UpForce);

	Character->LaunchCharacter(LaunchVelocity, true, true);
}

void AMyJumpPadActor::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyJumpPadActor::OnOverlapBegin);
}