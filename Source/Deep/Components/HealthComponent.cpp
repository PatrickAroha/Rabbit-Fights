#include "HealthComponent.h"
#include "Deep/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"



void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	AbilitySystemComponent = Owner->FindComponentByClass<UAbilitySystemComponent>();
    
	if (AbilitySystemComponent)
	{
		Attributes = const_cast<UBasicAttributeSet*>(AbilitySystemComponent->GetSet<UBasicAttributeSet>());
		if (Attributes)
		{
			Attributes->OnDamageReceived.AddUObject( this, &UHealthComponent::HandleDamage);
		}
	}
}


void UHealthComponent::HandleDamage(AActor* Victim,  AActor* Instigator,  AActor* Weapon)
{
	LastDamageInstigator = Instigator;
	LastUsedWeapon = Weapon;

	if (Attributes && Attributes->GetHealth() <= 0 && !bIsDead)
	{
		bIsDead = true;
		Die(LastDamageInstigator, Weapon);
	}
}
