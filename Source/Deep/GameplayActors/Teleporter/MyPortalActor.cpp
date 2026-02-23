// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPortalActor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"

AMyPortalActor::AMyPortalActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ExitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ExitArrow"));
	ExitArrow->SetupAttachment(Root);
}

bool AMyPortalActor::CanActorTeleport(AActor* Actor) const
{
	if (!Actor) return false;

	const float* AllowedTime = CooldownPlayers.Find(Actor);
	if (!AllowedTime) return true;

	return GetWorld()->GetTimeSeconds() >= *AllowedTime;
}

void AMyPortalActor::RegisterCooldown(AActor* Actor)
{
	if (!Actor) return;

	const float NextAllowedTime = GetWorld()->GetTimeSeconds() + CooldownTime;

	CooldownPlayers.Add(Actor, NextAllowedTime);
}

void AMyPortalActor::OnOverlapBegin( UPrimitiveComponent* OverlappedComp,
									 AActor* OtherActor,
									 UPrimitiveComponent* OtherComp,
									 int32 OtherBodyIndex,
									 bool bFromSweep,
									 const FHitResult& SweepResult
)
{
	if (!HasAuthority()) return;

	if (!LinkedPortal) return;

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character) return;

	// Ainda em cooldown?
	if (!CanActorTeleport(Character)) return;

	// Registra cooldown NOS DOIS portais
	RegisterCooldown(Character);
	LinkedPortal->RegisterCooldown(Character);

	// Teleporta
	Character->TeleportTo( LinkedPortal->ExitArrow->GetComponentLocation(), LinkedPortal->ExitArrow->GetComponentRotation());
}

void AMyPortalActor::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyPortalActor::OnOverlapBegin);
}