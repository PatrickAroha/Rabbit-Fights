#include "RabbitAbilitySystemComponent.h"
#include "Characters/RabbitCharacterBase.h"

URabbitAbilitySystemComponent::URabbitAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URabbitAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URabbitAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	ARabbitCharacterBase *Character = Cast<ARabbitCharacterBase>(GetOwner());

	if (!Character) return;

	bool bAbilitiesChanged = false;
	if (LastActivatableAbilities.Num() != ActivatableAbilities.Items.Num())
	{
		bAbilitiesChanged = true;
	}
	else
	{
		for (int32 i = 0; i < LastActivatableAbilities.Num(); i++)
		{
			if (LastActivatableAbilities[i].Ability != ActivatableAbilities.Items[i].Ability)
			{
				bAbilitiesChanged = true;
				break;
			}
		}
	}

	if (bAbilitiesChanged)
	{
		Character->SendAbilitiesChangedEvent();
		LastActivatableAbilities = ActivatableAbilities.Items;
	}
}

void URabbitAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

