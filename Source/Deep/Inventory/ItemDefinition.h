#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NiagaraComponent.h"
#include "ItemDefinition.generated.h"

class ABaseItem;
class APickUp;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Potion UMETA(DisplayName = "Potion"),
	Throwable UMETA(DisplayName = "Throwable")
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Item UMETA(DisplayName = "Item"),
	Magic UMETA(DisplayName = "Magic")
}; 

UCLASS(BlueprintType, Blueprintable)
class DEEP_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	//======= Base =========

	UPROPERTY(EditDefaultsOnly)
	EItemCategory ItemCategory;
	
	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ByMinigame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<APickUp> PickupClass;

	// ===== ITEM =====

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="ItemCategory == EItemCategory::Item", EditConditionHides))
	TObjectPtr<UStaticMesh> ItemMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="ItemCategory == EItemCategory::Item", EditConditionHides))
	TSubclassOf<ABaseItem> ActorItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="ItemCategory == EItemCategory::Item", EditConditionHides))
	bool bStackable = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="ItemCategory == EItemCategory::Item", EditConditionHides , ClampMin="1"))
	int32 MaxStack = 99;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
	int32 Cooldown = 0;

	// ===== MAGIC =====
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="ItemCategory == EItemCategory::Magic", EditConditionHides))
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CueMagicHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	TObjectPtr<UParticleSystem> CascadeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX")
	TObjectPtr<UNiagaraSystem> NiagaraEffect;
	
};