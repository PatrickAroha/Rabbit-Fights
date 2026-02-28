#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"


UCLASS()
class DEEP_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:

	ABaseItem();
	
	virtual void InitFromInstance(class UItemInstance* Instance);
};
