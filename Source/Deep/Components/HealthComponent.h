#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

struct FOnAttributeChangeData;
class UAbilitySystemComponent;
class UBasicAttributeSet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DEEP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY() UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY() AActor* LastDamageInstigator = nullptr;
	UPROPERTY() AActor* LastUsedWeapon = nullptr;
	UPROPERTY() UBasicAttributeSet* Attributes;

public:
	UPROPERTY(BlueprintReadOnly) bool bIsDead = false;
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION() void HandleDamage(AActor* Victim,  AActor* Instigator,  AActor* Weapon);

	UFUNCTION(BlueprintImplementableEvent) void Die(AActor* Killer, AActor* Weapon);
};
