// Fill out your copyright notice in the Description page of Project Settings.


#include "RabbitCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Deep/GameplayAbilitySystem/RabbitAbilitySystemComponent.h"
#include "Deep/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"

// Sets default values
ARabbitCharacterBase::ARabbitCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Add The Ability system component
	AbilitySystemComponent = CreateDefaultSubobject<URabbitAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ASCReplicationMode);

	// Add the basic attribute set
	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(UBasicAttributeSet::GetHealthAttribute()).AddUObject(this, &ARabbitCharacterBase::OnHealthChanged);
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
		GenerateWidgetOnSpawn();
		GrantAbilities(StartingAbilities);
	}
}

void ARabbitCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GenerateWidgetOnSpawn();
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

UAbilitySystemComponent* ARabbitCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

TArray<FGameplayAbilitySpecHandle> ARabbitCharacterBase::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	if (!AbilitySystemComponent || !HasAuthority()) return TArray<FGameplayAbilitySpecHandle>();

	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	
	for (TSubclassOf<UGameplayAbility> Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));

		AbilityHandles.Add(SpecHandle);
	}
	
	SendAbilitiesChangedEvent();
	return AbilityHandles;
}

void ARabbitCharacterBase::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	for (FGameplayAbilitySpecHandle AbilityHandle : AbilityHandlesToRemove)
	{
		AbilitySystemComponent->ClearAbility(AbilityHandle);
	}

	SendAbilitiesChangedEvent();
}

void ARabbitCharacterBase::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData;
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Abilities.Changed"));
	EventData.Instigator = this;
	EventData.Target = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}

void ARabbitCharacterBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.f)
	{
		Die();
	}
}

