#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameModeMatch.generated.h"

UCLASS()
class DEEP_API AMyGameModeMatch : public AGameMode
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UMyGameInstance> MyGI;

	UPROPERTY()
	TObjectPtr<class AMyGameState> MyGS;

	virtual void InitGameState() override;

public:
	UFUNCTION(BlueprintPure, Category="Refs")
	UMyGameInstance* GetMyGI() const { return MyGI; }

	UFUNCTION(BlueprintPure, Category="Refs")
	AMyGameState* GetMyGS() const { return MyGS; }
};
