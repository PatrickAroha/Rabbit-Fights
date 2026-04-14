#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "E_MagicSlot.h"
#include "Deep/Inventory/ItemInstance.h"
#include "MagicManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMagicAdded, int32, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotSelected,  EMagicSlot, Slot);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEEP_API UMagicManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UMagicManager();

	UItemInstance* TryCollectMagic(UItemInstance* MagicItem);
	void UpdateSlot(int32 Slot, UItemInstance* NewMagic);

	FGameplayAbilitySpecHandle GiveMagicAbility(UItemInstance* MagicItem);
	
	void RemoveMagicAbility(FGameplayAbilitySpecHandle Handle, int32 Slot);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RemoveMagicAbility(EMagicSlot Slot);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_ChangeSlot(EMagicSlot Slot);

	UFUNCTION()
	void UnequipMagic(EMagicSlot LastSlot);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RSUnequipMagic(EMagicSlot Slot);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_DropMagic(uint8 Slot);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SwapMagics();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_ActiveMagic(EMagicSlot Slot);

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	UFUNCTION()
	void OnRep_Magics();
	
	UPROPERTY(BlueprintAssignable)
	FOnMagicAdded OnMagicAdded;

	UPROPERTY(BlueprintAssignable)
	FOnSlotSelected OnSlotSelected;

	UPROPERTY(ReplicatedUsing=OnRep_Magics, VisibleAnywhere, BlueprintReadOnly)
	TArray<UItemInstance*> Magics;

	UPROPERTY(Replicated)
	FGameplayAbilitySpecHandle MagicHandles[3];

	UPROPERTY(ReplicatedUsing=OnRep_SelectedSlot, EditAnywhere, BlueprintReadWrite)
	EMagicSlot SelectedSlot;

	UFUNCTION()
	void OnRep_SelectedSlot();

	UPROPERTY(Replicated)
	int32 ChangedSlot = -1;
	
private:
	int32 GetFreeSlot() const;
};