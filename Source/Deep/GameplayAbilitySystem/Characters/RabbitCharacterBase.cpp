// Fill out your copyright notice in the Description page of Project Settings.


#include "RabbitCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Deep/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"

// Sets default values
ARabbitCharacterBase::ARabbitCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Add The Ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ASCReplicationMode);

	// Add the basic attribute set
	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));
}

void ARabbitCharacterBase::CheckJumpInput(float DeltaTime)
{
	JumpCurrentCountPreJump = JumpCurrentCount;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp || !bPressedJump)
		return;
	
	const bool bDidJump = CanJump() && MoveComp->DoJump(bClientUpdating, DeltaTime);

	if (bDidJump && !bWasJumping)
	{
		JumpCurrentCount++;
		JumpForceTimeRemaining = GetJumpMaxHoldTime();
		OnJumped();
	}

	bWasJumping = bDidJump;
}


// Called when the game starts or when spawned
void ARabbitCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARabbitCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

// Called every frame
void ARabbitCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARabbitCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARabbitCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

UAbilitySystemComponent* ARabbitCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
