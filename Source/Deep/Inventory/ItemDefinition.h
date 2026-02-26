#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.generated.h"

class APickUp;

UCLASS(BlueprintType, Blueprintable)
class DEEP_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stack")
	bool bStackable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stack", meta=(ClampMin="1"))
	int32 MaxStack = 99;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Design")
	UTexture2D* ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Design")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SpawnActor")
	TSubclassOf<APickUp> PickupClass;
};